#include "../FormatIRC.hpp"

FormatIRC::FormatIRC()
{

}
FormatIRC::~FormatIRC()
{
	
}
FormatIRC::FormatIRC(const FormatIRC& toCpy)
{
	(void)toCpy;
}
FormatIRC& FormatIRC::operator=(const FormatIRC& toCpy)
{
	(void)toCpy;
	return (*this);
}

void	FormatIRC::SendPRIVMESS(int fd, const std::string& senderNick, const std::string& destination, const std::string& msg)
{
	const std::string format(":" + senderNick + " PRIVMSG" + ' ' + destination + ' ' + msg + "\r\n");

	send(fd, format.c_str(), format.length(), 0);
	std::cout << CYAN << "[Server send]" << format << RESET << std::endl;
}

void	FormatIRC::SendWelcome(int fd, const std::string& Nick, const std::string& ServerCreationDate)
{
	const std::string format(msg_serv(std::string("001"), Nick) + "Welcome to the IRC server\r\n"
				+ msg_serv(std::string("002"), Nick) + GREEN + "Your host is 42IRCserver, running version 0.1\r\n"
				+ msg_serv(std::string("003"), Nick) + GREEN + "This server was created " + ServerCreationDate + "\r\n"
				+ msg_serv2(std::string("004"), Nick) + GREEN + "42IRCserver 0.1 connected \r\n"
				+ msg_serv3(std::string("005"), Nick, supp_info() + RESET));
	
	send(fd, format.c_str(), format.length(), 0);
	std::cout << CYAN << "[Server send]" << format << RESET << std::endl;
}

void	FormatIRC::PONG(int fd, const std::string& host)
{
	const std::string format("PONG :" + host + "\r\n");

	send(fd, format.c_str(), format.length(), 0);
	std::cout << CYAN << "[Server send]" << format << RESET << std::endl;
}
