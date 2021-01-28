#ifndef MATRIX_CPP_SDK_CLIENT_H
#define MATRIX_CPP_SDK_CLIENT_H
#include <string>
#include <map>

#include "Homeserver.h"
#include "WebAPI.h"
#include "Room.h"
#include "Logger.h"

namespace Matrix {
    class Client{
        Homeserver* homeserver;
        WebAPI* webapi;
        std::string token;
        std::string user_id;
        bool valid = false;
        std::map<std::string,Room> rooms;
        Logger logger;
        typedef std::map<std::string,Room>::iterator iterator;
        void fetchRooms();
        std::string genTxID(size_t len=15);
    public:
        Client(Homeserver *homeServer, const std::string &token);
        Client(Homeserver *homeServer, const std::string &name, const std::string &password);
        const std::string &getDisplayName() const;
        void setDisplayName(const std::string &displayName);
        bool isValid() const;
        const std::string &getUserId() const;
        WebAPI* getWebAPI();
        const std::string &getToken() const;
        void syncRooms();
        void send(const std::string &roomID, const std::string &message_type, const Json::Value &content);
        //Iterator stuff
        iterator begin();
        iterator end();
        //TODO: Rewrite these
        Json::Value sync() const;
        std::vector<std::string> getInvites();
        void acceptInvites();
    };
};

#endif //MATRIX_CPP_SDK_CLIENT_H
