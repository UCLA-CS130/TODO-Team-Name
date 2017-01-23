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

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 2)
    {
      std::cerr << "Usage: ./webserver <config_file>\n";
      return 1;
    }

    // Parse the config file.
    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(argv[1], &config);

    std::string address = "0.0.0.0";
    std::string port = "80"; // default port if nothing is set in config file
    std::string docroot = ".";

    // Look for a server block.
    for (int i = 0; i < config.statements_.size(); i++)
    {
      std::shared_ptr<NginxConfigStatement> statement = config.statements_.at(i);
      // Look for a listen statement.
      for (int j = 0; j < statement->tokens_.size(); j++)
      {
        if (statement->tokens_.at(j) == "server")
        {
          std::vector<std::shared_ptr<NginxConfigStatement>> child = statement->child_block_->statements_;
          if (child.size() == 1 &&
            child.at(0)->tokens_.size() == 2 &&
            child.at(0)->tokens_.at(0) == "listen")
          {
            port = child.at(0)->tokens_.at(1);
            std::cout << "***Listening at port " << port << "***\n";
          }
        }
      }
    }

    // Initialise the server.
    http::server::server s(address, port, docroot);

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}