#include "telephone.h"
#include "registration_message.h"
#include <string>
#include <utility>
#include <cstring>
#include <memory>
//#include "spdlog/spdlog.h"

session_server::TelephoneSession::TelephoneSession()
{ 
}

void session_server::TelephoneSession::operator () (std::shared_ptr<boost::asio::ip::tcp::socket> arg_socket, session_server_datastructure::threadsafe_queue<session_server_message::Message>& router_queue)
{
	try
	{
		const std::string ip_conn = arg_socket->remote_endpoint().address().to_string();
		boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws{ std::move(*arg_socket) };
		std::string sbuffer;
		auto buffer = boost::asio::dynamic_buffer(sbuffer);

		ws.accept();
		ws_ptr = &ws;

		auto register_message = session_server_message::CreateRegistrationMessage(this);
		router_queue.push(register_message);

		for (;;)
		{
			ws.read(buffer);
			auto message = session_server_message::MessageHandle(sbuffer);

			if (message)
			{
				router_queue.push(message);
			}

			sbuffer.clear();
		}
	}

	catch (boost::beast::system_error const& se)
	{

	}
	catch (std::exception const& e)
	{
		//place some error here
	}

	router_queue.push(CreateMessage(session_server_message::MsgType::Close, session_identity));
}

void session_server::TelephoneSession::RecieveMessage(std::shared_ptr<session_server_message::Message> & message)
{
	auto json_str = message->ConvertJSON();
	boost::asio::const_buffer buffer(json_str.c_str(), json_str.length());
	ws_ptr->text(true);
	ws_ptr->write(buffer);
}

void session_server::TelephoneSession::AssignIdentity(std::string arg_identity)
{
	session_identity = arg_identity;
}