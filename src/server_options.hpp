//
// request.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERVER_OPTIONS_HPP
#define SERVER_OPTIONS_HPP

#include <string>
#include <map>

namespace http {
namespace server {

// Holds the parsed config server options
struct server_options {
  std::string port;
  std::string echo_handler_path;

  // Key of the map is the path in url, value is the root on localhost
  std::map<std::string, std::string> static_files_map;
};

} // namespace server
} // namespace http

#endif // SERVER_OPTIONS_HPP