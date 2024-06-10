#include "../Server.hpp"

bool Server::_signal = false;

Server::Server()
{
	_domain = "42IRCserver";
	_limits._chanType = "#";
	_limits._prefix = "(o)@";
	_limits._nbModes = 1;
	_limits._nickLen = 12;
	_limits._topicLen = 200;
}

Server::~Server()
{
	for (std::vector<Client*>::iterator	it = _Clients.begin(); it != _Clients.end(); it++)
		delete (*it);
	for (std::vector<Channel*>::iterator	it = _Channels.begin(); it != _Channels.end(); it++)
		delete (*it);
	for (std::vector<struct pollfd>::iterator	it = _pfds.begin(); it != _pfds.end(); it++)
		close (it->fd);
	std::cout << RED << "Server destructed" << RESET << std::endl;
}

Server::Server(const Server& toCpy) : _password(toCpy._password)
{
	*this = toCpy;
}

Server&	Server::operator=(const Server& toCpy)
{
	if (this != &toCpy)
	{
		_domain = toCpy._domain;
		_Clients = toCpy._Clients;
		_pfds = toCpy._pfds;
		_port = toCpy._port;
	}
	return (*this);
}

Server::Server(const int& port, const std::string& password_input) : _password(password_input), _port(port)
{
	_domain = "42IRCserver";
	_limits._chanType = "#";
	_limits._channelLen = 20;
	_limits._prefix = "(o)@";
	_limits._nbModes = 1;
	_limits._nickLen = 15;
	_limits._topicLen = 200;
}

const std::string& Server::getDate() const
{
	return this->_date;
}

const std::string&	Server::getPassword() const
{
	return (_password);
}

const std::string&	Server::getDomain() const
{
	return (_domain);
}

const t_limits&	Server::getLimits() const
{
	return (_limits);
}

const std::vector<Channel*>& Server::getChannels() const
{
	return (this->_Channels);
}

void	Server::serverInit()
{
	struct pollfd		new_poll= {};
	struct sockaddr_in	sock_addr; //structure pour l'adresse internet
	int uh = 1; // necessaire car parametre opt_value de setsockopt() = const void *

	sock_addr.sin_family = AF_INET; //on set le type en IPV4
	sock_addr.sin_port = htons(_port); // converti le port(int) en big endian (pour le network byte order)
	sock_addr.sin_addr.s_addr = INADDR_ANY; // IMADDR_ANY = n'importe quel adresse donc full local

	new_poll.events = POLLIN; //le poll doit lire des infos;
	new_poll.revents = 0; //0 events actuellement donc par defaut;
	new_poll.fd = socket(AF_INET, SOCK_STREAM, 0); //socket creation
	if (new_poll.fd == -1)
		throw(std::runtime_error("socket creation failed"));
	_pfds.push_back(new_poll); 
	if (setsockopt(new_poll.fd, SOL_SOCKET, SO_REUSEADDR, &uh, sizeof(uh)) == - 1) //met l'option SO_REUSEADDR sur la socket = permet de reutiliser l'addresse
		throw(std::runtime_error("set option (SO_REUSEADDR) failed"));
	if (fcntl(new_poll.fd, F_SETFL, O_NONBLOCK) == -1) //met l'option O_NONBLOCK pour faire une socket non bloquante
		throw(std::runtime_error("set option (O_NONBLOCK) failed")); //lie la socket a l'adresse reutilisable (obliger de cast pour avoir toute les infos)
	if (bind(new_poll.fd, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) == -1)
		throw(std::runtime_error("socket binding failed"));
	if (listen(new_poll.fd, SOMAXCONN) == -1) //la socket devient passive = c'est une socket serveur qui attend des connections (SOMAXCONN = max_connections autorise)
		throw(std::runtime_error("listen() failed"));
	getServerCreationTime();
}


void	Server::run()
{
	serverInit();
	serverExec();
}



void	Server::TOPIC_1(const Client& client, const std::string& channelName)
{
	std::cout << "TOPIC_1" << std::endl;

	for (std::vector<Channel*>::iterator it = _Channels.begin(); it < _Channels.end(); it++)
	{
		if ((*it)->getName() == channelName)
			FormatIRC::sendTOPIC(client, *it);
	}
}

