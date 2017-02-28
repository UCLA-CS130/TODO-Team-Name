//
// request_handler_proxy.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "request_handler_proxy.hpp"
#include <string>
#include <boost/lexical_cast.hpp>
#include "response.hpp"
#include "request.hpp"
#include "request_handler.hpp"

namespace http {
namespace server {

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	return RequestHandler::OK;
}

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request, Response* response) {

	return RequestHandler::OK;
}

REGISTER_REQUEST_HANDLER(ProxyHandler);

} // namespace server
} // namespace http
