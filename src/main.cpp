//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"
#include "nginx-configparser/config_parser.h"

// Expected format of config file:
// server {
//   listen 8080;
//   static_file_location static;
//   request static // either 'static' or 'echo'
// }

struct server_options {
  std::string port;
  std::string static_file_location;
};

// Given a parsed config file, return a struct containing the config info
void get_server_options(NginxConfig config, server_options *server_options_pointer) {

  server_options_pointer->port = "";
  server_options_pointer->static_file_location = "";

  for (unsigned int i = 0; i < config.statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> statement = config.statements_.at(i);

    // Look for a server block.
    if (statement->tokens_.size() == 1 && statement->tokens_.at(0) == "server") {
      std::vector<std::shared_ptr<NginxConfigStatement>> server_statements = statement->child_block_->statements_;

      // Iterate over statements in server block, setting all config options.
      for (unsigned int j = 0; j < server_statements.size(); j++) {
        std::vector<std::basic_string<char> > server_option_tokens = server_statements.at(j)->tokens_;

        // port
        if (server_option_tokens.size() == 2 && server_option_tokens.at(0) == "listen") {
          server_options_pointer->port = server_option_tokens.at(1);
        }

        // directory path for files
        if (server_option_tokens.size() == 2 && server_option_tokens.at(0) == "static_file_location") {
          server_options_pointer->static_file_location = server_option_tokens.at(1);
        }

      }
    }
    return; // return when found server block
  }
}

// Parse the config file, handling error
bool parse_config(char * config_file, server_options *server_options_pointer) {
  NginxConfigParser config_parser;
  NginxConfig config;

  if (!config_parser.Parse(config_file, &config)) {
    return false;
  }

  get_server_options(config, server_options_pointer);
  std::string port = server_options_pointer->port;

  if (port == "") {
    std::cerr << "Please specify a port.\n";
    return false;
  }

  // Port number must be a valid number
  int port_as_num = std::stoi(port);
  if (port_as_num > 65535 || port_as_num < 0) {
    std::cerr << "Port must be a valid number ranging from 0 to 65535.\n";
    return false;
  }

  return true;
}


int main(int argc, char* argv[]) {
  try {

    // Check command line arguments.
    if (argc != 2) {
      std::cerr << "Usage: ./webserver <config_file>\n";
      return 1;
    }

    // Initialize the server.
    server_options server_options_;
    if (parse_config(argv[1], &server_options_)) {
      std::string port = server_options_.port;
      http::server::server s("0.0.0.0", port);
      std::cerr << "Listening at port " << port << "\n";
      // Run the server until stopped.
      s.run();
    }
    else {
      std::cerr << "ERROR: Could not parse config file.\n" <<
      "Please check that your port number is defined correctly and try again.\n";
    }

  } catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}