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

// Parse the config file and return the port number
std::string get_port(char * config_file) {
  NginxConfigParser config_parser;
  NginxConfig config;
  config_parser.Parse(config_file, &config);
  std::string port;

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
          port = child.at(0)->tokens_.at(1);
          std::cout << "Listening at port " << port << "\n";
          return port;
        }
      }
    }
  }

  return "80"; // default port if nothing is specified in config
}


int main(int argc, char* argv[]) {
  try {

    // Constants / default configs
    const std::string usage = "Usage: ./webserver <config_file>\n";
    const std::string address = "0.0.0.0";

    // Check command line arguments.
    if (argc != 2) {
      std::cerr << usage;
      return 1;
    }

    // Initialise the server.
    http::server::server s(address, get_port(argv[1]));

    // Run the server until stopped.
    s.run();
  } catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}