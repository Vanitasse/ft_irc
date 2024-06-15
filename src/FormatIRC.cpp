#include "../FormatIRC.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

std::string FormatIRC::_domain = "42IRC";

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

void	FormatIRC::sender(int fd, const std::string& format)
{
	const std::string res(format + "\r\n");

	send(fd, res.c_str(), res.length(), 0);
	std::cout << CYAN << "[Server send]" << res << RESET << std::endl;
}


void	FormatIRC::sendPRIVMESS(int fd, const std::string& senderNick, const std::string& destination, const std::string& msg)
{
	const std::string format(':' + senderNick + " PRIVMSG" + ' ' + destination + " :" + msg);

	sender(fd, format);
}

void	FormatIRC::sendWelcome(int fd, const std::string& Nick, const std::string& ServerCreationDate)
{
	const std::string format(msg_serv(std::string("001"), Nick) + "Welcome to the IRC server\r\n"
				+ msg_serv(std::string("002"), Nick) + GREEN + "Your host is 42IRCserver, running version 0.1\r\n"
				+ msg_serv(std::string("003"), Nick) + GREEN + "This server was created " + ServerCreationDate + "\r\n"
				+ msg_serv2(std::string("004"), Nick) + GREEN + "42IRCserver 0.1 connected \r\n"
				+ msg_serv3(std::string("005"), Nick, supp_info + RESET));
	
	sender(fd, format);
}

void	FormatIRC::sendPONG(int fd, const std::string& host)
{
	const std::string format("PONG :" + host);

	sender(fd, format);
}

void	FormatIRC::sendPART(const Client& client, const std::string& channelName, const std::string& partMsg, std::vector<Client*> chanClients)
{
	const std::string format(":" + client.getNick() + "!~" + client.getUsername() + "@" + _domain + " PART " + channelName + " :" + partMsg);

	for (std::vector<Client*>::iterator it = chanClients.begin(); it != chanClients.end(); it++)
		sender((*it)->getFd(), format);
}

void	FormatIRC::sendNICK(int fd, const Client& client, const std::string& newName)
{
	const std::string format(user_id(client.getNick(), client.getUsername()) + " NICK :" + newName);

	sender(fd, format);
}

void	FormatIRC::sendJoinInfo(const Client& client, const Channel& chan)
{
	std::vector<Client*> clients;

	clients = chan.getChanClients();
	if (clients.size() == 1)
		return ;
	else
	{
		std::string format = user_id(client.getNick(), client.getUsername()) + " JOIN :" + chan.getName();
		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it)->getNick() != client.getNick())
				sender((*it)->getFd(), format);
		}
	}
}

void	FormatIRC::sendJOIN(const Client& client, Channel& channel)
{
	std::string format(":" + client.getNick() + "!~" + client.getUsername() +  " JOIN :" + channel.getName());
	sender(client.getFd(), format);

	format = msg_serv2(std::string("329"), client.getNick()) + channel.getName() + " " + channel.getDate();
	sender(client.getFd(), format);
	format = msg_serv2(std::string("332"), client.getNick()) + channel.getName() + " :" + channel.getTopic();
	sender(client.getFd(), format);
	format = msg_serv2(std::string("333"), client.getNick()) + channel.getName() + " " + channel.getTopicInfo();
	sender(client.getFd(), format);
	format = msg_serv2(std::string("353"), client.getNick()) + "= " + channel.getName() + " :" + channel.getNickList();
	sender(client.getFd(), format);
	format = msg_serv2(std::string("366"), client.getNick()) + channel.getName() + " :End of /NAMES list\r\n";
	sender(client.getFd(), format);
	FormatIRC::sendJoinInfo(client, channel);

}

void	FormatIRC::sendTOPIC(const Client& client, const Channel* chan)
{
	std::string format(msg_serv2(std::string("332"), client.getNick()) + chan->getName() + " :" + chan->getTopic());

	sender(client.getFd(), format);
	format = msg_serv2(std::string("333"), client.getNick()) + chan->getName() + " " + chan->getTopicInfo();
	sender(client.getFd(), format);
}

