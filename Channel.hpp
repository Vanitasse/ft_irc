#pragma once

# include "Client.hpp"
# include "Irc.hpp"

class Channel
{
	protected:
		t_chanModes					_modes;
		std::string					_password; // mode +k
		std::size_t					_userLimit; // mode +l
		std::string					_name;
		std::string					_topic;
		std::string					_date;
		std::string					_who_topic;
		std::vector<Client*>		_operators;
		std::vector<Client*>		_chanClients;
	public:
									Channel();
									Channel(const Channel &cpy);
									~Channel();
		Channel    					&operator=(const Channel &rhs);

		const std::string&				getName() const;
		void							setName(const std::string& name);
		const std::string&				getTopic() const;
		void							setTopic(const std::string& topic, const Client& client);
		const std::string&				getPassword() const;
		const t_chanModes&				getModes() const;
		bool							enable_I(const bool i); //renvoie change/non change
		bool							enable_L(const std::size_t limit); //renvoie change/non change
		bool							enable_T(const bool t); //renvoie change/non change
		bool							enable_K(const bool k, const std::string& password); //renvoie change/non change et set le password

		const std::string&				getDate() const;

		const std::vector<Client*>&		getOperators() const;
		const std::vector<Client*>&		getChanClients() const;

		const std::string					getTopicInfo() const;
		const std::string					getNickList();
		bool 								IsAnOp(const std::string nickname);
		bool 								IsInChan(const std::string nickname);



		void								addClient(Client *client);
		void								addOperator(Client *client);
		void								removeClient(const Client *client);
		void								removeClient(const std::string& nick);
		void								removeOP(const Client& client);

	
		void								sendToClients(const Client& sender, const std::string& msg);
};