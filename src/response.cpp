//
// response.cpp
// ~~~~~~~~~
//

#include "response.hpp"
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace http {
namespace server {

void Response::SetStatus(const ResponseCode response_code) {
  status_ = response_code;
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value) {
  headers_.push_back(std::make_pair(header_name, header_value));
}

void Response::SetBody(const std::string& body) {
  body_ = body;
}

std::string Response::ToString() {
  std::string response_string = "";
  const std::string carriage_return = "\r\n";

  // Response code
  switch(status_) {
    case OK:
      response_string += "HTTP/1.1 200 OK" + carriage_return;
      break;
    case BAD_REQUEST:
      response_string += "HTTP/1.1 400 Bad Request" + carriage_return;
      break;
    case NOT_FOUND:
      response_string += "HTTP/1.1 404 Not Found" + carriage_return;
      break;
    default:
      response_string += "HTTP/1.1 500 Internal Server Error" + carriage_return;
      break;
  }

  // HTTP headers
  for (auto it = headers_.begin(); it != headers_.end(); it++) {
    response_string += it->first + ": " + it->second + carriage_return;
  }
  response_string += carriage_return;

  // Response content
  response_string += body_;

  return response_string;
}


} // namespace server
} // namespace http