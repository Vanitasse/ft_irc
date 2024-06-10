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
	public:
		static void	sender(int fd, const std::string& format);
		static void	sendWelcome(int fd, const std::string& Nick, const std::string& ServerCreationDate);
		static void	sendPRIVMESS(int fd, const std::string& senderNick, const std::string& destination, const std::string& msg);
		static void	sendPONG(int fd, const std::string& host);
		static void	sendNICK(int fd, const Client& client, const std::string& newName);
		static void	sendQUIT(int fd, const std::string& client_nick, const std::string& client_username);
		static void sendJOIN(const Client& client, Channel& channel, const std::string& domain);
		static void sendPART(const Client& client, const std::string& channelName, const std::string& partMsg);
		static void	sendTOPIC(const Client& client, const Channel* chan);
		static void	updateTOPIC(const Client& client, const Channel* chan);
		static void	sendKICK(const Client& client, const std::string& channelName, const std::vector<std::string>& users);




		static void	sendErrorChannelLen(const Client& client, const std::string& channelName, const std::string& domain);

};