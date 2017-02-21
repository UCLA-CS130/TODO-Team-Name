//
// request_handler_static.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include "request_handler_static.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "request_parser.hpp"

namespace http {
namespace server {

request_handler_static::request_handler_static(const server_options* server_options_) 
  : server_options_(server_options_){}

// Serve the static file that is requested
void request_handler_static::handle_request(const request& req, reply& rep) {
  // root on localhost containing desired files
  std::string static_file_root_;

  // Decode url to path.
  std::string request_string;
  if (!url_decode(req.uri, request_string)) {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // gets the desired path
  std::string request_path = request_string.substr(0, request_string.find('/', 1));
  //first part of url is the path, as specified in config. Check map TODO:
  if ( server_options_->static_handlers.find(request_path) == server_options_->static_handlers.end() ) {
    // not found
    std::cout << "PATH NOT FOUND; " << request_path << "\n"; //TODO: legit error handling
  } else {
    // found
    static_file_root_ = server_options_->static_handlers.at(request_path);
  }
  //this string contains the actual path to the file.
  std::string request_file = request_string.substr(request_string.find(request_path)+request_path.length());

  // Request path must be absolute and not contain "..".
  if (request_file.empty() || request_file[0] != '/'
      || request_file.find("..") != std::string::npos) {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_file[request_file.size() - 1] == '/') {
    request_file += "/index.html";
  }

  // Determine the file extension.
  std::size_t last_slash_pos = request_file.find_last_of("/");
  std::size_t last_dot_pos = request_file.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
    extension = request_file.substr(last_dot_pos + 1);
  }

  // Open the file to send back.
  std::string full_path = static_file_root_ + request_file;
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is) {
    rep = reply::stock_reply(reply::not_found);
    return;
  }

  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
    rep.content.append(buf, is.gcount());
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = mime_types::extension_to_type(extension);
}

bool request_handler_static::url_decode(const std::string& in, std::string& out) {
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (in[i] == '%') {
      if (i + 3 <= in.size()) {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value) {
          out += static_cast<char>(value);
          i += 2;
        }
        else {
          return false;
        }
      }
      else {
        return false;
      }
    }
    else if (in[i] == '+') {
      out += ' ';
    }
    else {
      out += in[i];
    }
  }
  return true;
}

} // namespace server
} // namespace http
