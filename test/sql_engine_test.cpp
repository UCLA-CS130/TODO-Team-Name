#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "sql_engine.hpp"


class SqlEngineTest : public ::testing::Test {
protected:
    std::string HandleQuery(std::string query) {
        return handler.HandleRequest(query, 0);
    }
    std::string HandleUpdate(std::string update) {
        return handler.HandleRequest(update, 1);
    }
    void recreate(){
        handler.HandleRequest(dropTable, 1);
        handler.HandleRequest(createTable, 1);
    }
    SqlEngine handler;
    std::string dropTable = "DROP TABLE IF EXISTS Test";
    std::string createTable = "CREATE TABLE Test(name)";
};

TEST_F(SqlEngineTest, QueryTest) {
    recreate();
    HandleUpdate("INSERT INTO Test(name) VALUES(test)");
    std::string queryString = "SELECT * FROM Test";
    std::string expectedOutput = "test";
	EXPECT_EQ(expectedOutput, HandleQuery(queryString));
}

TEST_F(SqlEngineTest, EmptyQuery) {
    std::string errorString = "MYSQL ERR: INVALID SYNTAX";
    EXPECT_EQ(errorString, HandleQuery(""));
}

TEST_F(SqlEngineTest, UpdateTest) {
    recreate();
    std::string updateString = "INSERT INTO Test(name) VALUES(test)";
    std::string expectedOutput = "Update Success!";
    EXPECT_EQ(expectedOutput, HandleUpdate(updateString));
}

TEST_F(SqlEngineTest, EmptyUpdate) {
    std::string errorString = "MYSQL ERR: INVALID SYNTAX";
    EXPECT_EQ(errorString, HandleUpdate(""));
}