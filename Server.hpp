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

		std::string					_domain;
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

		Channel*					createNewChannel(const std::string& chanName);
	public:
							Server();
							~Server();
							Server(const int& port, const std::string& password);
							Server(const Server& toCpy);
		Server&				operator=(const Server& toCpy);

		const std::string&		getPassword() const;
		const t_limits&			getLimits() const;

		static void					signalHandler(int signum);

		void						clearClient(Client* client);

		void						run();

		void						getServerCreationTime() ;
		const std::string&			getDate() const ;
		const std::string&			getDomain() const ;
		const std::vector<Channel*>&	getChannels() const;

		int							checkNicks(const std::string& nickname);
		Channel*					checkChannels(const std::string& chanName);
		
};