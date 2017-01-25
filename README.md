# CS130 Project - Web Server

A full HTTP web server, from the [boost documentation](http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/examples.html). This code is an adapted version of the [HTTP server](http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/examples/cpp11_examples.html) example.

### Getting started
Set the `BOOST_PATH` environment variable to the location where you downloaded Boost.
For example:
```
export BOOST_PATH=~/Desktop/TODO-Team-Name/boost_1_63_0
```

### Usage
Edit `config` to define the port on which you want the server to run. Run server using:
```
$ sudo ./webserver config
```

The server can be accessed via a variety of methods:
* Use your browser to view the resource (for example, `http://localhost:8080/`).
* Use a curl request: `curl -i localhost:8080`


###Dealing with bound ports
If when running the server you get an error that the port is already bound,
execute the following (change 8080 to whatever port number you're using):
```
sudo lsof -i:8080
```
Then find the PID of the process associated with the server and kill it:
```
sudo kill -9 <pid>
```

###Source code explanation
* `main`: Parses the config file, initializes and runs the server
* `server`: Constructs a server to listen on the specified TCP address and port, and serves up files from the given directory
* `connection_manager`: Manages open connections so they may be cleanly stopped when the server needs to shut down
* `connection`: Represents a single TCP connection from a client
* `request_handler`: Processes a request and fills out a reply
* `request`: A struct representing the contents of a HTTP request
* `reply`: A struct representing the contents of an HTTP reply
* `header`: A struct representing a header item's name/value pair
