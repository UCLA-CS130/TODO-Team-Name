//
// response.hpp
// ~~~~~~~~~
//
// Represents an HTTP response.
//
// Usage:
//   Response r;
//   r.SetStatus(RESPONSE_200);
//   r.SetBody(...);
//   return r.ToString();
//
// Constructed by the RequestHandler, after which the server should call ToString
// to serialize.

#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace http {
namespace server {

class Response {
 public:
  enum ResponseCode {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500
  };
  
  void SetStatus(const ResponseCode response_code);
  void AddHeader(const std::string& header_name, const std::string& header_value);
  void SetBody(const std::string& body);

  std::string ToString();

private:
  ResponseCode status_;
  std::vector<std::pair<std::string, std::string>> headers_;
  std::string body_;
};

} // namespace server
} // namespace http

#endif // HTTP_RESPONSE_HPP