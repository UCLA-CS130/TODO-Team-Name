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
#include "server_options.hpp"
#include <sys/stat.h>
#include "request_handler.hpp"

namespace http {
  namespace server {
    struct server_options;
  }
}

// Given a parsed config file, return a struct containing the config info
void getServerOptions(NginxConfig config, http::server::server_options *server_options_pointer) {

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
        server_options_pointer->static_handlers[statement.at(1)] = config_line->child_block_;
      }
      // Status
      else if (statement.at(2) == "StatusHandler") {
        server_options_pointer->status_handler = statement.at(1);
      }
      // Proxy
      else if (statement.at(2) == "ProxyHandler") {
        server_options_pointer->proxy_handlers[statement.at(1)] = config_line->child_block_;
      }

      server_options_pointer->all_handlers.push_back(std::make_pair(statement.at(2), statement.at(1)));
    }

    // Default
    if (statement.size() == 2 && statement.at(0) == "default") {
      server_options_pointer->default_handler = statement.at(1);
    }
  }
}

// Parse the config file, handling error
bool parseConfig(char * config_file, http::server::server_options *server_options_pointer) {
  NginxConfigParser config_parser;
  NginxConfig config;

  if (!config_parser.Parse(config_file, &config)) {
    return false;
  }

  getServerOptions(config, server_options_pointer);
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

void printParsedConfig(http::server::server_options *server_options_pointer) {
  std::cout << "******** PARSED CONFIG ********\n";
  std::cout << "Port: " << server_options_pointer->port << "\n";

  for (unsigned int i = 0; i < server_options_pointer->echo_handlers.size(); i++) {
    std::cout << "Echo Handler " << server_options_pointer->echo_handlers.at(i) << "\n";
  }

  for (auto it = server_options_pointer->static_handlers.begin(); it != server_options_pointer->static_handlers.end(); ++it) {
    std::cout << "Static Handler "<< it->first << "\n";
  }

  for (auto it = server_options_pointer->proxy_handlers.begin(); it != server_options_pointer->proxy_handlers.end(); ++it) {
    std::cout << "Proxy Handler "<< it->first << "\n";
  }

  std::cout << "Default Handler " << server_options_pointer->default_handler << "\n";
  std::cout << "Status Handler " << server_options_pointer->status_handler << "\n";
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
    if (parseConfig(argv[1], &server_options_)) {

      // for debugging
      printParsedConfig(&server_options_);

      http::server::Server s("0.0.0.0", &server_options_);
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
