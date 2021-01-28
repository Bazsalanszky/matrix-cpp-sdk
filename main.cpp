#include <iostream>
#include "src/Client.h"

int main(){
    Homeserver server(getenv("HOMESERVER_NAME"),getenv("HOMESERVER_URL"));
    Matrix::Client c(&server, getenv("ACCESS_TOKEN"));
    std::cout << c.getUserId() << std::endl;
    c.setDisplayName("Magdi Josnő");
    c.acceptInvites();
    return 0;
}
