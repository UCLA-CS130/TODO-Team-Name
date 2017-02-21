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
#include <map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"
#include "nginx-configparser/config_parser.h"
#include "server_options.hpp"
#include <sys/stat.h>

namespace http {
  namespace server {
    struct server_options;
  }
}

// Given a parsed config file, return a struct containing the config info
void get_server_options(NginxConfig config, http::server::server_options *server_options_pointer) {

  server_options_pointer->port = "";
  std::string m_path;

  // Iterate over statements in parsed config.
  for (unsigned int i = 0; i < config.statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> config_line = config.statements_.at(i);
    std::vector<std::string> statement = config_line->tokens_;


    // Port
    if (statement.size() == 2 && statement.at(0) == "port") {
      server_options_pointer->port = statement.at(1);
    }

    // Handlers
    if (statement.size() == 3 && statement.at(0) == "path") {

      // Echo
      if (statement.at(2) == "EchoHandler") {
        server_options_pointer->echo_handlers.push_back(statement.at(1));
      }

      // Static
      else if (statement.at(2) == "StaticHandler") {
        std::vector<std::shared_ptr<NginxConfigStatement>> child_statements = config_line->child_block_->statements_;
        if (child_statements.size() > 0) {
          std::shared_ptr<NginxConfigStatement> child_statement_line = child_statements.at(0);
          std::vector<std::string> child_statement = child_statement_line->tokens_;
          if (child_statement.size() == 2 && child_statement.at(0) == "root") {
            server_options_pointer->static_handlers[statement.at(1)] = child_statement.at(1);
          }
        }
      }
    }

    // Default
    if (statement.size() == 2 && statement.at(0) == "default") {
      server_options_pointer->default_handler = statement.at(1);
    }
  }
}

// Parse the config file, handling error
bool parse_config(char * config_file, http::server::server_options *server_options_pointer) {
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

  // Loop through map of server options and make sure the roots exist
  struct stat sb;
  std::string try_path;
  std::map<std::string, std::string> static_handlers = server_options_pointer->static_handlers;


  for (auto it = static_handlers.begin(); it != static_handlers.end(); ++it) {
    // Create a boost path out of suggested root
    try_path = "./" + it->second;
    if(stat(try_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)){ // true, directory exists
      // Directory exists
    }
    else{
      std::cerr << "ERROR: Invalid root directory. The directory " << it->second << " does not exist for url path " << it->first << ".\n";
      return false;
    }
  }

  return true;
}

void print_parsed_config(http::server::server_options *server_options_pointer) {
  std::cout << "******** PARSED CONFIG ********\n";
  std::string port = server_options_pointer->port;
  std::vector<std::string> echo_handlers = server_options_pointer->echo_handlers;
  std::map<std::string, std::string> static_handlers = server_options_pointer->static_handlers;

  std::cout << "Port: " << port << "\n";

  for (unsigned int i = 0; i < echo_handlers.size(); i++) {
    std::cout << "Echo Handler " << echo_handlers.at(i) << "\n";
  }

  for (auto it = static_handlers.begin(); it != static_handlers.end(); ++it) {
    std::cout << "Static Handler "<< it->first << " accesses root " << it->second << "\n";
  }

  std::cout << "*******************************" << "\n";
}

int main(int argc, char* argv[]) {
  http::server::server_options server_options_;
  try {

    // Check command line arguments.
    if (argc != 2) {
      std::cerr << "Usage: ./webserver <config_file>\n";
      return 1;
    }

    // Initialize the server.
    if (parse_config(argv[1], &server_options_)) {

      // for debugging
      print_parsed_config(&server_options_);

      http::server::server s("0.0.0.0", &server_options_);
      std::cerr << "Listening at port " << server_options_.port << "\n";
      // Run the server until stopped.
      s.run();
    }
    else {
      std::cerr << "ERROR: Could not parse config file.\n";
    }

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}