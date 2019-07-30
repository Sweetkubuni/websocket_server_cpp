#pragma once
#ifndef TELEPHONE_H
#define TELEPHONE_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "threadsafe_queue.h"
#include "basic_message.h"
namespace session_server {
	class TelephoneSession {
	private:
		std::string session_identity;
		boost::beast::websocket::stream<boost::asio::ip::tcp::socket> * ws_ptr;
	public:
		TelephoneSession();

		void operator () (std::shared_ptr<boost::asio::ip::tcp::socket> arg_socket, session_server_datastructure::threadsafe_queue<session_server_message::Message>& router_queue);
		void RecieveMessage(std::shared_ptr<session_server_message::Message>& message);
		void AssignIdentity(std::string arg_identidy);
	};

}

#endif // !TELEPHONE_H