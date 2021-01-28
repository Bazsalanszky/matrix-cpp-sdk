#ifndef MATRIX_CPP_SDK_CLIENT_H
#define MATRIX_CPP_SDK_CLIENT_H
#include <string>

#include "Homeserver.h"
#include "WebAPI.h"

namespace Matrix {
    class Client{
        Homeserver* homeserver;
        WebAPI* webapi;
        std::string token;
        std::string user_id;
        bool valid = false;
    public:
        Client(Homeserver *homeServer, const std::string &token);
        Client(Homeserver *homeServer, const std::string &name, const std::string &password);
        const std::string &getDisplayName() const;
        void setDisplayName(const std::string &displayName);
        bool isValid() const;
        const std::string &getUserId() const;
        Json::Value sync() const;
        std::vector<std::string> getInvites();
        void acceptInvites();
        WebAPI* getWebAPI();
        const std::string &getToken() const;
    };
};

#endif //MATRIX_CPP_SDK_CLIENT_H
