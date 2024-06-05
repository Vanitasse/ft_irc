#include "../Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(const Channel& cpy)
{
	*this = cpy;
}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel& rhs)
{
	if (this != &rhs)
	{
		this->_name = rhs.getName();
		this->_topic = rhs.getTopic();
		this->_operators = rhs._operators;
		this->_chanClients = rhs._chanClients;
	}
	return *this;
}

const std::string& Channel::getName() const
{
	return (this->_name);
}

void Channel::setName(const std::string& name)
{
	this->_name = name;
}

const std::string& Channel::getTopic() const
{
	return (this->_topic);
}

void Channel::setTopic(const std::string& topic, const Client& client)
{
	this->_topic = topic;
	this->_who_topic = client.getNick();
}

const std::vector<Client*>& Channel::getOperators() const
{
	return (this->_operators);
}

const std::vector<Client*>& Channel::getChanClients() const
{
	return (this->_chanClients);
}

void	Channel::addClient(Client& client)
{
	if (_operators.size() == 0)
	{
		_operators.push_back(&client);
		std::cout << BG_BLUE << "client " << client.getNick() << " added to channel " << this->_name << " as Operator" << RESET << std::endl;
	}
	else
	{
		_chanClients.push_back(&client);
		std::cout<<  BG_GREEN << "client " << client.getNick() << " added to channel " << this->_name << " as Simple user" << RESET << std::endl;
	}
}

void	Channel::sendToClients(const Client& sender, const std::string& msg)
{
	for (auto& client : _operators)
		if (client->getFd() != sender.getFd())
			FormatIRC::SendPRIVMESS(client->getFd(), sender.getNick(), this->_name, msg);
	for (auto& client : _chanClients)
		if (client->getFd() != sender.getFd())
			FormatIRC::SendPRIVMESS(client->getFd(), sender.getNick(), this->_name, msg);
}

const std::string Channel::getNickList()
{
	std::string res;
	for (std::vector<Client*>::iterator it = _chanClients.begin(); it < _chanClients.end(); it++)
	{
		res += (*it)->getNick() + " ";
	}
	return res;
}

const std::string Channel::getTopicInfo() const
{
	const std::string format = "[" + this->_who_topic + "] [" + this->_date + "]";
	return format;
}


void 	Channel::getTopicTime()
{
	// Obtenir l'heure actuelle
	std::time_t now = std::time(nullptr);
	// Convertir l'heure en structure tm pour le formatage
	std::tm* now_tm = std::localtime(&now);
	
	// Créer une chaîne de caractères pour la date et l'heure formatées
	char buffer[100] = {0};
	std::strftime(buffer, sizeof(buffer), "%a %b %e %H:%M:%S %Y", now_tm);
	
	// Retourner la chaîne formatée
	this->_date = buffer;
}

// Thu Jan  1 01:33:44 1970