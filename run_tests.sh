#!/bin/bash

echo "Running Integration Test..."
./integration_test.sh

echo "Running Unit Tests..."
echo "Building Unit Tests..."
make test
echo "Server Test"
./server_test
echo "Request Handler Echo Test"
./request_handler_echo_test
echo "Reply Test"
./reply_test
echo "Connection Test"
./connection_test
echo "Connection Manager Test"
./connection_manager_test
echo "Request Parser Test"
./request_parser_test

echo "All tests complete. Cleaning up..."
make clean
