#include <iostream>
#include "gtest/gtest.h"
#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler_echo.hpp"
#include "request_handler_static.hpp"

class ConnectionManagerTest : public ::testing::Test {
protected:
    void makeConn() {
        conn = new http::server::connection(io_service, manager_, request_handler_static_, request_handler_echo_);
    }
    void startConnMan() {
        manager_.start(conn->shared_from_this());
    }
    void stopConnMan() {
        manager_.stop(conn->shared_from_this());
    }
    void stopAllConnMan() {
        manager_.stop_all();
    }
    http::server::connection* conn;
    boost::asio::io_service io_service;
    http::server::connection_manager manager_; 
    http::server::request_handler_static request_handler_static_;
    http::server::request_handler_echo request_handler_echo_;
};

TEST_F(ConnectionManagerTest, SimpleConnection) {
    //make sure we can successfully form a connection
	EXPECT_NO_THROW(makeConn());
}

TEST_F(ConnectionManagerTest, StartManager) {
    makeConn();
    //We expect bad_weak_ptr exception just like start() in connection,
    //since the manager tries to start the connection.
    EXPECT_ANY_THROW(startConnMan());
}

TEST_F(ConnectionManagerTest, StopManager) {
    makeConn();
    //we accept a throw because conn is currently not running, theres nothing to stop
    EXPECT_ANY_THROW(stopConnMan());
}

TEST_F(ConnectionManagerTest, StopAllManager) {
    //there should be nothing thrown. 
    //with no connections, the manager will stop all (none), and then return.
    EXPECT_NO_THROW(stopAllConnMan());
}
