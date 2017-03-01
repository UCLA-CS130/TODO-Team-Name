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

	std::vector<std::shared_ptr<NginxConfigStatement>> statements = config.statements_;

  // Get root directory from config.
	if (statements.size() == 1) {
    std::vector<std::string> statement_tokens = statements.at(0)->tokens_;
    if (statement_tokens.size() == 2 && statement_tokens.at(0) == "url") {
      std::string full_url = statement_tokens.at(1);

      std::string::size_type protocol_pos = full_url.find("//");
      if(protocol_pos == std::string::npos) {
        std::cout << "ProxyHandler " << uri_prefix << " didn't specify protocol. Using default http" << std::endl;
        protocol_ = "http";

        std::string::size_type host_pos = full_url.find('/');
        if(host_pos != std::string::npos) {
          host_ = full_url.substr(0, host_pos);
          path_ = full_url.substr(host_pos);
        }
        else {
          host_ = full_url;
          path_ = "/";
        }
      }
      else {
        protocol_ = full_url.substr(0, protocol_pos - 1);

        std::string::size_type host_pos = full_url.find('/', protocol_pos + 2);
        if(host_pos != std::string::npos) {
          host_ = full_url.substr(protocol_pos + 2, host_pos - protocol_pos - 2);
          path_ = full_url.substr(host_pos);
        }
        else {
          host_ = full_url.substr(protocol_pos + 2);
          path_ = "/";
        }
      }

      return RequestHandler::OK;
    }
  }

  return RequestHandler::BAD_CONFIG;
}

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request, Response* response) {
  Request new_request = TransformRequest(request);
  Response* resp = RunOutsideRequest(new_request);
  if(resp == nullptr) {
    return RequestHandler::INTERNAL_SERVER_ERROR;
  }

  if(resp->GetStatus() == Response::FOUND) {
	  std::cout << "redirecting" << std::endl;
	if(!resp->GetRedirectHostAndPath(host_, path_, protocol_)) {
	  return RequestHandler::INTERNAL_SERVER_ERROR;
	}
    Request redirect_request = TransformRedirect(new_request);
    resp = RunOutsideRequest(redirect_request);
    if(resp == nullptr) {
      return RequestHandler::INTERNAL_SERVER_ERROR;
    }
  }
`
  (*response) = (*resp);

  return RequestHandler::OK;
}

Request ProxyHandler::TransformRequest(const Request& request) const {
  Request transformed_request(request);
  transformed_request.remove_header("Cookie");
  transformed_request.set_header("Host", host_);
  transformed_request.set_header("Connection", "close");
  transformed_request.set_uri(path_ + request.uri().substr(uri_prefix_.length()));
  return transformed_request;
}

Request ProxyHandler::TransformRedirect(const Request& request) const {
  Request transformed_request(request);
  transformed_request.remove_header("Cookie");
  transformed_request.set_header("Host", host_);
  transformed_request.set_header("Connection", "close");
  transformed_request.set_uri(path_);

  return transformed_request;
}

Response* ProxyHandler::RunOutsideRequest(const Request& request) const {
  HttpClient c;
  c.EstablishConnection(host_, protocol_);
  auto resp = c.SendRequest(request);
  return resp;
}

REGISTER_REQUEST_HANDLER(ProxyHandler);

} // namespace server
} // namespace http
