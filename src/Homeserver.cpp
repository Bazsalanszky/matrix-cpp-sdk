#include "Homeserver.h"

Matrix::Homeserver::Homeserver(const std::string &name, const std::string &baseUrl) : name(name), baseURL(baseUrl), API(baseUrl) {}

WebAPI *Matrix::Homeserver::getWebAPI() {
    return &API;
}
