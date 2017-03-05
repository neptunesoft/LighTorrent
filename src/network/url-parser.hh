#include <string>
#include <iostream>

namespace UrlParser
{
  enum urlProtocol
  {
    Http,
    Udp
  };

  class UrlParser
  {
    public:
      UrlParser(std::string url);
      std::string getHost() const;
      urlProtocol getProtocol() const;
      int getPort() const;
      std::string getBody() const;
      std::string getBaseUrl() const;

      void dump() const;

    private:
      std::string baseUrl_;
      urlProtocol protocol_;
      std::string host_;
      int port_;
      std::string body_;
  };
}
