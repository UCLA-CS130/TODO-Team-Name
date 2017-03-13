//
// request_handler_sql.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "request_handler_sql.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "mime_types.hpp"
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
	// Get root directory from config.
	  std::vector<std::shared_ptr<NginxConfigStatement>> statements = config.statements_;
	  if (statements.size() == 1) {
	    std::vector<std::string> statement_tokens = statements.at(0)->tokens_;
	    if (statement_tokens.size() == 2 && statement_tokens.at(0) == "root") {
	      root_ = statement_tokens.at(1);
	      return RequestHandler::OK;
	    }
	  }

  	  return RequestHandler::BAD_CONFIG;
}

RequestHandler::Status SqlHandler::HandleRequest(const Request& request, Response* response) {
	  //Flag for whether a query, insert, etc.
	  int SQL_MODE = 0; //0 = none
	  					//1 = query
	  					//2 = update

	  //Will be present in the header whenever a query is made
	  std::string queryRequest = "query?q=";
	  std::string updateRequest = "update?q=";

	  //parse request url
	  std::string request_string = request.ToString();
	  std::string qField = "";

	  //check for query
	  std::size_t foundQ = request_string.find(queryRequest);
	  if (foundQ != std::string::npos){
	  	std::string request_substr = request_string.substr(request_string.find(queryRequest) + queryRequest.length());
	  	std::string qField_d = request_substr.substr(0, request_substr.find(" ", 0));
	  	url_decode(qField_d, qField);
	  	SQL_MODE = 1;
	  }

	  //check for update
	  std::size_t foundUp = request_string.find(updateRequest);
	  if (foundUp != std::string::npos){
	  	std::string request_substr = request_string.substr(request_string.find(updateRequest) + updateRequest.length());
	  	std::string qField_d = request_substr.substr(0, request_substr.find(" ", 0));
	  	url_decode(qField_d, qField);
	  	SQL_MODE = 2;
	  }

	  //fill out result string
	  std::string resString = "";
	  if (SQL_MODE == 1){
	  	SqlEngine sqlEngine;
	  	resString = sqlEngine.HandleRequest(qField, 1);
	  }
	  else if (SQL_MODE == 2){
	  	SqlEngine sqlEngine;
	  	resString = sqlEngine.HandleRequest(qField, 2);
	  }

	// Open the file to send back.
	  std::string full_path = root_ + "/sqlpage.html";
	  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	  if (!is) {
	    response->SetStatus(Response::NOT_FOUND);
	    return RequestHandler::NOT_FOUND;
	  }

	  // Fill out the response to be sent to the client.
	  response->SetStatus(Response::OK);
	  char buf[512];
	  std::string file_content = "";
	  while (is.read(buf, sizeof(buf)).gcount() > 0) {
	    file_content.append(buf, is.gcount());
	  }
	  if (resString.length() != 0){
	  	file_content.append("<h1>Results:</h1>");
	  }
	  file_content.append(resString.c_str(), resString.length());
	  std::string ender = "</body></html>";
	  file_content.append(ender.c_str(), ender.length());
	  response->SetBody(file_content);
	  response->AddHeader("Content-Length", boost::lexical_cast<std::string>(file_content.length()));
	  response->AddHeader("Content-Type", mime_types::extension_to_type("html"));
	  return RequestHandler::OK;
}

bool SqlHandler::url_decode(const std::string& in, std::string& out) {
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (in[i] == '%') {
      if (i + 3 <= in.size()) {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value) {
          out += static_cast<char>(value);
          i += 2;
        }
        else {
          return false;
        }
      }
      else {
        return false;
      }
    }
    else if (in[i] == '+') {
      out += ' ';
    }
    else {
      out += in[i];
    }
  }
  return true;
}

REGISTER_REQUEST_HANDLER(SqlHandler);

} // namespace server
} // namespace http