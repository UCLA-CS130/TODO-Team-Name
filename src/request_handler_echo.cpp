//
// request_handler_echo.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler_echo.hpp"
#include <string>
#include <boost/lexical_cast.hpp>
#include "response.hpp"
#include "request.hpp"

namespace http {
namespace server {

RequestHandler::Status RequestHandlerEcho::Init(const std::string& uri_prefix, const NginxConfig& config) {
	uri_prefix_ = uri_prefix;
	return RequestHandler::OK;
}

RequestHandler::Status RequestHandlerEcho::HandleRequest(const Request& request, Response* response) {
	response->SetStatus(Response::OK);
	response->AddHeader("Content-Length", std::to_string(request.raw_request().size()));
	response->AddHeader("Content-Type", "text/plain");
	response->SetBody(request.raw_request());

	return RequestHandler::OK;
}

} // namespace server
} // namespace http
