#pragma once
#ifndef MESSAGE_HANDLE_H
#define MESSAGE_HANDLE_H
#include <type_traits>
#include <cstring>
#include <string>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "threadsafe_queue.h"
#include  <memory>

namespace session_server_message {

	enum class MsgType {
		Registration,
		BroadCast,
		Status,
		Close
	};

	/**
		Message
		Generic Abstract Message Class

		must define GET
		must define convertJSON
	*/
	struct Message {
		virtual std::string Get(const char* str) = 0;
		virtual std::string ConvertJSON() = 0;

	protected:
		std::string msg_type;
	};

	/**
		BroadCastMessage
		Relays text message to all sessions
	*/
	struct BroadCastMessage : public Message {
		BroadCastMessage( std::string arg_text) :text(std::move(arg_text))
		{
			msg_type = "broadcast";
		}

		std::string Get(const char* str)
		{
			const bool match = 0;

			if (strcmp(str, "msg_type") == match)
				return msg_type;
			return std::string();
		}

		std::string ConvertJSON() { 
			rapidjson::StringBuffer sb;
			rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

			// create json object
			writer.StartObject();

			// message type
			writer.Key("msg_type");
			writer.String(msg_type.c_str());

			// broadcast message
			writer.Key("message");
			writer.String(text.c_str());

			writer.EndObject();

			//output json string
			return sb.GetString();
		}

	protected:
		std::string text;
	};

	/**
		CloseMessage
		Close Message that is sent from session thread to router thread
	*/
	struct CloseMessage : public Message {
		CloseMessage(std::string arg_sender)
		{
			sender = arg_sender;
			msg_type = "close";
		}

		std::string Get(const char* str)
		{
			const bool equal = 0;
			if (strcmp(str, "msg_type") == equal)
				return msg_type;
			if (strcmp(str, "sender") == equal)
				return sender;
			return std::string();
		}

		std::string ConvertJSON() { return std::string(); }
	protected:
		std::string sender;
	};


	struct StatusMessage : public Message {

		StatusMessage(std::string arg_sender, std::string arg_recipient, std::string arg_status) : status(arg_status)
		{
			sender = arg_sender;
			recipient = arg_recipient;
			msg_type = "status";
		}

		std::string Get(const char* str)
		{
			const bool equal = 0;
			if (strcmp(str, "msg_type") == equal)
				return msg_type;
			if (strcmp(str, "sender") == equal)
				return sender;
			if (strcmp(str, "recipient") == equal)
				return recipient;
			return std::string();
		}

		std::string ConvertJSON()
		{
			rapidjson::StringBuffer sb;
			rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

			// create json object
			writer.StartObject();

			// message type
			writer.Key("msg_type");
			writer.String(msg_type.c_str());

			// where the message came from
			writer.Key("sender");
			writer.String(sender.c_str());

			// where the message is going
			writer.Key("recipient");
			writer.String(recipient.c_str());

			writer.EndObject();

			//output json string
			return sb.GetString();
		}

	protected:
		std::string status;
		std::string sender;
		std::string recipient;
	};


	template <typename Arg, typename... Ts, typename std::enable_if<std::is_same<std::string, Arg>::value || std::is_same<const char*, Arg>::value, Arg>::type* = nullptr>
	std::shared_ptr<session_server_message::Message> CreateMessage(MsgType msg_type, Arg i, Ts... all)
	{
		std::string args[] = { i, all... };
		if (msg_type == MsgType::Close)
		{
			return std::make_shared<CloseMessage>(args[0]);
		}

		if (msg_type == MsgType::BroadCast)
		{
			return  std::make_shared<BroadCastMessage>(args[0]);
		}

		if (msg_type == MsgType::Status)
		{
			std::string args[] = { i, all... };
			return std::make_shared<StatusMessage>(args[0], args[1], args[2]);
		}

		return std::shared_ptr<session_server_message::Message>();
	}

	std::shared_ptr<session_server_message::Message> MessageHandle(std::string arg_json);

}
#endif // !MESSAGE_HANDLE_H

