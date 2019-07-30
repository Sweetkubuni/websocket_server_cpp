#include "basic_message.h"

std::shared_ptr<session_server_message::Message> session_server_message::MessageHandle(std::string arg_json)
{

	if (!arg_json.empty())
	{
		const bool match = 0;
		rapidjson::Document document;

		document.Parse(arg_json.c_str());

		//we check if root is an object and has the base properties
		if (document.IsObject() && document.HasMember("msg_type"))
		{

			if ( (std::strcmp(document["msg_type"].GetString(), "broadcast") == match) && document.HasMember("message") )
			{
				return session_server_message::CreateMessage(MsgType::BroadCast,  document["message"].GetString());
			}

		}
	}

	return std::shared_ptr<session_server_message::Message>();
}
