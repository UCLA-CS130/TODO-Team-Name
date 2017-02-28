// echo_client.cpp
// g++ -o echo_client -O3 echo_client.cpp -lboost_system -lboost_thread
#include <boost/asio.hpp>
namespace asio = boost::asio;
using asio::ip::tcp;
#include <boost/bind.hpp>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>


class session
: public boost::enable_shared_from_this<session>
{
    public:
    session(asio::io_service& io, std::string request)
    : socket_(io),
      request_(request)
      { }

    tcp::socket& socket()
    { return socket_; }

    void start()
    {
    	asio::async_write(socket_, asio::buffer(request_, request_.length()), boost::bind(&session::handle_write, shared_from_this(), _1, _2));
    }

    void handle_write(const boost::system::error_code& ec, std::size_t bytes_transfered)
    {
        if(!ec)
        {
            asio::async_read(socket_, asio::buffer(input_buffer_, bytes_transfered), boost::bind(&session::handle_read, shared_from_this(), _1, _2));
        } else {
            std::cerr << "write error:" << ec.message() << std:: endl;
        }
    }

    void handle_read(const boost::system::error_code& ec, std::size_t bytes_transfered)
    {
        if(ec)
        {
            std::cerr << "read error:" << ec.message() << std::endl;
        }
        std::string s(input_buffer_);
        std::cout << s;
    }

    private:
    tcp::socket socket_;
    std::string request_;
    char input_buffer_[8192];
};


void handle_connect(boost::shared_ptr<session> session_ptr, const boost::system::error_code& ec)
{
    if(ec)
    {
        std::cerr << "connect error:" << ec.message() << std::endl;
    } else {
        session_ptr->start();
    }
}


int main(int argc, char* argv[])
{
    asio::io_service io;
    tcp::resolver resolver(io);
    std::string s(argv[1]);
    std::string request;
    if (s == "echo"){
    	request = "GET /echo HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate, sdch, br\r\nAccept-Language: en-GB,en-US;q=0.8,en;q=0.6\r\n\r\n";
    }
    else if (s == "partial"){
    	request = "GET /static1 HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive";
    }
    tcp::resolver::iterator endpoint = resolver.resolve(tcp::resolver::query("localhost", "8080"));
    boost::shared_ptr<session> session_ptr;
    session_ptr.reset(new session(io, request));
    asio::async_connect(session_ptr->socket(), endpoint, boost::bind(handle_connect, session_ptr, _1));
    std::vector<boost::shared_ptr<boost::thread>> threadArray;

      for (int i = 0; i < 4; i++){
        boost::shared_ptr<boost::thread> temp(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
        threadArray.push_back(temp);
      }

      for (int i = 0; i < 4; i++){
        threadArray[i]->join();
      }

    io.run();
}