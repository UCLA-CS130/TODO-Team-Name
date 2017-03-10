//
// sql_engine.hpp
//
#include <mysql.h>

#ifndef SQL_ENGINE_HPP
#define SQL_ENGINE_HPP

// just going to input the general details and not the port numbers
struct connection_details
{
    char *server;
    char *user;
    char *password;
    char *database;
};

class SqlEngine {
public:
	bool HandleRequest();

private:
	MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query);
	MYSQL* mysql_connection_setup(struct connection_details mysql_details);
};

#endif // SQL_ENGINE_HPP