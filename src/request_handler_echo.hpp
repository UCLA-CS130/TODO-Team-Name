//
// request_handler_echo.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "request_handler.hpp"

namespace http {
namespace server {

struct reply;
struct request;

/// Handler for echo requests.
class request_handler_echo : public http::server::request_handler {
public:
  /// Handle a request and produce an echo response.
  void handle_request(const request& req, reply& rep) override;
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_HPP