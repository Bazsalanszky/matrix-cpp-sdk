#ifndef MATRIX_CPP_SDK_HOMESERVER_H
#define MATRIX_CPP_SDK_HOMESERVER_H
#include <string>
#include "WebAPI.h"

namespace Matrix {
    class Homeserver {
        std::string name;
        std::string baseURL;
        WebAPI API;
    public:
        Homeserver(const std::string &name, const std::string &baseUrl);

        WebAPI *getWebAPI();
    };
}

#endif //MATRIX_CPP_SDK_HOMESERVER_H