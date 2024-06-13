#pragma once

# include "Client.hpp"
# include "Irc.hpp"

class Channel
{
	private:
		t_chanModes			_modes;
		std::string			_password; // mode +k
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
		// const std::string&				getPassword() const;
		const t_chanModes&				getModes() const;
		bool							setI(const bool i); //renvoie change/non change
		bool							setT(const bool t); //renvoie change/non change
		bool							setK(const bool k, const std::string& password); //renvoie change/non change et set le password
		bool							setO(const bool o); //renvoie change/non change
		bool							setL(const bool l); //renvoie change/non change

		const std::string&				getDate() const;

		// const std::vector<Client*>&		getOperators() const;
		const std::vector<Client*>&		getChanClients() const;

		const std::string					getTopicInfo() const;
		const std::string					getNickList();
		bool 								listOPs(const std::string nickname);



		void								addClient(Client *client);
		void								removeClient(const Client *client);
		void								removeClient(const std::string& nick);
		void								removeOP(const Client& client);

	
		void								sendToClients(const Client& sender, const std::string& msg);
};