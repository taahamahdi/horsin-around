#include "request.h"
#include "curl/curl.h"
#include <sstream>

Request::Request(std::string origin, std::string destination, std::string key)
    : origin{origin}, destination{destination}, mode{"walking"}, key{key} {
    CURL *curl = curl_easy_init();
    if (curl) {
        auto originEscaped = curl_easy_escape(curl, origin.c_str(), 0);
        auto destinationEscaped = curl_easy_escape(curl, destination.c_str(), 0);
        if (originEscaped) {
            this->origin = std::string(originEscaped);
            curl_free(originEscaped);
        }
        if (destinationEscaped) {
            this->destination = std::string(destinationEscaped);
            curl_free(destinationEscaped);
        }
        curl_easy_cleanup(curl);
    }
}

std::string Request::getUrl() {
    std::stringstream ss;

    std::string originsKey = "origins=";
    std::string destinationsKey = "destinations=";
    std::string modeKey = "mode=";
    std::string apiKey = "key=";

    char delim = '&';
    ss << requestUrl << originsKey << origin << delim << destinationsKey
       << destination << delim << modeKey << mode << delim << apiKey << key;
    return ss.str();
}
