#include "Client.h"
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
    if(response.empty()){
        std::cout << "Displayname set!" << std::endl;
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
