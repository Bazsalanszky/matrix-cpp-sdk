#include "Client.h"
#include "Logger.h"
#include <iostream>
#include <vector>

Matrix::Client::Client(Homeserver *homeServer, const std::string &token) : token(token),
                                                                           homeserver(homeServer),
                                                                           webapi(homeServer->getWebAPI()) {
    Json::Value response = webapi->get("/_matrix/client/r0/account/whoami?access_token="+token );
    if(response["user_id"].isString()){
        user_id = response["user_id"].asString();
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
        valid = true;
        token = response["access_token"].asString();
        user_id = response["user_id"].asString();
    }
}

bool Matrix::Client::isValid() const {
    return valid;
}

const std::string &Matrix::Client::getUserId() const {
    return user_id;
}

const std::string &Matrix::Client::getDisplayName() const {
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

Json::Value Matrix::Client::sync() const{
    return webapi->get("/_matrix/client/r0/sync?access_token="+token);
}

std::vector<std::string> Matrix::Client::getInvites() {
    std::vector<std::string> result;
    Json::Value s = sync();
    Json::Value invite = s["rooms"]["invite"];
    for (int i = 0; i < invite.size(); ++i) {
        result.push_back(invite.getMemberNames()[i]);
    }
    return result;
}

void Matrix::Client::acceptInvites() {
    std::vector<std::string> invites = getInvites();
    for (int i = 0; i < invites.size(); ++i) {
        Json::Value res = webapi->post("/_matrix/client/r0/rooms/"+invites[i]+"/join?access_token="+token,Json::Value(Json::objectValue));
    }
}

const std::string &Matrix::Client::getToken() const {
    return token;
}

WebAPI *Matrix::Client::getWebAPI() {
    return webapi;
}

void Matrix::Client::syncRooms() {
    fetchRooms();
    for(auto it = rooms.begin();it != rooms.end();it++){
        it->second.sync();
    }
}

Matrix::Client::iterator Matrix::Client::begin() {
    return rooms.begin();
}

Matrix::Client::iterator Matrix::Client::end() {
    return rooms.end();
}

void Matrix::Client::fetchRooms() {
    Json::Value sync_response = webapi->get("/_matrix/client/r0/sync?access_token="+token);
    Json::Value joined = sync_response["rooms"]["join"];
    for (int i = 0; i < joined.size(); ++i) {
        if(rooms.find(joined.getMemberNames()[i]) == rooms.end()){
            rooms[joined.getMemberNames()[i]] = Room(joined.getMemberNames()[i],this);
            logger.info("Added room "+ joined.getMemberNames()[i]);
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

