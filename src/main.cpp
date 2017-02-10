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

namespace http{
  namespace server{
    struct server_options;
  }
}

// Given a parsed config file, return a struct containing the config info
void get_server_options(NginxConfig config, http::server::server_options *server_options_pointer) {

  server_options_pointer->port = "";
  server_options_pointer->echo_handler = "";
  //server_options_pointer->static_files_map;
  //variables to be used when inserting into map.
  std::string m_path;
  std::string m_root;

  for (unsigned int i = 0; i < config.statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> statement = config.statements_.at(i);

    // Look for a server block.
    if (statement->tokens_.size() == 1 && statement->tokens_.at(0) == "server") {
      std::vector<std::shared_ptr<NginxConfigStatement>> server_statements = statement->child_block_->statements_;

      // Iterate over statements in server block, setting all config options.
      for (unsigned int j = 0; j < server_statements.size(); j++) {
        std::shared_ptr<NginxConfigStatement> server_block_line = server_statements.at(j);
        std::vector<std::basic_string<char> > server_block_line_tokens = server_block_line->tokens_;

        // Port
        if (server_block_line_tokens.size() == 2 && server_block_line_tokens.at(0) == "listen") {
          server_options_pointer->port = server_block_line_tokens.at(1);
        }

        // Specified paths for whether to return an echo statement or static file
        if (server_block_line_tokens.size() == 3 && server_block_line_tokens.at(0) == "path") {

          if (server_block_line_tokens.at(2) == "EchoHandler") {
            server_options_pointer->echo_handler = server_block_line_tokens.at(1);
          }
          else if (server_block_line_tokens.at(2) == "StaticFileHandler") {
            m_path = server_block_line_tokens.at(1);
            // Child block specifies location from which to serve static files
            std::vector<std::shared_ptr<NginxConfigStatement>> static_handler_statements = server_block_line->child_block_->statements_;
            if (static_handler_statements.size() == 1 && static_handler_statements.at(0)->tokens_.size() == 2
              && static_handler_statements.at(0)->tokens_.at(0) == "root") {
              m_root = static_handler_statements.at(0)->tokens_.at(1);
            }
            // Assign m_ variables to map
            server_options_pointer->static_files_map[m_path] = m_root;
          }
        }

      }
    }
    return; // return when found server block
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

  //TODO: Possibly support different static handlers
  
  // NOTE: Boost filesystem is awful and from the stone age.
  // If you want to try this code, compile using -lboost_filesystem and #include <boost/filesystem.hpp>.
  // It doesnt work however, because boost uses c++98 strings instead of c++11 strings (I think)
  // http://stackoverflow.com/questions/38807501/boostfilesystemexists-segfault-on-linux

  // //loop through map of server options and make sure the root's exist
  // for(auto it = server_options_pointer->static_files_map.begin(); it != server_options_pointer->static_files_map.end(); ++it) {
  //   //create a boost path out of suggested root
  //   boost::filesystem::path root_dir(it->second);
  //   if(!boost::filesystem::is_directory(root_dir)){ // true, directory exists
  //     //directory does not exist
  //     std::cerr << "Invalid root directory. The directory " << it->second << " does not exist for url path " << it->first << ".\n";
  //     return false;
  //   }
  // }


  //TODO: check other folders on local system maybe? so far all must be foward of .
  //loop through map of server options and make sure the root's exist
  struct stat sb;
  std::string thePath;
  for(auto it = server_options_pointer->static_files_map.begin(); it != server_options_pointer->static_files_map.end(); ++it) {
    // Create a boost path out of suggested root
    thePath = "./" + it->second;
    if(stat(thePath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)){ // true, directory exists
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
  std::cout << "port: " << server_options_pointer->port << "\n";
  std::cout << "path to use echo handler: " << server_options_pointer->echo_handler << "\n";

  
  for(auto it = server_options_pointer->static_files_map.begin(); it != server_options_pointer->static_files_map.end(); ++it) {
    std::cout << "static handler path: "<< it->first << " accesses root directory of: " << it->second << "\n";
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
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}