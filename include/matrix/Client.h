#ifndef MATRIX_CPP_SDK_CLIENT_H
#define MATRIX_CPP_SDK_CLIENT_H
#include <string>
#include <map>
#include <thread>

#include "Homeserver.h"
#include "WebAPI.h"
#include "Room.h"
#include "Logger.h"


namespace Matrix {
    class EventListener;

    class Client{
        Homeserver* homeserver;
        WebAPI* webapi;
        std::string token;
        std::string user_id;
        bool valid = false;
        std::map<std::string,Room> rooms;
        Logger logger;
        std::string last_sync_time;
        std::thread* sync_thread = nullptr;
        typedef std::map<std::string,Room>::iterator iterator;

        void fetchRooms(const Json::Value &join_json);
        std::string genTxID(size_t len=15);
        void syncRooms(const Json::Value& sync_response);
        void syncThread();
    public:
        Client(Homeserver *homeServer, const std::string &token);
        Client(Homeserver *homeServer, const std::string &name, const std::string &password);
        const std::string getDisplayName() const;
        void setDisplayName(const std::string &displayName);
        bool isValid() const;
        const std::string &getUserId() const;
        WebAPI* getWebAPI();
        const std::string &getToken() const;

        void send(const std::string &roomID, const std::string &message_type, const Json::Value &content);
        //Iterator stuff
        iterator begin();
        iterator end();

        void sync();
        void start_thread();

        void addEventListener(EventListener* eventListener);
    };
};

#endif //MATRIX_CPP_SDK_CLIENT_H
