
#include "router.h"
#include "telephone.h"
#include "threadsafe_queue.h"
#include <iostream>
#include <thread>

session_server_datastructure::threadsafe_queue<session_server_message::Message> router_queue;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr <<
				"Usage: websocket-server-sync <address> <port>\n" <<
				"Example:\n" <<
				"    websocket-server-sync 0.0.0.0 8080\n";
			return EXIT_FAILURE;
		}

		auto const address = boost::asio::ip::make_address(argv[1]);
		auto const port = static_cast<unsigned short>(std::atoi(argv[2]));


		// The io_context is required for all I/O
		boost::asio::io_context ioc{ 1 };

		std::thread t1{ session_server::Router(router_queue) };
		// The acceptor receives incoming connections
		boost::asio::ip::tcp::acceptor acceptor{ ioc, {address, port} };

		for (;;)
		{
			// This will receive the new connection
			auto socket_ptr = std::make_shared<boost::asio::ip::tcp::socket> ( ioc );

			// Block until we get a connection
			acceptor.accept(*socket_ptr);

			// Launch the session, transferring ownership of the socket
			std::thread { session_server::TelephoneSession(), socket_ptr, std::ref(router_queue) }.detach();
		}

		t1.join();
	}

	catch (const std::exception & e)
	{
		return EXIT_FAILURE;
	}
	return 0;
}