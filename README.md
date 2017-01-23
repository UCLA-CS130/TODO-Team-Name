# CS130 Project - Web Server

A full HTTP web server, from the [boost documentation](http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/examples.html)

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

Use your browser to view the resource (for example, `http://localhost:80/default.html`).

Request `default.html` for an awesome webpage with some really cool text.


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

###TODO: 
* Change the implementation of the server to return the user's request, rather than
parsing it and returning the requested page.
* Integrate nginx-configparser.