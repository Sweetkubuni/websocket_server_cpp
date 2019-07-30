#pragma once
#ifndef REGISTRATION_MESSAGE_H
#define REGISTRATION_MESSAGE_H
#include "basic_message.h"
#include "telephone.h"
namespace session_server_message {
	struct RegistrationRequestMessage : public Message {

		session_server::TelephoneSession* telephonesession;

		RegistrationRequestMessage(session_server::TelephoneSession* arg_telephonesession) :telephonesession(arg_telephonesession)
		{
			msg_type = "registration";
		}

		std::string Get(const char* str)
		{
			const bool match = 0;

			if (strcmp(str, "msg_type") == match)
				return msg_type;
			return std::string();
		}

		std::string ConvertJSON() { return std::string(); }
	};

	std::shared_ptr<session_server_message::Message> CreateRegistrationMessage(session_server::TelephoneSession* arg_telephonesession);
}
#endif // !REGISTRATION_MESSAGE_H
