//
// http_client.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "http_client.hpp"
#include "response.hpp"
#include "request.hpp"

namespace http {
namespace server {

bool HttpClient::EstablishConnection(const std::string& host, const std::string& service) {
  boost::asio::io_service io_service;

  // Get a list of endpoints corresponding to the server name.
  tcp::resolver resolver(io_service);
  tcp::resolver::query query(host, service);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query), end;

  // Try each endpoint until we successfully establish a connection.
  boost::system::error_code error;
  socket_ = new boost::asio::ip::tcp::socket(io_service);

  boost::asio::connect(*socket_, endpoint_iterator, end, error);

  if(error) {
    //error_code means binding the socket failed
    std::cerr << "Unable to bind socket to website " + host + " for " + service << std::endl;
    return false;
  }

  return true;
}

Response* HttpClient::SendRequest(const Request& req) {
  // Form the request from the req string
  boost::asio::streambuf request;
  std::ostream request_stream(&request);
  request_stream << req.ToString();

  // Send the request.
  boost::asio::write(*socket_, request);

  // Read the response back and store in Response object
  boost::asio::streambuf raw_response_stream;
  boost::system::error_code error;

  std::string raw_response_string;
  std::size_t bytes_read;
  while((bytes_read = boost::asio::read(*socket_, raw_response_stream, boost::asio::transfer_at_least(1), error))) {
    // Read the values from the buffer (all bytes read)
    std::string read_data = std::string(boost::asio::buffers_begin(raw_response_stream.data()),
                      boost::asio::buffers_begin(raw_response_stream.data()) + bytes_read);
    std::cout << read_data;
    raw_response_string += read_data;
    // Remove the bytes read from the buffer
    raw_response_stream.consume(bytes_read);
  }


  if(error != boost::asio::error::eof) {
    // Error reading.
    std::cerr << "Reading failed.";
    return nullptr;
  }

  auto resp = Response::Parse(raw_response_string);
  return resp;

  /*
  // Read the response status line. The response streambuf will automatically
  // grow to accommodate the entire line. The growth may be limited by passing
  // a maximum size to the streambuf constructor.
  Response* response = new Response();
  boost::asio::streambuf response_buffer;
  boost::asio::read_until(*socket_, response_buffer, "\r\n");

  // Check that response is OK.
  std::istream response_stream(&response_buffer);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
  {
    std::cerr << "Invalid response" << std::endl;
    return nullptr;
  }
  if (status_code != 200)
  {
    std::cerr << "Response returned with status code " << status_code << std::endl;;
    return nullptr;
  }

  response->SetStatus(Response::OK);

  // Read the response headers, which are terminated by a blank line.
  boost::asio::read_until(*socket_, response_buffer, "\r\n\r\n");

  // Process the response headers.
  std::string header;
  while (std::getline(response_stream, header) && header != "\r") {
    size_t index = header.find(":");
    std::string first = header.substr(0, index);
    std::string second = header.substr(index + 2);
    response->AddHeader(first, second);
  }

  // Consume whatever content we already have.
  if (response_buffer.size() > 0)
      response_buffer.consume(response_buffer.size());

  // Read until EOF for body, writing data to output as we go.
  std::string body_string;
  std::size_t bytes_read;
  boost::system::error_code error;
  while((bytes_read = boost::asio::read(*socket_, response_buffer, boost::asio::transfer_at_least(1), error))) {
    std::string read_data = std::string(boost::asio::buffers_begin(response_buffer.data()),
                                        boost::asio::buffers_begin(response_buffer.data()) + bytes_read);
    body_string += read_data;

    // Remove the bytes read from the buffer
    response_buffer.consume(bytes_read);
  }

  if (error != boost::asio::error::eof) {
    std::cerr << "Reading failed." << std::endl;
    return nullptr;
  }

  response->SetBody(body_string);

  return response;
  */

  /*
  // Read the response status line. The response streambuf will automatically
  // grow to accommodate the entire line. The growth may be limited by passing
  // a maximum size to the streambuf constructor.
  boost::asio::streambuf response;
  boost::asio::read_until(*socket_, response, "\r\n");

  // Check that response is OK.
  std::istream response_stream(&response);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
  {
    std::cout << "Invalid response\n";
    return nullptr;
  }
  if (status_code != 200)
  {
    std::cout << "Response returned with status code " << status_code << "\n";
    return nullptr;
  }

  // Read the response headers, which are terminated by a blank line.
  boost::asio::read_until(*socket_, response, "\r\n\r\n");

  // Process the response headers.
  std::string header;
  while (std::getline(response_stream, header) && header != "\r")
    std::cout << header << "\n";
  std::cout << "\n";

  // Write whatever content we already have to output.
  if (response.size() > 0)
    std::cout << &response;

  // Read until EOF, writing data to output as we go.
  boost::system::error_code error;
  while (boost::asio::read(*socket_, response,
        boost::asio::transfer_at_least(1), error))
    std::cout << &response;

  return nullptr;
  */
}

} // namespace server
} // namespace http
