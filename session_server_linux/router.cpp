#include "router.h"
#include "registration_message.h"
#include <utility>
//#include "spdlog/spdlog.h"

session_server::Router::Router(session_server_datastructure::threadsafe_queue<session_server_message::Message>& arg_router_queue):router_queue(arg_router_queue)
{

}

void session_server::Router::operator() ()
{
	for (;;)
	{
		auto message_ptr = router_queue.wait_and_pop();
		Recieve(message_ptr);
	}
}

void session_server::Router::Register(std::string username, session_server::TelephoneSession* arg_telephonesession)
{
	session_table.emplace(username, arg_telephonesession);
}

void session_server::Router::Recieve(std::shared_ptr<session_server_message::Message>& message)
{
	if (message)
	{
		const bool match = 0;
		auto check_exist = [this](const std::string username) {
			return !(session_table.find(username) == session_table.end());
		};

		if (message->Get("msg_type").compare("registration") == match)
		{
			auto register_message = std::dynamic_pointer_cast<session_server_message::RegistrationRequestMessage>(message);
			auto label = std::to_string(++id_counter);
			Register( label, register_message->telephonesession);
			register_message->telephonesession->AssignIdentity(label);
		}

		if (message->Get("msg_type").compare("close") == match)
		{
			auto it = session_table.find(message->Get("sender"));
			if (it != session_table.end())
			{
				session_table.erase(it);
			}
			return;
		}

		if (message->Get("msg_type").compare("broadcast") == match)
		{
			for (auto session : session_table)
			{
				session.second->RecieveMessage(message);
			}
		}
	}
}