#pragma once

# include "Irc.hpp"
// # include "Channel.hpp"

// class Client;
// class Channel;

class FormatIRC
{
	private:
		FormatIRC();
		~FormatIRC();
		FormatIRC(const FormatIRC& toCpy);
		FormatIRC& operator=(const FormatIRC& toCpy);
	public:
		static void	sender(int fd, const std::string& format);
		static void	SendWelcome(int fd, const std::string& Nick, const std::string& ServerCreationDate);
		static void	SendPRIVMESS(int fd, const std::string& senderNick, const std::string& destination, const std::string& msg);
		static void	PONG(int fd, const std::string& host);
		static void	NICK(int fd, const std::string& client_nick, const std::string& client_username, const std::string& newName);
		static void JOIN(int fd, const std::string& client_nick, const std::string& client_username,
						const std::string& chan_name, const std::string& topic,
						const std::string& topic_info, const std::string& nick_list);


};