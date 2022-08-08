//
// Created by WJG on 2022-4-25.
//

#ifndef SITEMANAGERSERVICE_SITETREE_H
#define SITEMANAGERSERVICE_SITETREE_H

#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include "json/json.h"
#include "comm_define.h"

class SiteTree{
public:
    static SiteTree& GetInstance()
    {
        if ( m_pInstance == nullptr )
            m_pInstance = new SiteTree();
        return *m_pInstance;
    };

    ~SiteTree();

    static std::string config_file_dir;

    int initSiteTree(const std::string &file_dir);

    //获取所有站点信息
    Json::Value getAllSite();

    //获取指定站点信息中key对应的值
    std::string getSiteInfo(const std::string &site_id, const std::string &key);

    //获取指定站点的信息
    Json::Value getSiteAllInfo(const std::string &site_id);

    //更新站点信息
    int updateSiteInfo(const std::string &site_id, const Json::Value &info);

    //更新站点信息的一个字段
    int updateSiteInfo(const std::string &site_id, const std::string &key, const std::string &info);

    //是否包含指定站点
    bool hasSite(const std::string &site_id);

    //更新指定站点信息，并将站点编号存入已注册站点map
    int cacheSiteInfo(const std::string &site_id, const Json::Value &info);

    int getSiteport();

    std::unordered_set<std::string> getSubscribeSite(const std::string msg_id);

    //更新已注册站点计数
    int refreshSitePingCounter(const std::string &site_id, int mark);

    //获取已注册站点
    std::unordered_set<std::string> getSiteIdList();

    //移除站点信息（包括站点信息列表，已注册站点列表）
    int notifySiteOffline(const std::string &site_id);

private:
    static SiteTree* m_pInstance;

    int used_port;
    std::mutex treeMutex;
    std::mutex pingMutex;

    std::unordered_map<std::string, Json::Value> allSite;       //unordered_map<site_id，站点信息Json>
    std::unordered_set<std::string> site_id_setlist;            //unordered_set<site_id>  已注册的站点
    std::unordered_map<std::string, int> site_ping_counter;     //unordered_map<site_id, ping_counter> 已注册的站点ping计数
    // unordered_map<订阅的msg_id，unordered_set<订阅此消息的site_id>>
    std::unordered_map<std::string, std::unordered_set<std::string>> subscribe_site_map;

    SiteTree();

    void setInfo(Json::Value &sobj, const Json::Value &value);
    void setInfo(Json::Value &sobj, const std::string &key, const std::string &value);
    std::string getInfo(const Json::Value &sobj, const std::string &key);
    void saveSiteInfo(std::string site_id);
    int deleteSite(const std::string &site_id);
};


#endif //SITEMANAGERSERVICE_SITETREE_H
