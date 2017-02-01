#!/bin/bash

# Build the binary
make clean
make

# Start the server
echo "Running an integration test on the server"
echo "server { listen 8080; }" >> test_config
./webserver test_config &>/dev/null &

# Send a request to the server using curl
echo "Sending a curl request"
curl -i -s localhost:8080 > test_output

# Verify the response from the server works as expected
diff expected_output test_output > diff_of_output
EXIT_STATUS=$?

# Indicate success or failure with an appropriate exit code
# Diff returns 0 if there are no differences, 1 if there are,
# and another number if something else went wrong
if [ "$EXIT_STATUS" -eq 0 ]
then
	echo "SUCCESS: Integration test produced expected output"
else
	echo "Integration test did not produce the expected result"
	echo "Diff of curl output:"
	cat diff_of_output
fi

# Shutdown the webserver
echo "Cleaning up and shutting down"
pkill webserver
rm test_config
rm test_output
rm diff_of_output

exit "$EXIT_STATUS"