void	FormatIRC::updateTOPIC(const Client& client, const Channel* chan)
{
	std::vector<Client*> allclients;
	std::string format(user_id(client.getNick(), "~" + client.getUsername()) + " TOPIC " + chan->getName() + " :" + chan->getTopic());

	allclients = chan->getChanClients();
	for (std::vector<Client*>::iterator it = allclients.begin(); it != allclients.end(); it++)
		sender((*it)->getFd(), format);
}

void	FormatIRC::sendQuitInfo(const Client& client, const Channel* chan)
{
	std::vector<Client*> clients;
	
	clients = chan->getChanClients();
	if (clients.size() == 1)
		return ;
	else
	{
		std::string format = user_id(client.getNick(), client.getUsername()) + " QUIT :by using /quit";
		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it)->getNick() != client.getNick())
				sender((*it)->getFd(), format);
		}
	}
}

void	FormatIRC::sendQUIT(const Client& client)
{
	std::vector<Channel*> allchan;

	allchan = client.getInChannels();
	for (std::vector<Channel*>::iterator it = allchan.begin(); it != allchan.end(); it++)
		sendQuitInfo(client, *it);





	// std::vector<Client*> clients = chan.getChanClients();
	// if (clients.size() == 1)
	// 	return ;
	// else
	// {
	// 	std::string format = user_id(client.getNick(), client.getUsername()) + " JOIN :" + chan.getName();
	// 	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	// 	{
	// 		if ((*it)->getNick() != client.getNick())
	// 			sender((*it)->getFd(), format);
	// 	}
	// }




	// const std::string format(user_id(client.getNick(), client.getUsername()) + " QUIT :BYE BYE");
	// sender(client.getFd(), format);
}

// >> :max!~mablatie@B2ED245D.B270E442.5F584402.IP QUIT :Quit:: leaving

void	FormatIRC::sendKICK(const Client& client, const std::string& channelName, const std::string& user_kicked, std::vector<Client*> allclients, const std::string& reason)
{
	const std::string format(user_id(client.getNick(), client.getUsername()) + " KICK " + channelName + " " + user_kicked + " :" + reason);

	for (std::vector<Client*>::iterator it = allclients.begin(); it != allclients.end(); it++)
		sender((*it)->getFd(), format);
}

void	FormatIRC::sendMODE(const Client& client, const std::string& channelName, const std::string& mode, std::vector<Client*> chanClients)
{
	const std::string format(":" + client.getNick() + "!~" + client.getUsername() + "@" + _domain + " MODE " + channelName + " " + mode);

	for (std::vector<Client*>::iterator it = chanClients.begin(); it != chanClients.end(); it++)
		sender((*it)->getFd(), format);
}

void FormatIRC::sendNOTICE(const Client& noticer, const Client& noticed, const std::string& notice)
{
	const std::string format(":" + noticer.getNick() + "!~" + noticer.getUsername() + '@' + _domain + " NOTICE " + noticed.getNick() + " :" + notice);

	sender(noticed.getFd(), format);
}

void FormatIRC::sendINVITE(const Client& inviter, const Client& invited, const std::string& channelName,  std::vector<Client*> operators)
{
	const std::string format(":" + inviter.getNick() + "!~" + inviter.getUsername() + "@" + _domain + " INVITE " + invited.getNick() + " :" + channelName);

	const std::string format2(":" + _domain + " NOTICE @" + channelName + " :" + inviter.getNick() + " invited " + invited.getNick() + " into this channel");

	sender(invited.getFd(), format);
	for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); it++)
		sender((*it)->getFd(), format2);
}
void	FormatIRC::sendCodeMsg(const Client& client, const std::string& code, const std::string& channelName, const std::string& msg)
{
	const std::string format(":" + _domain + " " + code + " " + client.getNick() + " " + channelName + " :" + msg);
	sender(client.getFd(), format);
}

void	FormatIRC::sendCodeMsg(const Client& client, const std::string& code, const std::string& msg)
{
	const std::string format(":" + _domain + " " + code + " " + client.getNick() + " :" + msg);
	sender(client.getFd(), format);
}