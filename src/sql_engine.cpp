#include <iostream>
#include "sql_engine.hpp"

#include <stdlib.h>
#include <string>
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <resultset_metadata.h>

 
std::string SqlEngine::HandleRequest(std::string& field, int mode)
{
  std::string outString = "";
  try {
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("54.190.63.110:6603", "root", "");
    /* Connect to the MySQL test database */
    con->setSchema("DB");

    //If its a query, we want to return some results
    std::cout << "QueryStatement: " << field << "\n"; 
    stmt = con->createStatement();
    if (mode == 1){   //query mode
      res = stmt->executeQuery(field);
      //get result set metadata
      sql::ResultSetMetaData *res_meta = res -> getMetaData();
      int columns = res_meta -> getColumnCount();

      //Loop for each row
      while (res->next()) {
        /* Access column data by index, 1-indexed*/
        for (int i = 1; i <= columns; i++) {
          outString += res->getString(i) + ", ";
        }
      }
    }
    //if its an update, we dont need to return any results
    else if (mode == 2){  //update mode
      res = stmt->executeQuery(field);
      outString = "Update Attempted.";
    }

    delete res;
    delete stmt;
    delete con;

  } catch (sql::SQLException &e) {
    outString = "MYSQL ERR: INVALID SYNTAX";
  }

  return outString;
}