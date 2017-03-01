//
// request_handler_proxy.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "request_handler_proxy.hpp"
#include <string>
#include <boost/lexical_cast.hpp>
#include "http_client.hpp"
#include "response.hpp"
#include "request.hpp"
#include "request_handler.hpp"

namespace http {
namespace server {

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	uri_prefix_ = uri_prefix;

  // Set path uri of the end node
  path_ = "/";

	std::vector<std::shared_ptr<NginxConfigStatement>> statements = config.statements_;

  // Get root directory from config.
	if (statements.size() == 1) {
    std::vector<std::string> statement_tokens = statements.at(0)->tokens_;
    if (statement_tokens.size() == 2 && statement_tokens.at(0) == "url") {
      url_ = statement_tokens.at(1);
      return RequestHandler::OK;
    }
  }

  return RequestHandler::BAD_CONFIG;
}

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request, Response* response) {
  Request new_request = TransformRequest(request);
  Response* resp = RunOutsideRequest(new_request, url_, "http");
  if(resp == nullptr) {
    return RequestHandler::INTERNAL_SERVER_ERROR;
  }

  // add corresponding uri_prefix_ to all html paths
  // Not implemented due to errors
  // resp->AddRelativePaths(uri_prefix_);

  (*response) = (*resp);

  return RequestHandler::OK;
}

Request ProxyHandler::TransformRequest(const Request& request) const {
  Request transformed_request(request);
  transformed_request.set_header("Host", url_);
  transformed_request.set_header("Connection", "close");
  transformed_request.set_header("Accept-Encoding", "identity");
  transformed_request.set_uri(path_ + request.uri().substr(uri_prefix_.length()));
  return transformed_request;
}

Response* ProxyHandler::RunOutsideRequest(const Request& request, std::string url, std::string service) const {
  HttpClient c;
  c.EstablishConnection(url, service);
  auto resp = c.SendRequest(request);
  return resp;
}

REGISTER_REQUEST_HANDLER(ProxyHandler);

} // namespace server
} // namespace http
