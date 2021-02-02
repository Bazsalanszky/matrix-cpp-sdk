#ifndef MATRIX_CPP_SDK_CLIENT_H
#define MATRIX_CPP_SDK_CLIENT_H
#include <string>
#include <map>
#include <vector>
#include <thread>

#include "Homeserver.h"
#include "WebAPI.h"
#include "Room.h"
#include "Logger.h"
#include "Syncer.h"


namespace Matrix {
    class EventListener;
    class Client{
        friend Syncer;
        Homeserver* homeserver;
        WebAPI* webapi;
        Syncer syncer;
        std::string token;
        std::string user_id;
        bool valid = false;
        std::map<std::string,Room> rooms_joined;
        std::map<std::string,Room> rooms_invited;
        typedef std::map<std::string,Room>::iterator iterator;
        std::vector<EventListener*> listeners;
        Logger logger;

        std::string genTxID(size_t len=15);

    public:
        Client(Homeserver *homeServer, const std::string &token);
        Client(Homeserver *homeServer, const std::string &name, const std::string &password);
        const std::string getDisplayName(const std::string &id="") const;
        void setDisplayName(const std::string &displayName);
        bool isValid() const;
        const std::string &getUserId() const;
        WebAPI* getWebAPI();
        const std::string &getToken() const;
        void joinRoom(const std::string& room_id);

        Json::Value send(const std::string &roomID, const std::string &message_type, const Json::Value &content);
        void sendFile(const std::string& roomID, const std::string &file_path);
        std::string uploadFile(const char *file);

        void addRoom(const Room& room);
        Room& getRoom(const std::string &id);
        void addInvitedRoom(const Room& room);
        Room& getInvitedRoom(const std::string &id);

        void sync();
        void start();

        void addEventListener(EventListener* eventListener);
    };
};

#endif //MATRIX_CPP_SDK_CLIENT_H
