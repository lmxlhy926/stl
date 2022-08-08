//
// Created by WJG on 2022-4-26.
//

#ifndef SITEMANAGERSERVICE_SITEREQUESTPROCESS_H
#define SITEMANAGERSERVICE_SITEREQUESTPROCESS_H

#include <string>
#include "json/json.h"
#include "commlib/httplib.h"

int site_register(const httplib::Request &request, httplib::Response &response);
int site_query(const httplib::Request &request, httplib::Response &response);
int site_ping(const httplib::Request &request, httplib::Response &response);
int site_onoffline(const httplib::Request &request, httplib::Response &response);

#endif //SITEMANAGERSERVICE_SITEREQUESTPROCESS_H
