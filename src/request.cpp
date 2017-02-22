//
// request.cpp
// ~~~~~~~~~
//

#include "request.hpp"
#include "request_parser.hpp"
#include <string>
#include <memory>

namespace http {
namespace server {

std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
  //std::unique_ptr<Request> parsed_request = std::unique_ptr<Request>(new Request());

  // TODO: implement
  // just filling in the bare minimum needed for echo handler
  //parsed_request->raw_request_ = raw_request;

  /// The parser for the incoming request.
  request_parser request_parser_;
  Request *parsed_request = new Request();
  boost::tribool result;
  boost::tie(result, boost::tuples::ignore) = request_parser_.parse(*parsed_request, raw_request.c_str(), raw_request.c_str() + raw_request.length());

  //TODO: use result in some useful way

  //Get useful information
  parsed_request->raw_request_ = raw_request;
  parsed_request->method_ = request_parser_.getMethod();
  parsed_request->uri_ = request_parser_.getUri();
  parsed_request->version_ = request_parser_.getVersion();
  parsed_request->headers_ = request_parser_.getHeaders();

  // Unique pointers are fun
  std::unique_ptr<Request> pointer_to_req(parsed_request);

  //Return a pointer, not object
  return pointer_to_req;
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