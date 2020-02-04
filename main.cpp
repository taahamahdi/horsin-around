#include <iostream>
#include <json/json.h>
#include <stdexcept>

#include "horsin.h"

int main() {
    std::string origin, destination;
    std::cout << "Origin? ";
    std::getline(std::cin, origin);
    std::cout << "Destination? ";
    std::getline(std::cin, destination);
    std::string getResponse;
    try {
        getResponse = getRequest(origin, destination);
    } catch (const std::runtime_error &e) {
        throw e.what();
    } catch (const std::logic_error &e) {
        std::cerr << "GMAPS_API_KEY not set. Exiting..." << std::endl;
        throw e.what();
    }

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();

    Json::Value obj;
    std::string errors;

    bool successful =
        reader->parse(getResponse.c_str(),
                      getResponse.c_str() + getResponse.size(), &obj, &errors);
    delete reader;

    if (!successful) {
        throw;
    }

    auto formattedOrigin = obj["origin_addresses"][0].asString();
    auto formattedDestination = obj["destination_addresses"][0].asString();
    auto formattedDistance =
        obj["rows"][0]["elements"][0]["distance"]["text"].asString();
    auto etaSeconds =
        obj["rows"][0]["elements"][0]["duration"]["value"].asInt();
    auto formattedTime = getHorseETA(etaSeconds);

    if (obj["rows"][0]["elements"][0]["status"].asString() == "ZERO_RESULTS") {
        std::string errorStr = "Can't find directions from " + formattedOrigin +
                               " to " + formattedDestination + " :[";
        throw std::invalid_argument(errorStr);
    }

    std::cout << "Origin: " << formattedOrigin << std::endl;
    std::cout << "Destination: " << formattedDestination << std::endl;
    std::cout << "It will take you " << formattedTime << " to travel "
              << formattedDistance << "!" << std::endl;
}
