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
		static void	sendNICK(int fd, const std::string& client_nick, const std::string& client_username, const std::string& newName);
		static void	sendQUIT(int fd, const std::string& client_nick, const std::string& client_username);

		static void sendJOIN(const Client& client, Channel& channel, const std::string& domain);
		static void sendPART(const Client& client, const std::string& channelName, const std::string& partMsg);
};