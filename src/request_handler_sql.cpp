//
// request_handler_sql.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "request_handler_sql.hpp"
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include "response.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "sql_engine.hpp"

namespace http {
namespace server {

RequestHandler::Status SqlHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	root_ = uri_prefix;
	return RequestHandler::OK;
}

RequestHandler::Status SqlHandler::HandleRequest(const Request& request, Response* response) {
	SqlEngine sqlEngine;
	sqlEngine.HandleRequest();

	response->SetStatus(Response::OK);
	response->AddHeader("Content-Length", std::to_string(request.raw_request().size()));
	response->AddHeader("Content-Type", "text/plain");
	response->SetBody(request.raw_request());

	return RequestHandler::OK;
}

REGISTER_REQUEST_HANDLER(SqlHandler);

} // namespace server
} // namespace http