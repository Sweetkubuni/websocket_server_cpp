#pragma once
#ifndef ROUTER_H
#define ROUTER_H
#include <memory>
#include <map>
#include <string>
#include "threadsafe_queue.h"
#include "basic_message.h"
#include "telephone.h"
namespace session_server {
	class Router {
	private:
		session_server_datastructure::threadsafe_queue<session_server_message::Message> & router_queue;
		std::map<std::string, session_server::TelephoneSession *> session_table;
		std::size_t id_counter = 0;

	public:
		Router(session_server_datastructure::threadsafe_queue<session_server_message::Message>& arg_router_queue);
		void operator () ();
		void Register(std::string username, TelephoneSession* arg_telephonesession);
		void Recieve(std::shared_ptr<session_server_message::Message>& message);
	};
}
#endif // !ROUTER_H
