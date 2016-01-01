#ifndef TCP_H_INCLUDED
#define TCP_H_INCLUDED

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <quaternion.h>

class TCPsession
{
public:
	boost::asio::ip::tcp::socket socket;
	quat* orientation;
	TCPsession(boost::asio::io_service& io_service, quat* q) : socket(io_service), orientation(q) {}

	void start_read()
	{
		socket.async_read_some(boost::asio::buffer(read, sizeof(float)), boost::bind(&TCPsession::handle_read, this, boost::asio::placeholders::error));
	}

private:
	float read[1];
	float buffer[5];

	void handle_read(const boost::system::error_code& error)
	{
		if (!error)
		{
			buffer[0] = buffer[1];
			buffer[1] = buffer[2];
			buffer[2] = buffer[3];
			buffer[3] = buffer[4];
			buffer[4] = read[0];

			if(buffer[0] == 9999.0)
			{
				*orientation = quat(buffer[1], buffer[2], buffer[3], buffer[4]);
			}

			start_read();
		}
		else delete this;
	}
};

class TCPserver
{
private:
  	void start_accept()
  	{
		session = new TCPsession(io_service, &orientation);
		acceptor.async_accept(session->socket, boost::bind(&TCPserver::handle_accept, this, session, boost::asio::placeholders::error));
  	}

	void handle_accept(TCPsession* session, const boost::system::error_code& error)
  	{
		if (!error)
			session->start_read();
		else
			delete session;
		start_accept();
    }

  	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	TCPsession* session;

public:
  	TCPserver(int PORT) : acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT)) { }

	void operator()()
	{
		try
		{
			std::cout<< "Server starting";
			start_accept();
			io_service.run();
		}
		catch (std::exception& e) { std::cerr << "Exception: " << e.what() << "\n"; }
	}

	quat orientation;
};

#endif
