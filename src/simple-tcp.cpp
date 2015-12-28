//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session
{
public:
  	session(boost::asio::io_service& io_service) : socket_(io_service) {}
  	tcp::socket& socket() { return socket_; }

  	void start()
  	{
    	socket_.async_read_some(boost::asio::buffer(data_, sizeof(float)),
        	boost::bind(&session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  	}

private:
  	void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
  	{
    	if (!error)
		{
			std::cout << data_[0] << " ";
			start();
		}
    	else delete this;
  	}

  	tcp::socket socket_;
  	float data_[1];
};

class server
{
public:
  	server(boost::asio::io_service& io_service, short port) :
				io_service_(io_service),
				acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
  		{ start_accept(); }

private:
  	void start_accept()
  	{
    	session* new_session = new session(io_service_);
    	acceptor_.async_accept(
				new_session->socket(),
        		boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
  	}

	void handle_accept(session* new_session, const boost::system::error_code& error)
  	{
    	if (!error) new_session->start();
    	else delete new_session;
		start_accept();
    }

  	boost::asio::io_service& io_service_;
  	tcp::acceptor acceptor_;
};

int tcp_main()
{
  	try
  	{
		short PORT = 1234;
    	boost::asio::io_service io_service;
    	server s(io_service, PORT);
    	io_service.run();
  	}
  	catch (std::exception& e) { std::cerr << "Exception: " << e.what() << "\n"; }

  	return 0;
}
