//
// request.cpp
// ~~~~~~~~~
//

#include "request.hpp"
#include <string>

namespace http {
namespace server {

std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
  std::unique_ptr<Request> parsed_request = std::unique_ptr<Request>(new Request());

  // TODO: implement
  // just filling in the bare minimum needed for echo handler
  parsed_request->raw_request_ = raw_request;

  return parsed_request;
}

std::string Request::raw_request() const {
  return raw_request_;
}

std::string Request::method() const {
  return method_;
}

std::string Request::uri() const {
  return uri_;
}

std::string Request::version() const {
  return version_;
}

Request::Headers Request::headers() const {
  return headers_;
}

std::string Request::body() const {
  return body_;
}


} // namespace server
} // namespace http