//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.hpp"
#include <boost/bind.hpp>
#include <signal.h>
#include <iostream>

namespace http {
namespace server {

Server::Server(const std::string& address, const server_options* server_options_)
  : io_service_(),
    signals_(io_service_),
    acceptor_(io_service_),
    connection_manager_(),
    new_connection_(),
    server_options_(server_options_) {

  // Initialize echo request handler.
  for (unsigned int i = 0; i < server_options_->echo_handlers.size(); i++) {
    std::string uri_prefix = server_options_->echo_handlers.at(i);
    // TODO: error handling based on the value of Status
    RequestHandler* handler_ = new EchoHandler();
    NginxConfig config;
    handler_->Init(uri_prefix, config);
    handlers_[uri_prefix] = handler_;
  }

  // Initialize static handlers.
  for (auto it = server_options_->static_handlers.begin(); it != server_options_->static_handlers.end(); ++it) {
    std::string uri_prefix = it->first;
    NginxConfig* config = it->second;
    // Create handler.
    RequestHandler* handler_ = new StaticHandler();
    handler_->Init(uri_prefix, *config);
    handlers_[uri_prefix] = handler_;
  }

  // TODO: change how this works
  // Initialize default handler.
  RequestHandler* handler_ = new NotFoundHandler();
  NginxConfig config;
  handler_->Init("", config);
  default_handler_ = handler_;

  // Get the port.
  std::string port = server_options_->port;

  // Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
  signals_.add(SIGINT);
  signals_.add(SIGTERM);
#if defined(SIGQUIT)
  signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
  signals_.async_wait(boost::bind(&Server::handleStop, this));

  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::resolver::query query(address, port);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  startAccept();
}

void Server::run() {
  // The io_service::run() call will block until all asynchronous operations
  // have finished. While the server is running, there is always at least one
  // asynchronous operation outstanding: the asynchronous accept call waiting
  // for new incoming connections.
  io_service_.run();
}

void Server::startAccept() {
  new_connection_.reset(new Connection(io_service_,
        connection_manager_, handlers_, default_handler_));
  acceptor_.async_accept(new_connection_->socket(),
      boost::bind(&Server::handleAccept, this,
        boost::asio::placeholders::error));
}

void Server::handleAccept(const boost::system::error_code& e) {
  // Check whether the server was stopped by a signal before this completion
  // handler had a chance to run.
  if (!acceptor_.is_open()) {
    return;
  }

  if (!e) {
    connection_manager_.start(new_connection_);
  }

  startAccept();
}

void Server::handleStop() {
  // The server is stopped by cancelling all outstanding asynchronous
  // operations. Once all operations have finished the io_service::run() call
  // will exit.
  acceptor_.close();
  connection_manager_.stopAll();
}

} // namespace server
} // namespace http