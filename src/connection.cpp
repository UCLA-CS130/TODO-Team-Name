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
#include <deque>
#include <string>
#include <algorithm>
#include <iostream>
#include <memory>
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
  clearBuffer();
}

void connection::clearBuffer() {
  memset( buffer_, '\0', sizeof(char)*BUF_SIZE );
}

boost::asio::ip::tcp::socket& connection::socket() {
  return socket_;
}

void connection::start() {
  socket_.async_read_some(boost::asio::buffer(buffer_, BUF_SIZE),
      boost::bind(&connection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void connection::stop() {
  socket_.close();
}

std::deque<std::string> connection::splitUri(std::string uri){
  std::deque<std::string> ret;
  std::size_t last_slash_pos;
  std::string token;
  while ((last_slash_pos = uri.find_last_of("/")) != std::string::npos) {
    token = uri.substr(last_slash_pos);
    ret.push_front(token);
    uri.erase(last_slash_pos, last_slash_pos+token.length());
  }
  return ret;
}

int connection::longestSubstring(std::deque<std::string> req, std::deque<std::string> handler){
  int ret = 0;
  int upperBound = std::min(req.size(), handler.size());

  for (int i = 0; i < upperBound; i++){
    if (req[i] == handler[i])
      ret++;
    else
      return ret;
  }
  return ret;
}

RequestHandler* connection::chooseHandler(){
  // Create a response
    response_ = new Response();

    std::deque<std::string> request_uri_tokens = splitUri(request_->uri());
    // Decide which handler to use
    RequestHandler* bestHandler = default_handler_; 
    // loop through handlers 
    int quality;
    int max = 0;
    for( auto it = handlers_.begin(); it != handlers_.end(); ++it) {
      // Get data from it
      std::string uri_prefix = it->first;
      
      std::deque<std::string> handler_uri_tokens = splitUri(uri_prefix);

      quality = longestSubstring(request_uri_tokens, handler_uri_tokens);
      if (quality > max){
        max = quality;
        bestHandler = it->second;
        std::cerr << "Best handler is: " << uri_prefix << "\n";
      }
    }
    //invoke the handler
    return bestHandler;
}

void connection::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred) {
  if (!e) {
    // Get the request
    std::string raw_req(buffer_);
    request_ = Request::Parse(raw_req);

    // Pick a handler and handle the request
    auto statCode = chooseHandler()->HandleRequest(*request_, response_);

    //TODO: different handlers for different status codes
    if(statCode){
      default_handler_->HandleRequest(*request_, response_);
      std::cerr << "Error: handler returned status code " << statCode << ".\n";
    }

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

  // Make sure buffer is clear before another read
  clearBuffer();
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