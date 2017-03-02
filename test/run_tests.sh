#!/bin/bash

echo "Running Integration Test..."
./test/integration_test.sh

echo "Running Unit Tests..."
echo "Building Unit Tests..."
make test
echo "Server Test"
./server_test
echo "Request Handler Echo Test"
./request_handler_echo_test
echo "Request Handler Static Test"
./request_handler_static_test
echo "Request Handler Not Found Test"
./request_handler_notfound_test
echo "Response Test"
./response_test
echo "Request Test"
./request_test
echo "Connection Test"
./connection_test
echo "Connection Manager Test"
./connection_manager_test
echo "Request Parser Test"
./request_parser_test

echo "All tests complete. Cleaning up..."
make clean
