//
// request_handler_static.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_STATIC_HPP
#define HTTP_REQUEST_HANDLER_STATIC_HPP

#include "request_handler.hpp"

namespace http {
namespace server {

struct reply;
struct request;

/// Handler for atatic file requests.
class request_handler_static : public http::server::request_handler {
public:
  request_handler_static(const std::string& static_file_location);
  /// Handle a request and produce an echo response.
  void handle_request(const request& req, reply& rep) override;

private:
  /// Perform URL-decoding on a string. Returns false if the encoding is invalid.
  static bool url_decode(const std::string& in, std::string& out);
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_STATIC_HPP