#include <chrono>
#include <curl/curl.h>
#include <stdexcept>

#include "horsin.h"
#include "request.h"

std::string getRequest(std::string origin, std::string destination) {
    Request horsey{origin, destination, std::string(getenv("GMAPS_API_KEY"))};

    CURL *curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        std::string getResponse;
        curl_easy_setopt(curl, CURLOPT_URL, horsey.getUrl().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_func);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &getResponse);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("Invalid response code");
        }
        curl_easy_cleanup(curl);
        return getResponse;
    }
    throw std::runtime_error("libcurl not initialized correctly");
}

size_t callback_func(void *ptr, size_t size, size_t count, std::string *data) {
    // Store response inside data
    data->append((char *)ptr, size * count);
    return size * count;
}

std::string getHorseETA(int seconds) {
    std::chrono::seconds secs{seconds};
    std::chrono::minutes mins{};
    std::chrono::hours hours{};
    while (std::chrono::duration_cast<std::chrono::hours>(secs) >=
           std::chrono::hours(1)) {
        hours++;
        secs -= std::chrono::hours(1);
    }
    while (std::chrono::duration_cast<std::chrono::minutes>(secs) >=
           std::chrono::minutes(1)) {
        mins++;
        secs -= std::chrono::minutes(1);
    }

    // A horse is approximately 10 times faster at galloping than a human's
    // walking speed
    hours /= 10;
    mins /= 10;
    secs /= 10;

    if (!hours.count() && !mins.count() && !secs.count()) {
        return "no time at all";
    }

    return chronoToStr(secs, mins, hours);
}

std::string chronoToStr(std::chrono::seconds s, std::chrono::minutes m,
                        std::chrono::hours h) {
    std::string seconds = std::to_string(s.count());
    std::string minutes = std::to_string(m.count());
    std::string hours = std::to_string(h.count());
    std::string formattedTime;
    if (h.count()) {
        formattedTime += hours;

        if (h.count() == 1)
            formattedTime += " hour";
        else {
            formattedTime += " hours";
        }

        if (m.count() && s.count()) {
            formattedTime += ", ";
        } else if (m.count() || s.count()) {
            formattedTime += " and ";
        }
    }
    if (m.count()) {
        formattedTime += minutes;

        if (m.count() == 1)
            formattedTime += " minute";
        else {
            formattedTime += " minutes";
        }

        if (h.count() && s.count()) {
            formattedTime += ", and ";
        } else if (s.count()) {
            formattedTime += " and ";
        }
    }
    if (s.count()) {
        formattedTime += seconds;

        if (s.count() == 1)
            formattedTime += " second";
        else {
            formattedTime += " seconds";
        }
    }
    return formattedTime;
}
