#include "WebAPI.h"



WebAPI::WebAPI(const std::string &baseUrl) : baseURL(baseUrl) {}

Json::Value WebAPI::get(const std::string &path) const {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        std::string s;
        setDefaultCURLparams(curl,baseURL+path);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return stringToJson(s);
    }
    return Json::Value();
}

Json::Value WebAPI::post(const std::string &path, Json::Value body) const {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl){
        std::string s;
        Json::FastWriter fastWriter;
        std::string body_string = fastWriter.write(body);
        setDefaultCURLparams(curl,baseURL+path);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body_string.length());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_string.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        return stringToJson(s);
    }
    return Json::Value();
}

Json::Value WebAPI::put(const std::string &path, Json::Value body) const{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl){
        std::string s;
        Json::FastWriter fastWriter;
        std::string body_string = fastWriter.write(body);
        setDefaultCURLparams(curl,baseURL+path);

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_string.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        return stringToJson(s);
    }
    return Json::Value();
}

void WebAPI::setDefaultCURLparams(CURL * curl,std::string URL) const{
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
}

Json::Value WebAPI::stringToJson(std::string s) {
    Json::Value root;
    JSONCPP_STRING err;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(s.c_str(), s.c_str() + s.length(), &root,
                       &err)) {
        throw "Ivalid JSON!";
    }
    return root;
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s)
{
    s->append(static_cast<char *>(ptr), size*nmemb);
    return size*nmemb;
}
