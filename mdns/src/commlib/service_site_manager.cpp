/*
 * service_site_manager.cpp
 *
 *  Created on: 2022年4月22日
 *      Author: van
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mutex>
#include <semaphore.h>
#include "httplib.h"
#include "nlohmann/json.hpp"
#include"service_site_manager.h"

const string OK_RESPONSE_JSON = "{\"code\": 0, \"error\": \"ok\"}";

const string ERROR_RESPONSE_JSON_FORMAT = "{\"code\": -2, \"error\": \"json format error\"}";
const string ERROR_RESPONSE_NOT_SERVICE_OR_MESSAGE = "{\"code\": -3, \"error\": \"not service or message\"}";
const string ERROR_RESPONSE_RESPONSE_IS_NULL = "{\"code\": -4, \"error\": \"response is null\"}";
const string ERROR_RESPONSE_REQUEST_HANDLER_ERROR = "{\"code\": -5, \"error\": \"request handler error\"}";
const string ERROR_RESPONSE_NO_REQUEST_HANDLER_MATCH = "{\"code\": -6, \"error\": \"no request handler match\"}";
const string ERROR_RESPONSE_REQUEST_ILLEGAL = "{\"code\": -7, \"error\": \"request illegal\"}";
const string ERROR_RESPONSE_NO_MESSAGE_HANDLER_MATCH = "{\"code\": -8, \"error\": \"no message handler match\"}";

using namespace std;
using namespace servicesite;
using namespace httplib;

using json = nlohmann::json;

const string ServiceSiteManager::SERVICE_ID_GET_SERVICE_LIST = "get_service_list";
const string ServiceSiteManager::SERVICE_ID_GET_MESSAGE_LIST = "get_message_list";
const string ServiceSiteManager::SERVICE_ID_SUBSCRIBE_MESSAGE = "subscribe_message";
const string ServiceSiteManager::SERVICE_ID_UNSUBSCRIBE_MESSAGE = "unsubscribe_message";
const string ServiceSiteManager::SERVICE_ID_DEBUG = "debug";

std::vector<SiteHandle> ServiceSiteManager::siteHandleList;
std::vector<MessageSubscriber> ServiceSiteManager::messageSubscriberList;
std::vector<MessageSubscriberSiteHandle*> ServiceSiteManager::messageSubscriberSiteHandlePList;

ServiceRequestHandlers ServiceSiteManager::serviceRequestHandlers;
MessageIds ServiceSiteManager::messageIds;
MessageHandlers ServiceSiteManager::messageHandlers;

const int ServiceSiteManager::RET_CODE_OK = 0;

const int ServiceSiteManager::RET_CODE_ERROR_START_SERVER = -1;

const int ServiceSiteManager::RET_CODE_ERROR_REQ_CONN = -1;
const int ServiceSiteManager::RET_CODE_ERROR_REQ_STATUS_CODE = -2;
const int ServiceSiteManager::RET_CODE_ERROR_REQ_NOT_JSON = -3;
const int ServiceSiteManager::RET_CODE_ERROR_REQ_JSON_FORMAT = -4;
const int ServiceSiteManager::RET_CODE_ERROR_REQ_CODE = -5;

const string ServiceSiteManager::RET_OK = "ok";

const string ServiceSiteManager::MESSAGE_SUBSCRIBER_CONFIG_FILE = "_message_subscriber.json";
string ServiceSiteManager::messageSubscriberConfigPath = "/data/changhong/edge_midware/";

std::mutex init_mutex; // 保护 初始化
std::mutex http_request_mutex; // 保护 http_request handler
std::mutex messageSubscriberList_mutex; // 保护 MessageSubscriberList

string ServiceSiteManager::siteId;
int ServiceSiteManager::serverPort;
Server ServiceSiteManager::server;

string ServiceSiteManager::summary;

std::thread* ServiceSiteManager::pingThreadP;

ServiceSiteManager ServiceSiteManager::instance;

int ServiceSiteManager::registerServiceRequestHandler(string serviceId, ServiceRequestHandler handler) {
    // 线程锁, 对象析构时解锁
    std::lock_guard<std::mutex> lockGuard(init_mutex);
    
    serviceRequestHandlers.push_back(std::make_pair(serviceId, handler));

    return RET_CODE_OK;
}

int ServiceSiteManager::registerMessageId(string messageId) {
    messageIds.push_back(messageId);

    return RET_CODE_OK;
}

int ServiceSiteManager::serviceRequestHandlerGetServiceList(const Request& request, Response& response) {
    json response_json = {
		{"code", 0},
		{"error", "ok"},
		{"response", {
            {"service_list", json::array()}
        }}
	};

    for (const auto& item : serviceRequestHandlers) {
        response_json["response"]["service_list"].push_back(item.first);
    }

    response.set_content(response_json.dump(), "text/plain");

    return RET_CODE_OK;
}

int ServiceSiteManager::serviceRequestHandlerGetMessageList(const Request& request, Response& response) {
    json response_json = {
		{"code", 0},
		{"error", "ok"},
		{"response", {
            {"message_list", json::array()}
        }}
	};

    for (const auto& item : messageIds) {
        response_json["response"]["message_list"].push_back(item);
    }

    response.set_content(response_json.dump(), "text/plain");

    return RET_CODE_OK;
}

int ServiceSiteManager::serviceRequestHandlerSubscribeMessage(const Request& request, Response& response) {
    string ip = request.remote_addr;

    json request_json = json::parse(request.body);

    bool need_save = false;

    if (request_json["request"].is_null()) {
        response.set_content(ERROR_RESPONSE_REQUEST_ILLEGAL, "text/plain");
        return RET_CODE_OK;
    }

    if (request_json["request"]["port"].is_null()) {
        response.set_content(ERROR_RESPONSE_REQUEST_ILLEGAL, "text/plain");
        return RET_CODE_OK;
    }

    if (request_json["request"]["message_list"].is_null()) {
        response.set_content(ERROR_RESPONSE_REQUEST_ILLEGAL, "text/plain");
        return RET_CODE_OK;
    }

    int port = request_json["request"]["port"];

    for (auto& message_id : request_json["request"]["message_list"]) {
        need_save = subscribeMessage(message_id, ip, port);
    }

    response.set_content(OK_RESPONSE_JSON, "text/plain");

    if (need_save) {
        saveMessageSubscriber();
    }
    
    return RET_CODE_OK;
}

int ServiceSiteManager::serviceRequestHandlerUnsubscribeMessage(const Request& request, Response& response) {
    string ip = request.remote_addr;

    json request_json = json::parse(request.body);

    bool need_save = false;
    
    if (request_json["request"].is_null()) {
        response.set_content(ERROR_RESPONSE_REQUEST_ILLEGAL, "text/plain");
        return RET_CODE_OK;
    }

    if (request_json["request"]["port"].is_null()) {
        response.set_content(ERROR_RESPONSE_REQUEST_ILLEGAL, "text/plain");
        return RET_CODE_OK;
    }

    if (request_json["request"]["message_list"].is_null()) {
        response.set_content(ERROR_RESPONSE_REQUEST_ILLEGAL, "text/plain");
        return RET_CODE_OK;
    }

    int port = request_json["request"]["port"];

    for (auto& json_message_id : request_json["request"]["message_list"]) {
        // 线程锁, 对象析构时解锁
        std::lock_guard<std::mutex> lockGuard(messageSubscriberList_mutex);

        MessageSubscriberSiteHandle* temp_messageSubscriberSiteHandle = NULL;
        for (auto& item : messageSubscriberSiteHandlePList) {
            if (item->getIp() == ip) {
                if (item->getPort() == port) {
                    temp_messageSubscriberSiteHandle = item;
                    break;
                }
            }
        }
        if (temp_messageSubscriberSiteHandle == NULL) {
            // 此站点没有订阅过， 忽略
            response.set_content(OK_RESPONSE_JSON, "text/plain");
            return RET_CODE_OK;
        }
        
        MessageSubscriber* temp_messageSubscriber = NULL;
        for (auto& item : messageSubscriberList) {
            if (json_message_id == item.getMessageId()) {
                temp_messageSubscriber = &item;
            }
        }
        if (temp_messageSubscriber == NULL) {
            // 此消息没有订阅过， 忽略
            response.set_content(OK_RESPONSE_JSON, "text/plain");
            return RET_CODE_OK;
        }
        else {
            temp_messageSubscriber->delSiteMessageSubscriberSiteHandleP(temp_messageSubscriberSiteHandle);

            need_save = true;
        }
    }

    response.set_content(OK_RESPONSE_JSON, "text/plain");

    if (need_save) {
        saveMessageSubscriber();
    }

    return RET_CODE_OK;
}

int ServiceSiteManager::serviceRequestHandlerDebug(const Request& request, Response& response) {
    json response_json = {
		{"code", 0},
		{"error", "ok"},
		{"response", {
            {"message_subscriber_list", json::array()},
            {"message_subscriber_site_handle_list", json::array()}
        }}
	};

    for (auto& item : messageSubscriberList) {
        json item_json = {
            {"messageId", item.getMessageId()},
            {"site_handle_list", json::array()}
        };

        for (const auto& sub_item : item.getSiteMessageSubscriberSiteHandlePlist()) {
            json sub_item_json = {
                {"ip", sub_item->getIp()},
                {"port", sub_item->getPort()},
                {"sendRetryCount", sub_item->getSendRetryCount()},
                {"isStop", sub_item->getIsStop()},
            };

            item_json["site_handle_list"].push_back(sub_item_json);
        }

        response_json["response"]["message_subscriber_list"].push_back(item_json);
    }

    for (const auto& item : messageSubscriberSiteHandlePList) {
        json item_json = {
            {"ip", item->getIp()},
            {"port", item->getPort()},
            {"sendRetryCount", item->getSendRetryCount()},
            {"isStop", item->getIsStop()},
        };
        response_json["response"]["message_subscriber_site_handle_list"].push_back(item_json);
    }

    response.set_content(response_json.dump(), "text/plain");

    return RET_CODE_OK;
}

ServiceSiteManager::ServiceSiteManager() {
    registerServiceRequestHandler(SERVICE_ID_GET_SERVICE_LIST, ServiceSiteManager::serviceRequestHandlerGetServiceList);
    registerServiceRequestHandler(SERVICE_ID_GET_MESSAGE_LIST, ServiceSiteManager::serviceRequestHandlerGetMessageList);
    registerServiceRequestHandler(SERVICE_ID_SUBSCRIBE_MESSAGE, ServiceSiteManager::serviceRequestHandlerSubscribeMessage);
    registerServiceRequestHandler(SERVICE_ID_UNSUBSCRIBE_MESSAGE, ServiceSiteManager::serviceRequestHandlerUnsubscribeMessage);

    registerServiceRequestHandler(SERVICE_ID_DEBUG, ServiceSiteManager::serviceRequestHandlerDebug);
}

void ServiceSiteManager::rawHttpRequestHandler(const Request& request, Response& response) {
    // 线程锁, 对象析构时解锁
    std::lock_guard<std::mutex> lockGuard(http_request_mutex);

    // printf("%s\n", request.body.c_str());

    if (!json::accept(request.body)) {
        response.set_content(ERROR_RESPONSE_JSON_FORMAT, "text/plain");
        return;
    }

    auto request_json = json::parse(request.body);

    // Service
    if (!request_json["service_id"].is_null()) {
        string request_service_id = request_json["service_id"];
        for (const auto& x : serviceRequestHandlers) {
            const auto& serviceId = x.first;
            const auto& handler = x.second;

            if (serviceId == request_service_id) {
                int code = handler(request, response);
                if (code == 0) {
                    // 完成处理
                    return;
                }
                else {
                    // code 出错
                    response.set_content(ERROR_RESPONSE_REQUEST_HANDLER_ERROR, "text/plain");
                }
                break;
            }
        }

        // 没有匹配的 handler
        printf("no handler for service_id: %s\n", request_service_id.c_str());
        response.set_content(ERROR_RESPONSE_NO_REQUEST_HANDLER_MATCH, "text/plain");
        return;
    }

    // Message
    if (!request_json["message_id"].is_null()) {
        string request_message_id = request_json["message_id"];

        for (const auto &x : messageHandlers) {
            const auto &messageId = x.first;
            const auto &handler = x.second;

            if (messageId == request_message_id) {
                handler(request);
                response.set_content("{}", "text/plain");
                return;
            }
        }

        // 没有匹配的 handler
        printf("no handler for message_id: %s\n", request_message_id.c_str());
        response.set_content(ERROR_RESPONSE_NO_MESSAGE_HANDLER_MATCH, "text/plain");
        return;
    }

    response.set_content(ERROR_RESPONSE_NOT_SERVICE_OR_MESSAGE, "text/plain");
}

int ServiceSiteManager::start(void) {
    server.Post("/", ServiceSiteManager::rawHttpRequestHandler);

    loadMessageSubscriber();

    // server.set_read_timeout(0, 500000);
    // server.set_write_timeout(0, 500000);

    if (!server.listen("0.0.0.0", serverPort)) {
        return RET_CODE_ERROR_START_SERVER;
    }

    return RET_CODE_OK;
}

void  service_site_ping_thread(string siteId) {
    json request_json = {
		{"service_id", "site_ping"},
		{"request", {
            {"site_id", siteId}
        }}
	};

    while (true) {
        Client cli("127.0.0.1", ServiceSiteManager::SITE_QUERY_PORT);

        cli.Post("/", request_json.dump(), "text/plain");

        sleep(ServiceSiteManager::PING_PER_SECONDS);
    }
}

int ServiceSiteManager::startByRegister(void) {
    json request_json = {
		{"service_id", "site_register"},
		{"request", {
			{"site_id", siteId},
            {"summary", summary},
            {"port", serverPort}
		}}
	};

    Client cli("127.0.0.1", SITE_QUERY_PORT);

    auto res = cli.Post("/", request_json.dump(), "text/plain");
    if (!res) {
        printf("client connect error.\n");
        return RET_CODE_ERROR_REQ_CONN;
    }

    if (res->status != 200) {
        printf("http status = %d, error.\n", res->status);
        return RET_CODE_ERROR_REQ_STATUS_CODE;
    }

    if (!json::accept(res->body)) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_NOT_JSON;
    }

    json response_json = json::parse(res->body);

    if (response_json["code"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    // if (response_json["response"].is_null()) {
    //     printf("response_json format error.\n");
    //     return RET_CODE_ERROR_REQ_JSON_FORMAT;
    // }

    pingThreadP = new std::thread(service_site_ping_thread, siteId);

    printf("http listen port: %d\n", serverPort);

    server.Post("/", ServiceSiteManager::rawHttpRequestHandler);

    loadMessageSubscriber();

    if (!server.listen("0.0.0.0", serverPort)) {
        return RET_CODE_ERROR_START_SERVER;
    }

    return RET_CODE_OK;
}

int ServiceSiteManager::registerMessageHandler(string messageId, MessageHandler handler) {
    messageHandlers.push_back(std::make_pair(messageId, handler));

    return RET_CODE_OK;
}

void ServiceSiteManager::publishMessage(string messageId, string message) {
    // 线程锁, 对象析构时解锁
    std::lock_guard<std::mutex> lockGuard(messageSubscriberList_mutex);

    for (auto& messageSubscriberItem : messageSubscriberList) {
        if (messageId == messageSubscriberItem.getMessageId()) {
            for (auto& siteHandlePItem : messageSubscriberItem.getSiteMessageSubscriberSiteHandlePlist()) {
                siteHandlePItem->sendMessage(message);
            }
        }
    }
}

int ServiceSiteManager::subscribeMessage(string ip, int port, std::vector<string> messageIdList) {
    json request_json = {
		{"service_id", "subscribe_message"},
		{"request", {
			{"message_list", json::array()},
            {"port", serverPort}
		}}
	};

    for (auto& item : messageIdList) {
        request_json["request"]["message_list"].push_back(item);
    }

    Client cli(ip, port);
    cli.set_connection_timeout(1, 0);

    auto res = cli.Post("/", request_json.dump(), "text/plain");
    if (!res) {
        printf("client connect error.\n");
        return RET_CODE_ERROR_REQ_CONN;
    }

    if (res->status != 200) {
        printf("http status = %d, error.\n", res->status);
        return RET_CODE_ERROR_REQ_STATUS_CODE;
    }

    if (!json::accept(res->body)) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_NOT_JSON;
    }

    json response_json = json::parse(res->body);

    if (response_json["code"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["code"] != 0) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_CODE;
    }

    return RET_CODE_OK;
}


int ServiceSiteManager::unsubscribeMessage(string ip, int port, std::vector<string> messageIdList) {
    json request_json = {
		{"service_id", "unsubscribe_message"},
		{"request", {
			{"message_list", json::array()},
            {"port", serverPort}
		}}
	};

    for (auto& item : messageIdList) {
        request_json["request"]["message_list"].push_back(item);
    }

    Client cli(ip, port);

    auto res = cli.Post("/", request_json.dump(), "text/plain");
    if (!res) {
        printf("client connect error.\n");
        return RET_CODE_ERROR_REQ_CONN;
    }

    if (res->status != 200) {
        printf("http status = %d, error.\n", res->status);
        return RET_CODE_ERROR_REQ_STATUS_CODE;
    }

    if (!json::accept(res->body)) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_NOT_JSON;
    }

    json response_json = json::parse(res->body);

    if (response_json["code"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["code"] != 0) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_CODE;
    }

    return RET_CODE_OK;
}

int ServiceSiteManager::createDir(string sPathName) {
    char DirName[sPathName.length() + 1];
    strcpy(DirName, sPathName.c_str());
    int i, len = strlen(DirName);
    
    for (i = 1; i < len; i++) {
        if (DirName[i] == '/') {
            DirName[i] = 0;
            if (access(DirName, F_OK) != 0) {
                if (mkdir(DirName, 0755) == -1) {
                    printf("mkdir error\n");
                    return -1;
                }
            }
            DirName[i] = '/';
        }
    }

    return 0;
}

int ServiceSiteManager::getServiceList(string ip, int port, std::vector<string>& serviceIdList) {
    json request_json = {
		{"service_id", "get_service_list"}
	};

    Client cli(ip, port);

    auto res = cli.Post("/", request_json.dump(), "text/plain");
    if (!res) {
        printf("client connect error.\n");
        return RET_CODE_ERROR_REQ_CONN;
    }

    if (res->status != 200) {
        printf("http status = %d, error.\n", res->status);
        return RET_CODE_ERROR_REQ_STATUS_CODE;
    }

    if (!json::accept(res->body)) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_NOT_JSON;
    }

    json response_json = json::parse(res->body);

    if (response_json["code"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["code"] != 0) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_CODE;
    }

    if (response_json["response"].is_null()) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["response"]["service_list"].is_null()) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    for (auto& item : response_json["response"]["service_list"]) {
        serviceIdList.push_back(item);
    }

    return RET_CODE_OK;
}


int ServiceSiteManager::getMessageList(string ip, int port, std::vector<string>& messageIdList) {
    json request_json = {
		{"service_id", "get_message_list"}
	};

    Client cli(ip, port);
    cli.set_connection_timeout(1, 0);

    auto res = cli.Post("/", request_json.dump(), "text/plain");
    if (!res) {
        printf("client connect error.\n");
        return RET_CODE_ERROR_REQ_CONN;
    }

    if (res->status != 200) {
        printf("http status = %d, error.\n", res->status);
        return RET_CODE_ERROR_REQ_STATUS_CODE;
    }

    if (!json::accept(res->body)) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_NOT_JSON;
    }

    json response_json = json::parse(res->body);

    if (response_json["code"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["code"] != 0) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_CODE;
    }

    if (response_json["response"].is_null()) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["response"]["message_list"].is_null()) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    for (auto& item : response_json["response"]["message_list"]) {
        messageIdList.push_back(item);
    }

    return RET_CODE_OK;
}

int ServiceSiteManager::updateSiteHandleList(void) {
    json request_json = {
		{"service_id", "site_query"}
	};

    // 先完成本机，后续完成mDNS， 本局域网
    string query_site_ip = "127.0.0.1";
    Client cli(query_site_ip, ServiceSiteManager::SITE_QUERY_PORT);

    auto res = cli.Post("/", request_json.dump(), "text/plain");
    if (!res) {
        printf("client connect error.\n");
        return RET_CODE_ERROR_REQ_CONN;
    }

    if (res->status != 200) {
        printf("http status = %d, error.\n", res->status);
        return RET_CODE_ERROR_REQ_STATUS_CODE;
    }

    if (!json::accept(res->body)) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_NOT_JSON;
    }

    json response_json = json::parse(res->body);

    if (response_json["code"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["response"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["response"]["site_list"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["code"] != 0) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_CODE;
    }

    std::vector<SiteHandle> tempSiteHandleList;
    for (auto& json_item : response_json["response"]["site_list"]) {
        if (json_item["site_id"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        if (json_item["summary"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        // 与 query_site 一致 
        if (json_item["ip"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        if (json_item["port"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        string site_id = json_item["site_id"];
        string summary = json_item["summary"];
        // 与 query_site 一致 
        string ip = json_item["ip"];
        int port = json_item["port"];

        SiteHandle siteHandle(site_id, summary, query_site_ip, port);
        
        tempSiteHandleList.push_back(siteHandle);
    }

    std::swap(siteHandleList, tempSiteHandleList);
    // siteHandleList.swap(tempSiteHandleList);

    return RET_CODE_OK;
}

int ServiceSiteManager::querySiteList(std::vector<SiteHandle>& pSiteHandleList) {
    json request_json = {
		{"service_id", "site_query"}
	};

    // 先完成本机，后续完成mDNS， 本局域网
    string query_site_ip = "127.0.0.1";
    Client cli(query_site_ip, ServiceSiteManager::SITE_QUERY_PORT);

    auto res = cli.Post("/", request_json.dump(), "text/plain");
    if (!res) {
        printf("client connect error.\n");
        return RET_CODE_ERROR_REQ_CONN;
    }

    if (res->status != 200) {
        printf("http status = %d, error.\n", res->status);
        return RET_CODE_ERROR_REQ_STATUS_CODE;
    }

    if (!json::accept(res->body)) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_NOT_JSON;
    }

    json response_json = json::parse(res->body);

    if (response_json["code"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["response"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["response"]["site_list"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["code"] != 0) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_CODE;
    }

    for (auto& json_item : response_json["response"]["site_list"]) {
        if (json_item["site_id"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        if (json_item["summary"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        // 与 query_site 一致 
        if (json_item["ip"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        if (json_item["port"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        string site_id = json_item["site_id"];
        string summary = json_item["summary"];
        // 与 query_site 一致 
        string ip = json_item["ip"];
        int port = json_item["port"];

        SiteHandle siteHandle(site_id, summary, query_site_ip, port);
        
        pSiteHandleList.push_back(siteHandle);
    }

    return RET_CODE_OK;
}

int ServiceSiteManager::querySiteListBySiteId(string pSiteId, std::vector<SiteHandle>& pSiteHandleList) {
    json request_json = {
		{"service_id", "site_query"},
        {"request", {
            {"site_id", pSiteId}
        }}
	};

    // 先完成本机，后续完成mDNS， 本局域网
    string query_site_ip = "127.0.0.1";
    Client cli(query_site_ip, ServiceSiteManager::SITE_QUERY_PORT);

    auto res = cli.Post("/", request_json.dump(), "text/plain");
    if (!res) {
        printf("client connect error.\n");
        return RET_CODE_ERROR_REQ_CONN;
    }

    if (res->status != 200) {
        printf("http status = %d, error.\n", res->status);
        return RET_CODE_ERROR_REQ_STATUS_CODE;
    }

    if (!json::accept(res->body)) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_NOT_JSON;
    }

    json response_json = json::parse(res->body);

    if (response_json["code"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["response"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["response"]["site_list"].is_null()) {
        printf("response_json format error.\n");
        return RET_CODE_ERROR_REQ_JSON_FORMAT;
    }

    if (response_json["code"] != 0) {
        printf("response_json code error.\n");
        return RET_CODE_ERROR_REQ_CODE;
    }

    for (auto& json_item : response_json["response"]["site_list"]) {
        if (json_item["site_id"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        if (json_item["summary"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        // 与 query_site 一致 
        if (json_item["ip"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        if (json_item["port"].is_null()) {
            printf("response_json format error.\n");
            return RET_CODE_ERROR_REQ_JSON_FORMAT;
        }

        string site_id = json_item["site_id"];
        string summary = json_item["summary"];
        // 与 query_site 一致 
        string ip = json_item["ip"];
        int port = json_item["port"];

        SiteHandle siteHandle(site_id, summary, query_site_ip, port);
        
        pSiteHandleList.push_back(siteHandle);
    }

    return RET_CODE_OK;
}

SiteHandle::SiteHandle(string pSiteId, string pSummary, string pIp, int pPort) {
	siteId = pSiteId;
    summary = pSummary;
    ip = pIp;
    port = pPort;
}

MessageSubscriber::MessageSubscriber(string pMessageId) {
    messageId = pMessageId;
}

string MessageSubscriber::getMessageId(void) {
    return messageId;
}

void MessageSubscriber::addSiteMessageSubscriberSiteHandleP(MessageSubscriberSiteHandle* pMessageSubscriberSiteHandle) {
    MessageSubscriberSiteHandle* temp = NULL;
    for (auto& item : siteMessageSubscriberSiteHandlePlist) {
        if (item == pMessageSubscriberSiteHandle) {
            // 已存在
            temp = item;
            break;
        }
    }

    if (temp == NULL) {
        // 不存在 添加
        siteMessageSubscriberSiteHandlePlist.push_back(pMessageSubscriberSiteHandle);
    }

    pMessageSubscriberSiteHandle->setIsStop(false);
}

void MessageSubscriber::delSiteMessageSubscriberSiteHandleP(MessageSubscriberSiteHandle* pMessageSubscriberSiteHandle) {
    std::vector<MessageSubscriberSiteHandle*>::iterator iter=std::find(siteMessageSubscriberSiteHandlePlist.begin(), siteMessageSubscriberSiteHandlePlist.end(), pMessageSubscriberSiteHandle);
    siteMessageSubscriberSiteHandlePlist.erase(iter);
}

std::vector<MessageSubscriberSiteHandle*> MessageSubscriber::getSiteMessageSubscriberSiteHandlePlist(void) {
    return siteMessageSubscriberSiteHandlePlist;
}

void  message_subscriber_site_handle_send_message_thread(MessageSubscriberSiteHandle& messageSubscriberSiteHandle) {
    while (true) {
        messageSubscriberSiteHandle.sendFromThread();
    }
}

MessageSubscriberSiteHandle::MessageSubscriberSiteHandle(string pIp, int pPort) {
    ip = pIp;
    port = pPort;

    sendRetryCount = 0;
    isStop = false;

    // 初始化信号量 
    sem_init(&sem, 0, 0); 

    // 创建线程
	sendMessageThreadP = new std::thread(message_subscriber_site_handle_send_message_thread, std::ref(*this));
}

string MessageSubscriberSiteHandle::getIp(void) {
    return ip;
}

int MessageSubscriberSiteHandle::getPort(void) {
    return port;
}

void MessageSubscriberSiteHandle::sendFromThread(void) {
    string message = "";

    sem_wait(&sem); 

    queue_mutex.lock();

    if (!queue.empty()) {
        message = queue.front();
        queue.pop();
    }

    queue_mutex.unlock();

    if (message == "") {
        return;
    }

    if (cli == nullptr) {
        cli = new Client(ip, port);
        cli->set_keep_alive(true);
        cli->set_connection_timeout(1, 0);
    }

    // Client cli(ip, port);

    if (!cli->is_socket_open()) {
        cli->stop();
        delete cli;
        cli = nullptr;

        cli = new Client(ip, port);
        cli->set_keep_alive(true);
        cli->set_connection_timeout(1, 0);
    }

    auto res = cli->Post("/", message, "text/plain");
    if (!res) {
        printf("client connect error. %s %d\n", ip.c_str(), port);

        ++sendRetryCount;
    }
    else {
        if (res->status != 200) {
            printf("http status = %d, error.\n", res->status);

            ++sendRetryCount;
        }
    }

    if (sendRetryCount >= MAX_SEND_RETRY) {
        printf("sendRetryCount = %d, error.\n", sendRetryCount);
        sendRetryCount = 0;

        // isStop = true;
    }
}

void MessageSubscriberSiteHandle::sendMessage(string message) {
    if (isStop) {
        return;
    }
    
    queue_mutex.lock();

    if (queue.size() > MAX_QUEUE_SIZE) {
        printf("queue is full.\n");

        // 队列满， 清空
        while(!queue.empty()) {
            queue.pop();
        }
    }
    queue.push(message);

    queue_mutex.unlock();

    sem_post(&sem); 
}

void MessageSubscriberSiteHandle::setIsStop(bool pIsStop) {
    sendRetryCount = 0;
    isStop = pIsStop;
}

int MessageSubscriberSiteHandle::getSendRetryCount(void) {
    return sendRetryCount;
}

bool MessageSubscriberSiteHandle::getIsStop(void) {
    return isStop;
}

void servicesite::ServiceSiteManager::saveMessageSubscriber(void) {
    json message_subscriber_list = json::array();

    for (auto& item : messageSubscriberList) {
        json item_json = {
            {"messageId", item.getMessageId()},
            {"site_handle_list", json::array()}
        };

        for (const auto& sub_item : item.getSiteMessageSubscriberSiteHandlePlist()) {
            json sub_item_json = {
                {"ip", sub_item->getIp()},
                {"port", sub_item->getPort()},
                {"sendRetryCount", sub_item->getSendRetryCount()},
                {"isStop", sub_item->getIsStop()},
            };

            item_json["site_handle_list"].push_back(sub_item_json);
        }

        message_subscriber_list.push_back(item_json);
    }

    string config_filename = messageSubscriberConfigPath + siteId + MESSAGE_SUBSCRIBER_CONFIG_FILE;

    // printf("----%s\n", config_filename.c_str());
    // printf("----%s\n", message_subscriber_list.dump(4).c_str());

    if (0 != createDir(ServiceSiteManager::messageSubscriberConfigPath)) {
        printf("createDir error: %s\n", messageSubscriberConfigPath.c_str());
        return;
    }

    int fd = open(config_filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if(fd == -1)
    { 
        printf("open error: %s\n", config_filename.c_str());
        return;
    } 

    string json_data = message_subscriber_list.dump(4);
    write(fd, json_data.c_str(), json_data.size());

    close(fd);

    printf("saveMessageSubscriber ok.\n");
}

void servicesite::ServiceSiteManager::loadMessageSubscriber(void) {
    const int buf_size = 10 * 1024;
    char buf[buf_size];
    
    string config_filename = messageSubscriberConfigPath + siteId + MESSAGE_SUBSCRIBER_CONFIG_FILE;

    // printf("----%s\n", config_filename.c_str());

	int fd = open(config_filename.c_str(), O_RDONLY);
	if(fd == -1)
	{
		printf("open error: %s\n", config_filename.c_str());
		return;
	}

	int read_len = read(fd, buf, buf_size);
	close(fd);

    if (read_len == buf_size) {
        printf("read_len error: %d\n", read_len);
        return;
    }

    buf[read_len] = 0;

    if (!json::accept(string(buf))) {
        printf("json::accept error: %s\n", buf);
        return;
    }

    json message_subscriber_list = json::parse(buf);

    // printf("----%s\n", message_subscriber_list.dump(4).c_str());
    for (json& item : message_subscriber_list) {
        if (!item["messageId"].is_string()) {
            printf("json::parse messageId error: %s\n", item.dump().c_str());
            return;
        }

        string message_id = item["messageId"];

        if (!item["site_handle_list"].is_array()) {
            printf("json::parse site_handle_list error: %s\n", item.dump().c_str());
            return;
        }

        for (json& sub_item : item["site_handle_list"]) {
            if (!sub_item["ip"].is_string()) {
                printf("json::parse ip error: %s\n", sub_item.dump().c_str());
                return;
            }
            
            string ip = sub_item["ip"];

            if (!sub_item["port"].is_number_integer()) {
                printf("json::parse ip port: %s\n", sub_item.dump().c_str());
                return;
            }

            int port = sub_item["port"];

            // printf("%s %s %d\n", messageId.c_str(), ip.c_str(), port);

            subscribeMessage(message_id, ip, port);
        }
    }
}

bool servicesite::ServiceSiteManager::subscribeMessage(string message_id, string ip, int port) {
    bool need_save = false;
    bool is_msg_id_ok = false;

    // 检查是否支持此 消息ID
    for (auto& item : messageIds) {
        if (item == message_id) {
            is_msg_id_ok = true;
            break;
        }
    }
    if (!is_msg_id_ok) {
        return false;
    }

    // 线程锁, 对象析构时解锁
    std::lock_guard<std::mutex> lockGuard(messageSubscriberList_mutex);

    MessageSubscriberSiteHandle* temp_messageSubscriberSiteHandle = NULL;
    for (auto& item : messageSubscriberSiteHandlePList) {
        if (item->getIp() == ip) {
            if (item->getPort() == port) {
                temp_messageSubscriberSiteHandle = item;
                break;
            }
        }
    }
    if (temp_messageSubscriberSiteHandle == NULL) {
        temp_messageSubscriberSiteHandle = new MessageSubscriberSiteHandle(ip, port);
        messageSubscriberSiteHandlePList.push_back(temp_messageSubscriberSiteHandle);
    }
    
    MessageSubscriber* temp_messageSubscriber = NULL;
    for (auto& item : messageSubscriberList) {
        if (message_id == item.getMessageId()) {
            temp_messageSubscriber = &item;
        }
    }
    if (temp_messageSubscriber == NULL) {
        temp_messageSubscriber = new MessageSubscriber(message_id);
        temp_messageSubscriber->addSiteMessageSubscriberSiteHandleP(temp_messageSubscriberSiteHandle);

        messageSubscriberList.push_back(*temp_messageSubscriber);
        need_save = true;
    }
    else {
        temp_messageSubscriber->addSiteMessageSubscriberSiteHandleP(temp_messageSubscriberSiteHandle);
    }

    return need_save;
}
