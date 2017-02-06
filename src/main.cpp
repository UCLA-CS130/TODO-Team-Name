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
//   path /echo EchoHandler;
//   path /static StaticFileHandler {
//      root static;
//   }
// }

struct server_options {
  std::string port;
  std::string static_file_root;
  std::string echo_handler;
  std::string static_handler;
};

// Given a parsed config file, return a struct containing the config info
void get_server_options(NginxConfig config, server_options *server_options_pointer) {

  server_options_pointer->port = "";
  server_options_pointer->static_file_root = "";
  server_options_pointer->echo_handler = "";
  server_options_pointer->static_handler = "";

  for (unsigned int i = 0; i < config.statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> statement = config.statements_.at(i);

    // Look for a server block.
    if (statement->tokens_.size() == 1 && statement->tokens_.at(0) == "server") {
      std::vector<std::shared_ptr<NginxConfigStatement>> server_statements = statement->child_block_->statements_;

      // Iterate over statements in server block, setting all config options.
      for (unsigned int j = 0; j < server_statements.size(); j++) {
        std::shared_ptr<NginxConfigStatement> server_block_line = server_statements.at(j);
        std::vector<std::basic_string<char> > server_block_line_tokens = server_block_line->tokens_;

        // port
        if (server_block_line_tokens.size() == 2 && server_block_line_tokens.at(0) == "listen") {
          server_options_pointer->port = server_block_line_tokens.at(1);
        }

        // specified paths for whether to return an echo statement or static file
        if (server_block_line_tokens.size() == 3 && server_block_line_tokens.at(0) == "path") {

          if (server_block_line_tokens.at(2) == "EchoHandler") {
            server_options_pointer->echo_handler = server_block_line_tokens.at(1);
          }
          else if (server_block_line_tokens.at(2) == "StaticFileHandler") {
            server_options_pointer->static_handler = server_block_line_tokens.at(1);
            // child block specifies location from which to serve static files
            std::vector<std::shared_ptr<NginxConfigStatement>> static_handler_statements = server_block_line->child_block_->statements_;
            if (static_handler_statements.size() == 1 && static_handler_statements.at(0)->tokens_.size() == 2
              && static_handler_statements.at(0)->tokens_.at(0) == "root") {
              server_options_pointer->static_file_root = static_handler_statements.at(0)->tokens_.at(1);
            }

          }
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
  std::string static_file_root = server_options_pointer->static_file_root;

  if (port == "") {
    std::cerr << "Please specify a port.\n";
    return false;
  }

  if (static_file_root == "") {
    std::cerr << "Please specify a root directory from which to serve static files.\n";
    return false;
  }

  // Port number must be a valid number
  int port_as_num = std::stoi(port);
  if (port_as_num > 65535 || port_as_num < 0) {
    std::cerr << "Port must be a valid number ranging from 0 to 65535.\n";
    return false;
  }

  // TODO: check that static file root is an actual directory that exists?

  return true;
}

void print_parsed_config(server_options *server_options_pointer) {
  std::cout << "******** PARSED CONFIG ********\n";
  std::cout << "port: " << server_options_pointer->port << "\n";
  std::cout << "root for static files: " << server_options_pointer->static_file_root << "\n";
  std::cout << "path to use echo handler: " << server_options_pointer->echo_handler << "\n";
  std::cout << "path to use static handler: " << server_options_pointer->static_handler << "\n";
  std::cout << "*******************************" << "\n";
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

      // for debugging
      print_parsed_config(&server_options_);

      std::string port = server_options_.port;
      std::string static_file_root = server_options_.static_file_root;
      http::server::server s("0.0.0.0", port, static_file_root);
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