#include <string>

class Request {
  public:
    explicit Request(std::string origin, std::string destination,
                     std::string key);

  private:
    std::string requestUrl{
        "https://maps.googleapis.com/maps/api/distancematrix/json?"};
    std::string origin;
    std::string destination;
    std::string mode;
    std::string key;

  public:
    std::string getUrl();
};
