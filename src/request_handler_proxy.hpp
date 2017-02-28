//
// request_handler_proxy.hpp
// ~~~~~~~~~~~~~~~~~~~
//

#ifndef HTTP_REQUEST_HANDLER_PROXY_HPP
#define HTTP_REQUEST_HANDLER_PROXY_HPP

#include "request_handler.hpp"

namespace http {
namespace server {

// Handler for echo requests.
class ProxyHandler : public http::server::RequestHandler {
public:
  Status Init(const std::string& uri_prefix, const NginxConfig& config) override;

  Status HandleRequest(const Request& request, Response* response) override;
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_PROXY_HPP
