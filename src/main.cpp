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

// Given a parsed config file, return the port number
std::string get_port(NginxConfig config) {
  // Look for a server block.
  for (unsigned int i = 0; i < config.statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> statement = config.statements_.at(i);
    // Look for a listen statement.
    for (unsigned int j = 0; j < statement->tokens_.size(); j++) {
      if (statement->tokens_.at(j) == "server") {
        std::vector<std::shared_ptr<NginxConfigStatement>> child = statement->child_block_->statements_;
        if (child.size() == 1 &&
            child.at(0)->tokens_.size() == 2 &&
            child.at(0)->tokens_.at(0) == "listen") {
          return child.at(0)->tokens_.at(1);
        }
      }
    }
  }
  // Port number not defined correctly in config file.
  return "";
}

// Parse the config file
bool parse_config(char * config_file, std::string * port) {
  NginxConfigParser config_parser;
  NginxConfig config;

  if (!config_parser.Parse(config_file, &config)) {
    return false;
  }

  *port = get_port(config);
  if (*port == "") {
    std::cerr << "Please specify a port.\n";
    return false;
  }

  // Port number must be a valid number
  int port_as_num = std::stoi(*port);
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
    std::string port = "";
    if (parse_config(argv[1], &port)) {
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