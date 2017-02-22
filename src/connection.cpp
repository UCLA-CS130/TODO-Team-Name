//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"

namespace http {
namespace server {

connection::connection(boost::asio::io_service& io_service,
    connection_manager& manager,
    std::map<std::string, RequestHandler*> handlers,
    RequestHandler* default_handler)
  : socket_(io_service),
    connection_manager_(manager),
    handlers_(handlers),
    default_handler_(default_handler)
{
}

boost::asio::ip::tcp::socket& connection::socket() {
  return socket_;
}

void connection::start() {
  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&connection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void connection::stop() {
  socket_.close();
}

void connection::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred) {
  if (!e) {

    request_ = Request::Parse(buffer_.data());

    // TODO: determine the correct handler to send the request to
    // Do error handling on the result of the HandleRequest() call
    // (this is just a placeholder to use an echo handler)
    RequestHandler* echo = new EchoHandler();
    response_ = new Response();
    echo->HandleRequest(*request_, response_);

    // Write response to socket
    // TODO: this is unneccessarily complex but just trying to make things work right now
    std::vector<boost::asio::const_buffer> buffers;
    std::string response_string = response_->ToString();
    buffers.push_back(boost::asio::buffer(response_string));
    boost::asio::async_write(socket_, buffers,
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));

  }

  else if (e != boost::asio::error::operation_aborted) {
    connection_manager_.stop(shared_from_this());
  }
}

void connection::handle_write(const boost::system::error_code& e) {
  if (!e) {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }

  if (e != boost::asio::error::operation_aborted) {
    connection_manager_.stop(shared_from_this());
  }
}

} // namespace server
} // namespace http