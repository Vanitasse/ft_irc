#pragma once

# include "Irc.hpp"

class Client;
class Channel;

class FormatIRC
{
	private:
		FormatIRC();
		~FormatIRC();
		FormatIRC(const FormatIRC& toCpy);
		FormatIRC& operator=(const FormatIRC& toCpy);

		static std::string	_domain;

	public:
		static void	sender(int fd, const std::string& format);
		static void	sendWelcome(int fd, const std::string& Nick, const std::string& ServerCreationDate);
		static void	sendPRIVMESS(int fd, const std::string& senderNick, const std::string& destination, const std::string& msg);
		static void	sendPONG(int fd, const std::string& host);
		static void	sendNICK(int fd, const Client& client, const std::string& newName);
		static void	sendQUIT(const Client& client);
		static void	sendQuitInfo(const Client& client, const Channel* chan);

		static void sendJOIN(const Client& client, Channel& channel);
		static void	sendJoinInfo(const Client& client, const Channel& chan);
		static void sendPART(const Client& client, const std::string& channelName, const std::string& partMsg, std::vector<Client*> chanClients);
		static void	sendTOPIC(const Client& client, const Channel* chan);
		static void	updateTOPIC(const Client& client, const Channel* chan);
		static void	sendKICK(const Client& client, const std::string& channelName, const std::string& user_kicked, std::vector<Client*> allclients, const std::string& reason);
		static void	sendMODE(const Client& client, const std::string& channelName, const std::string& mode, std::vector<Client*> chanClients);



		static void	sendCodeMsg(const Client& client, const std::string& code, const std::string& channelName, const std::string& msg);

		static void	sendCodeMsg(const Client& client, const std::string& code, const std::string& msg);
};