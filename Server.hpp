#pragma once

# include <iostream>
# include "Irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Bot.hpp"

class	Server
{
	private:
		t_limits					_limits;
		const	std::string			_password;
		int							_port;

		static bool					_signal;
	
		std::vector<struct pollfd>	_pfds;
		std::vector<Client*>		_Clients;
		std::vector<Channel*>		_Channels;
	
		void						serverInit();
		void						serverExec();
		void						connectClient();
		void						readData(Client* client);
		int							ServerRecv(int fd);
		
		std::string					_receivedBuffer;
		std::string					_date;

		Channel*					createNewChannel(const std::string& chanName, Client& client);
	public:
							Server();
							~Server();
							Server(const int& port, const std::string& password);
							Server(const Server& toCpy);
		Server&				operator=(const Server& toCpy);

		const std::string&				getPassword() const;
		const t_limits&					getLimits() const;
		void							getServerCreationTime() ;
		const std::string&				getDate() const ;
		const std::vector<Channel*>&	getChannels() const;

		void						run();
		
		static void					signalHandler(int signum);

		void						clearClient(Client* client);


	
		Client*						findClient(const std::string& nick);
		Channel*					findChannel(const std::string& channelName);


		int							checkNicks(const std::string& nickname);
		Channel*					checkChannels(const std::string& chanName, Client& client);

		bool 						checkOPs(const std::string& nickname, const std::string& chanName);
		std::vector<std::string>	splitUsernames(const std::string& usernames);

		// std::vector<Client*> 		checkUsers(const std::string& user_kicked);




		void						TOPIC(const Client& client, const std::string& channelName);
		void						TOPIC(const Client& client, const std::string& param, const std::string& param_2, const std::string& topic);
};