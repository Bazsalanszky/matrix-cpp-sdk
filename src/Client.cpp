#include "Client.h"
#include "Logger.h"
#include "EventListener.h"
#include <iostream>
#include <vector>

Matrix::Client::Client(Homeserver *homeServer, const std::string &token) : token(token),
                                                                           homeserver(homeServer),
                                                                           webapi(homeServer->getWebAPI()) {
    Json::Value response = webapi->get("/_matrix/client/r0/account/whoami?access_token="+token );
    if(response["user_id"].isString()){
        user_id = response["user_id"].asString();
        sync();
        valid = true;
    }
}

Matrix::Client::Client(Homeserver *homeServer, const std::string &name, const std::string &password)
        : homeserver(homeServer),
          webapi(homeServer->getWebAPI()) {
    Json::Value request_body;
    request_body["type"] = "m.login.password";
    request_body["user"] = name;
    request_body["password"] = password;
    Json::Value response = webapi->post("/_matrix/client/r0/login", request_body);
    if (response["access_token"].isString()) {
        token = response["access_token"].asString();
        user_id = response["user_id"].asString();
        sync();
        valid = true;
    }
}

bool Matrix::Client::isValid() const {
    return valid;
}

const std::string &Matrix::Client::getUserId() const {
    return user_id;
}

const std::string Matrix::Client::getDisplayName() const {
    Json::Value response = webapi->get("/_matrix/client/r0/profile/"+user_id+"/displayname?access_token="+token);
    if(response["displayname"].isString())
        return response["displayname"].asString();
    else return user_id;
}

void Matrix::Client::setDisplayName(const std::string &displayName) {
    Json::Value body;
    body["displayname"] = displayName;
    Json::Value response = webapi->put("/_matrix/client/r0/profile/"+user_id+"/displayname?access_token="+token,body);
    if(!response.empty()){
        logger.error("Error setting displayname. JSON: " + logger.JsonToString(response));
    }

}

void Matrix::Client::syncRooms(const Json::Value& join_json) {
    fetchRooms(join_json);
    for(auto it = rooms.begin();it != rooms.end();it++){
        for (int i = 0; i < join_json.size(); ++i) {
            if(join_json.getMemberNames()[i] == it->first) {
                it->second.sync(join_json[join_json.getMemberNames()[i]]);
                break;
            }
        }
    }
}

void Matrix::Client::sync(){
    Json::Value sync_response = (last_sync_time.empty()) ? webapi->get("/_matrix/client/r0/sync?access_token="+token) : webapi->get("/_matrix/client/r0/sync?access_token="+token +"&since="+last_sync_time);
    syncRooms(sync_response["rooms"]["join"]);
    last_sync_time = sync_response["next_batch"].asString();
}

const std::string &Matrix::Client::getToken() const {
    return token;
}

WebAPI *Matrix::Client::getWebAPI() {
    return webapi;
}

Matrix::Client::iterator Matrix::Client::begin() {
    return rooms.begin();
}

Matrix::Client::iterator Matrix::Client::end() {
    return rooms.end();
}

void Matrix::Client::fetchRooms(const Json::Value &join_json) {
    for (int i = 0; i < join_json.size(); ++i) {
        if(rooms.find(join_json.getMemberNames()[i]) == rooms.end()){
            rooms[join_json.getMemberNames()[i]] = Room(join_json.getMemberNames()[i],this);
            logger.info("Added room "+ join_json.getMemberNames()[i]);
        }
    }
}

void Matrix::Client::send(const std::string &roomID, const std::string &message_type, const Json::Value &content) {
    Json::Value sync_response = webapi->put("/_matrix/client/r0/rooms/"+roomID+"/send/"+message_type+"/"+genTxID()+"?access_token="+token,content);
}

std::string Matrix::Client::genTxID(size_t len) {
    static const char base62[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
    srand((unsigned int) time(NULL));
    std::string result;
    for(int i = 0;i<len-1;i++){
        result += base62[rand() % 58];
    }
    return result;
}

void Matrix::Client::syncThread() {
    using namespace std::chrono_literals;
    while (1){
        sync();
        std::this_thread::sleep_for(1s);
    }
}

void Matrix::Client::start_thread() {
    if(sync_thread == nullptr) {
        std::thread t(&Client::syncThread, this);
        sync_thread = &t;
        t.join();
    }else
        logger.error("Thread already running!");
}

void Matrix::Client::addEventListener(Matrix::EventListener* eventListener) {
    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        it->second.addEventListener(eventListener);
    }
}

