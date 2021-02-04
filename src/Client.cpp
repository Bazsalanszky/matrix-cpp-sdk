#include "Client.h"
#include "Logger.h"
#include "EventListener.h"
#include <iostream>
#include <vector>

Matrix::Client::Client(Homeserver *homeServer, const std::string &token) : token(token),
                                                                           homeserver(homeServer),
                                                                           webapi(homeServer->getWebAPI()),
                                                                           syncer(this){
    Json::Value response = webapi->get("/_matrix/client/r0/account/whoami?access_token=" + token);
    if (response["user_id"].isString()) {
        user_id = response["user_id"].asString();
        sync();
        valid = true;
    }
}

Matrix::Client::Client(Homeserver *homeServer, const std::string &name, const std::string &password)
	: homeserver(homeServer),
	  webapi(homeServer->getWebAPI()),
	  syncer(this){
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

const std::string Matrix::Client::getDisplayName(const std::string &id) const {
    std::string user = (id == "") ? user_id : id;
    Json::Value response = webapi->get("/_matrix/client/r0/profile/" + user + "/displayname?access_token=" + token);
    if (response["displayname"].isString())
        return response["displayname"].asString();
    else return user;
}

void Matrix::Client::setDisplayName(const std::string &displayName) {
    Json::Value body;
    body["displayname"] = displayName;
    Json::Value response = webapi->put("/_matrix/client/r0/profile/" + user_id + "/displayname?access_token=" + token,
                                       body);
    if (!response.empty()) {
        logger.error("Error setting displayname. JSON: " + logger.JsonToString(response));
    }

}

const std::string &Matrix::Client::getToken() const {
    return token;
}

WebAPI *Matrix::Client::getWebAPI() {
    return webapi;
}



Json::Value
Matrix::Client::send(const std::string &roomID, const std::string &message_type, const Json::Value &content) {
    return webapi->put(
					   "/_matrix/client/r0/rooms/" + roomID + "/send/" + message_type + "/" + genTxID() + "?access_token=" +
					   token,
					   content);
}

std::string Matrix::Client::genTxID(size_t len) {
    static const char base62[] = "0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
    srand((unsigned int) time(NULL));
    std::string result;
    for (int i = 0; i < len - 1; i++) {
        result += base62[rand() % 58];
    }
    return result;
}

void Matrix::Client::addEventListener(Matrix::EventListener *eventListener) {
    syncer.addEventListener(eventListener);
}

std::string Matrix::Client::uploadFile(const char *file) {
    Json::Value response = webapi->post("/_matrix/media/r0/upload?access_token=" + token, file);
    if (response["content_uri"].isString()) {
        return response["content_uri"].asString();
    }
    return std::string();
}

void Matrix::Client::sendFile(const std::string &roomID, const std::string &file_path) {
    std::string resource = uploadFile(file_path.c_str());
    Json::Value payload;
    payload["url"] = resource;
    payload["msgtype"] = "m.image";
    payload["body"] = file_path.substr(file_path.find_last_of("/\\") + 1);
    Json::Value resp = send(roomID, "m.room.message", payload);
}

void Matrix::Client::sync() {
    syncer.sync();
}

void Matrix::Client::start() {
    syncer.start();
}

void Matrix::Client::addRoom(const Matrix::Room &room) {
    rooms_joined[room.getRoomId()] = room;
}

void Matrix::Client::addInvitedRoom(const Matrix::Room &room) {
    rooms_invited[room.getRoomId()] = room;
}

void Matrix::Client::joinRoom(const std::string &room_id) {
    Json::Value response = webapi->post("/_matrix/client/r0/join/"+room_id+"?access_token=" + token, Json::Value(Json::objectValue));
}

void Matrix::Client::setTyping(const std::string& roomID,bool typing,int timeout){
	Json::Value payload;
    payload["timeout"] = timeout;
    payload["typing"] = typing;
	Json::Value response = webapi->put("/_matrix/client/r0/rooms/"+roomID+"/typing/"+user_id+"?access_token=" + token, payload);
	logger.debug(response.toStyledString());
	if(!response.isObject() || !response.empty()) {
		logger.error(response.toStyledString());
	}
}


