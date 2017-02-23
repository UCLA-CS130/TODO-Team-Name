//
// request_handler_status.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "request_handler_status.hpp"
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include "response.hpp"
#include "request.hpp"

namespace http {
namespace server {

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	TotalRequests = 0;
	uri_prefix_ = uri_prefix;
	return RequestHandler::OK;
}

bool StatusHandler::SetHandlers(const std::vector<std::pair<std::string, std::string>> all_handlers) {
	handlers_ = all_handlers;
	return true;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response) {
	std::string StatusBuilder = "<html><head><title>Server Status</title></head><body>"; //404 Not Found</h1></body></html>";
	std::string someLine;
	StatusBuilder.append("<h1>These are the handlers currently active on the server</h1>");
	for(std::vector<int>::size_type i = 0; i != handlers_.size(); i++) {
		someLine = "The handler " + handlers_[i].first + " handles the path " + handlers_[i].second + ".<br></br>";
		StatusBuilder.append(someLine);
	}
 	
 	someLine = "<h1>There were " + std::to_string(TotalRequests) + " total requests made to the server.</h1>";
	StatusBuilder.append(someLine);
	for(std::vector<int>::size_type i = 0; i != url_and_response.size(); i++) {
		someLine = "The url " + url_and_response[i].first + " was requested, and response code " + url_and_response[i].second + " was given.<br></br>";
		StatusBuilder.append(someLine);
	}

	StatusBuilder.append("</body></html>");

	response->SetStatus(Response::OK);
	response->AddHeader("Content-Length", std::to_string(StatusBuilder.size()));
	response->AddHeader("Content-Type", "text/html");
	response->SetBody(StatusBuilder);

	return RequestHandler::OK;
}

void StatusHandler::update(std::string uri, int respCode){
	TotalRequests++;
	url_and_response.push_back(std::make_pair(uri, std::to_string(respCode)));
}

} // namespace server
} // namespace http
