#include <string>
#include <chrono>

std::string getRequest(std::string origin, std::string destination);

size_t callback_func(void *ptr, size_t size, size_t count, std::string *data);

std::string getHorseETA(int seconds);

std::string chronoToStr(std::chrono::seconds s, std::chrono::minutes m, std::chrono::hours h);
