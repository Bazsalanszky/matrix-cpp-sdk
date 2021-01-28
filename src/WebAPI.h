#ifndef WEBAPI_H
#define WEBAPI_H
#include <string>
#include <curl/curl.h>
#include <json/json.h>

class WebAPI {
    std::string baseURL;
    void setDefaultCURLparams(CURL* curl,std::string URL) const;
    static Json::Value stringToJson(std::string string);
 public:
    WebAPI(const std::string &baseUrl);
    Json::Value get(const std::string& path) const;
    Json::Value post(const std::string& path, Json::Value body) const;
    Json::Value put(const std::string& path, Json::Value body) const;
};

static size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s);

#endif //TRADESIM_WEBAPI_H
