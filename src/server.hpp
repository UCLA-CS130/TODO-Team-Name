//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <boost/noncopyable.hpp>
#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler_echo.hpp"
#include "request_handler_static.hpp"
#include "request_handler_notfound.hpp"
#include "server_options.hpp"

namespace http {
namespace server {

struct server_options;

/// The top-level class of the HTTP server.
class Server
  : private boost::noncopyable
{
public:
  /// Construct the server to listen on the specified TCP address and port
  /// Serve static files from the directory specified by static_file_root
  explicit Server(const std::string& address, const server_options* server_options_);

  // see if server is valid
  bool isValid(const std::string& address, const std::string& port);

  /// Run the server's io_service loop.
  void run();

private:
  /// Initiate an asynchronous accept operation.
  void startAccept();

  /// Handle completion of an asynchronous accept operation.
  void handleAccept(const boost::system::error_code& e);

  /// Handle a request to stop the server.
  void handleStop();

  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  /// The signal_set is used to register for process termination notifications.
  boost::asio::signal_set signals_;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The connection manager which owns all live connections.
  ConnectionManager connection_manager_;

  /// The next connection to be accepted.
  connection_ptr new_connection_;

  /// Map of URL paths to Request Handlers.
  std::map<std::string, RequestHandler*> handlers_;

  /// The status handler 
  RequestHandler* status_handler_;

  /// The default request handler if no URL path matches.
  RequestHandler* default_handler_;

  //pointer to server_options
  const server_options* server_options_;
};

} // namespace server
} // namespace http

#endif // HTTP_SERVER_HPP