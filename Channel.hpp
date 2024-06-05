#pragma once

# include "Client.hpp"
# include "Irc.hpp"

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::string			_date;
		std::string			_who_topic;
		std::vector<Client*> _operators;
		std::vector<Client*> _chanClients;
	public:
									Channel();
									Channel(const Channel &cpy);
									~Channel();
		Channel    					&operator=(const Channel &rhs);

		const std::string&				getName() const;
		void							setName(const std::string& name);
		const std::string&				getTopic() const;
		void							setTopic(const std::string& topic, const Client& client);

		const std::vector<Client*>&		getOperators() const;
		const std::vector<Client*>& 	getChanClients() const;

		const std::string					getTopicInfo() const;
		const std::string					getNickList();


		void								addClient(Client& client);
		void								sendToClients(const Client& sender, const std::string& msg);
};