//
// server_options.hpp
// 

#ifndef SERVER_OPTIONS_HPP
#define SERVER_OPTIONS_HPP

#include <string>
#include <map>
#include <vector>

namespace http {
namespace server {

// Holds the parsed config server options
struct server_options {
  std::string port;

  // Vector of echo handlers (each string represents the URL path)
  std::vector<std::string> echo_handlers;

  // Map of static handlers (key is the path, value is the
  // NginxConfig child block containing the root info)
  std::map<std::string, NginxConfig*> static_handlers;

  // Status Handler
  std::string status_handler;
  std::vector<std::pair<std::string, std::string>> all_handlers;

  // Default response handler
  std::string default_handler;
};

} // namespace server
} // namespace http

#endif // SERVER_OPTIONS_HPP