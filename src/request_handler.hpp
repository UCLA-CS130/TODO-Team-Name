//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace http {
namespace server {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler
  : private boost::noncopyable
{
public:
  /// Handle a request and produce an echo response.
  void handle_request(const char buffer_[8192], reply& rep);

  /// The directory containing the files to be served.
  // TODO: this probably shouldn't be defined here (only request_handler_static needs it and it probably should be private
  // , but this is the only way I could get it to compile :( help please))
  std::string static_file_location_;
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_HPP