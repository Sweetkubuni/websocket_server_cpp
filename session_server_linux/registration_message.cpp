#include "registration_message.h"

std::shared_ptr<session_server_message::Message> session_server_message::CreateRegistrationMessage( session_server::TelephoneSession* arg_telephonesession)
{

	return std::make_shared<session_server_message::RegistrationRequestMessage>(arg_telephonesession);
}