void	Server::TOPIC_2(const Client& client, const std::string& param, const std::string param_2)
{
	if (param == "-delete")
	{
		if (!checkOPs(client.getNick(), param_2))
				return;
		for (std::vector<Channel*>::iterator it = _Channels.begin(); it < _Channels.end(); it++)
		{
			if ((*it)->getName() == param_2)
			{
				(*it)->setTopic("", client);
				FormatIRC::updateTOPIC(client, *it);
			}
		}
	}
	else
	{
		if (!checkOPs(client.getNick(), param))
				return;
		for (std::vector<Channel*>::iterator it = _Channels.begin(); it < _Channels.end(); it++)
		{
			if ((*it)->getName() == param)
			{
				(*it)->setTopic(param_2, client);
				FormatIRC::updateTOPIC(client, *it);
			}
		}
	}
}

void	Server::KICK(const Client& client, const std::string& channelName, const std::string& user_kicked, const std::string& reason)
{
	if (!checkOPs(client.getNick(), channelName))
		return;
	std::vector<std::string> users = splitUsernames(user_kicked);

	for(std::vector<Channel*>::iterator it = _Channels.begin(); it < _Channels.end(); it++)
	{
		if ((*it)->getName() == channelName)
		{
			for(std::vector<std::string>::iterator its = users.begin(); its < users.end(); its++)
			{
				// if (chan_users[i] == (*its))
				// 	(*it)->removeClient((*its));
			}
		}
	}
	(void)reason;


	FormatIRC::sendKICK(client, channelName, users);
}


void	Server::KICK(const Client& client, const std::string& channelName, const std::string& user_kicked)
{
	if (!checkOPs(client.getNick(), channelName))
		return;
	(void)user_kicked;
}

// KICK [<channel>] <nicks> [<reason>]


// void	Server::KICK(const Client& client, const std::string& user_kicked)
// {
// 	if (!checkOPs(client.getNick(), client.getWhere()))
// 		return;
// 	if (!checkWhere())
// }






void	Server::connectClient()
{
	Client				*client = new Client(*this);
	struct sockaddr_in 	sock_addr;
	socklen_t 			len;
	struct pollfd		new_poll = {};

	len = sizeof(sock_addr);
	new_poll.fd = accept(_pfds[0].fd, (struct sockaddr *)&sock_addr, &len);
	if (new_poll.fd == -1)
		throw std::runtime_error("Client socket creation failed");
	if (fcntl(new_poll.fd, F_SETFL, O_NONBLOCK) == -1) //met l'option O_NONBLOCK pour faire une socket non bloquante
		throw(std::runtime_error("set option (O_NONBLOCK) failed"));
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_pfds.push_back(new_poll);
	client->setFd(new_poll.fd);
	_Clients.push_back(client);
	std:: cout << "New Client  " << _Clients.back()->getNick() << " with fd " << _Clients.back()->getFd() << std::endl;
	std::cout << "_Clients.size() = " << _Clients.size() << "\n_pfds.size() = " << _pfds.size() << std::endl;
}








int	Server::ServerRecv(int fd)
{
	char buffer[1024] = {0};
	ssize_t bytes;

	bytes = recv(fd, buffer, sizeof(buffer), 0); //MSG_WAITALL MSG_DONTWAIT MSG_PEEK MSG_TRUNC
	if (bytes <= 0)
		return (0);
	buffer[bytes] = '\0';
	_receivedBuffer = buffer;
	std::cout << CYAN << "[Server receive]" << _receivedBuffer << RESET << std::endl;
	return (1);
}

void	Server::readData(Client* client)
{
	if (!ServerRecv(client->getFd()))
		return (clearClient(client));
	client->ParseAndRespond(_receivedBuffer);
}


void Server::serverExec()
{
	while (this->_signal == false)
	{
		if (poll(&_pfds[0], _pfds.size(), -1) == -1 && this->_signal == false) //-1 = bloque ici attend un event, 0 = bloque pas tourne dans le while
			throw(std::runtime_error("poll() function failed"));
		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].revents && POLLIN)
			{
				if (i == 0)
					connectClient();
				else
					readData(_Clients[i - 1]);
			}
		}
	}
}
