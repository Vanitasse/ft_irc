#pragma once

# include <iostream>
# include "Irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Bot.hpp"


class	Server
{
	private:
		const std::string			_password;
		int							_port;

		static bool					_signal;
	
		std::vector<struct pollfd>	_pfds;
		std::vector<Client>			_Clients;
		std::vector<Channel>		_Channels;
	
		void						serverInit();
		void						serverExec();
		void						connectClient();
		void						readData(Client& client);
		int							ServerRecv(int fd);
		
		std::string					_receivedBuffer;
		std::string					_date;

	public:
							Server();
							~Server();
							Server(const int& port, const std::string& password);
							Server(const Server& toCpy);
		Server&				operator=(const Server& toCpy);

		const std::string&		getPassword() const;

		static void					signalHandler(int signum);

		void						clearClient(Client& client);

		void						run();

		void						getServerCreationTime() ;
		const std::string&			getDate() const ;

		int							newNickAccess(const std::string& nickname);
		Channel&					newChannelAccess(const std::string& chanName);
		
};