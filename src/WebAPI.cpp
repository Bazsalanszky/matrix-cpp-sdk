#include <sys/stat.h>
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

Json::Value WebAPI::post(const std::string &path, const Json::Value &body) const {
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

Json::Value WebAPI::put(const std::string &path, const Json::Value &body) const{
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

Json::Value WebAPI::post(const std::string &path, const char *file_path) const {
    CURL *curl;
    CURLcode res;
    struct stat file_info;
    int exist = stat(file_path, &file_info);
    if(exist != 0) {
        return "";
    }
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        return "";
    }
    curl = curl_easy_init();
    if(curl){
        std::string s;
        setDefaultCURLparams(curl,baseURL+path);


        curl_easy_setopt(curl, CURLOPT_READDATA, file);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,
                         (curl_off_t) file_info.st_size);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        return stringToJson(s);
    }
    return Json::Value();
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s)
{
    s->append(static_cast<char *>(ptr), size*nmemb);
    return size*nmemb;
}
