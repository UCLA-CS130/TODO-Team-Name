#include <iostream>
#include "gtest/gtest.h"
#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"



class ConnectionTest : public ::testing::Test {
protected:
	bool isSocketOpen() {
        conn = new http::server::connection(io_service, manager_, handler_);
        return conn->socket().is_open();
    }
    void startConn() {
        conn = new http::server::connection(io_service, manager_, handler_);
        conn->start();
    }
    void stopConn() {
        conn = new http::server::connection(io_service, manager_, handler_);
        conn->stop();
    }
    http::server::connection* conn;
    boost::asio::io_service io_service;
    http::server::connection_manager manager_; 
    http::server::request_handler handler_;
};

TEST_F(ConnectionTest, SocketTest) {
	//When we get the socket, we expect it to be closed
	//since we never bound it.
    EXPECT_FALSE(isSocketOpen());
}

TEST_F(ConnectionTest, StartTest) {
	//starting connection should throw an exception
	//this is designed to expect that exception to be thrown 
    EXPECT_ANY_THROW(startConn());
    //this is expected to throw an exception because it is unable to 
    //start a connection with an uninitialized io_service
}


TEST_F(ConnectionTest, StopTest) {
    //stopping connection should not throw an exception
	//it merely stops all connections, if there is none to stop it returns silently
    EXPECT_NO_THROW(stopConn());
}