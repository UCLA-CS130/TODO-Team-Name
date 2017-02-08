# Build the binary
make clean
make

# Start the server
echo "Running an integration test on the server"
echo "server { listen 8080;
path /echo EchoHandler;
path /static1 StaticFileHandler { root www; }
path /static2 StaticFileHandler { root pics; } }" >> test_config
./webserver test_config &>/dev/null &

# Send a request to the server using curl
echo "Sending a curl request"
curl -i -s localhost:8080/echo > test_output_echo
curl -s localhost:8080/static1/default.html > test_output_static1
curl -s localhost:8080/static2/ducks.jpg > test_output_static2_jpg
curl -s localhost:8080/static2/giphy.gif > test_output_static2_gif
curl -s localhost:8080/static2/mario.jpeg > test_output_static2_jpeg

# Verify the response from the server works as expected
diff expected_echo_output test_output_echo > diff_echo_output
EXIT_STATUS=$?
diff www/default.html test_output_static1 > diff_static1_output
EXIT_STATUS=$?
diff pics/ducks.jpg test_output_static2_jpg > diff_static2_jpg_output
EXIT_STATUS=$?
diff pics/giphy.gif test_output_static2_gif > diff_static2_gif_output
EXIT_STATUS=$?
diff pics/mario.jpeg test_output_static2_jpeg > diff_static2_jpeg_output
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
	cat diff_echo_output
	cat diff_static1_output
	cat diff_static2_jpg_output
	cat diff_static2_gif_output
	cat diff_static2_jpeg_output
fi

# Shutdown the webserver
echo "Cleaning up and shutting down"
pkill webserver
make clean
rm test_config
rm test_output_echo
rm test_output_static1
rm test_output_static2_jpg
rm test_output_static2_gif
rm test_output_static2_jpeg
rm diff_echo_output
rm diff_static1_output
rm diff_static2_jpg_output
rm diff_static2_gif_output
rm diff_static2_jpeg_output

exit "$EXIT_STATUS"
