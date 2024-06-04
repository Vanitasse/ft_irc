#pragma once

# include "Irc.hpp"

class FormatIRC
{
	private:
		FormatIRC();
		~FormatIRC();
		FormatIRC(const FormatIRC& toCpy);
		FormatIRC& operator=(const FormatIRC& toCpy);
	public:
		static void	SendWelcome(int fd, const std::string& Nick, const std::string& ServerCreationDate);
		static void	SendPRIVMESS(int fd, const std::string& senderNick, const std::string& destination, const std::string& msg);
		static void	PONG(int fd, const std::string& host);
};