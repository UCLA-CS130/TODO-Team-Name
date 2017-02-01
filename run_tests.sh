#!/bin/bash

echo "Running Integration Test..."
./integration_test.sh

echo "Running Unit Tests..."
echo "Building Unit Tests..."
make test
echo "Server Test"
./server_test
echo "Request Handler Test"
./request_handler_test
echo "Reply Test"
./reply_test
echo "Connection Test"
./connection_test
echo "Connection Manager Test"
./connection_manager_test

echo "All tests complete. Cleaning up..."
make clean