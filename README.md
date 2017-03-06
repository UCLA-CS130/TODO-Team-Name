# CS 130 Project - Web Server

// TODO: Team Name

Matt Dragotto, Andy Kuang, McKenna Galvin


A full HTTP web server, from the [boost documentation](http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/examples.html). This code is an adapted version of the [HTTP server](http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/examples/cpp11_examples.html) example.

---

### To Build
Run `make build`

---

### Usage
* Edit `config` to define the port on which you want the server to run.
* Run `make` to build the source code
* Run server using `./webserver config`

The server can be accessed via a variety of methods:
* Use your browser to view the resource (for example, `http://localhost:8080/`).
* Use a curl request: `curl -i -s localhost:8080`

To test the static file handler, you can try requesting some of the following resources (assuming the config file is kept the same):
* `localhost:8080/static1/index.html`
* `localhost:8080/static2/ducks.jpg`

---

### Running tests
* `make integration` runs the integration test only
* `make test` builds the test binaries for the individual files. You can then run each test separately, if you choose (for example, `./request_handler_echo_test`)
* `make all-tests` builds and runs all unit and integration tests

---

### Dealing with bound ports
If when running the server you get an error that the port is already bound,
execute `sudo lsof -i:8080` (change 8080 to whatever port number you're using).
Then find the PID of the process associated with the server and kill it using `sudo kill -9 <pid>`

---

### Source code explanation
* `main`: Parses the config file, initializes and runs the server
* `server_options`: Struct which contains the config options specified in the config file. Passed to the server upon creation so the server can properly set up request handlers
* `server`: Constructs a server to listen on the specified TCP address and port, and serves up files from the given directory
* `connection_manager`: Manages open connections so they may be cleanly stopped when the server needs to shut down
* `connection`: Represents a single TCP connection from a client
* `request_handler`: Processes a request and fills out a response (variations are `request_handler_echo`, `request_handler_static`, `request_handler_notfound`, and `request_handler_status`)
* `request`: Represents the contents of a HTTP request
* `response`: Represents the contents of an HTTP response

We also use the following helper classes taken from the boost example:
* `mime_types`
* `request_parser`


---

### Deploying on AWS
(Mainly just notes for me so I can remember how to do it again. I had saved the EC2 public key as webserver.pem in the root directory of the repo.)
* Save docker image as tar file: `docker save -o httpserver.tar httpserver`
* Copy the docker image to AWS: `sudo scp -i webserver.pem httpserver.tar ec2-user@54.190.63.110:/home/ec2-user`
* SSH into AWS: `ssh -i webserver.pem ec2-user@54.190.63.110`
* Load the image into docker: `docker load -i httpserver.tar`
* Run the server: `docker run --rm -t -p 8080:8080 httpserver`
* If the port is already allocated, run `docker ps` and `docker kill <container id>`
