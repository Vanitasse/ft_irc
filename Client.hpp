#pragma once

# include "Irc.hpp"
# include <map>

class Server;
class Channel;


class	Client
{
	protected:
		int			_fd;
		std::string	_ip_addr;
	
		bool		_nickIsSet;
		bool		_passIsSet;
		bool		_isConnected;
	
		std::string	_username;
		std::string	_nick;
		std::string	_realname;
	
		std::string	_message;

		Server* 	_server;
		std::vector<Channel*>		_inChannels;
		std::vector<Channel*>		_OPChannels;
		std::vector<Channel*>		_InvitedAt;


		void	channelThrow(const std::string& channelName);
		void	operatorThrow(const std::string& channelName);
		class Error : public std::exception
		{
			private:
				char const* _error;
			public:
				Error(const char* error) : _error(error){}
				virtual char const* what() const throw()
				{
					return (_error);
				};
		};

		void							smiley(std::string& input);
		std::vector<std::string>		splitInput(const std::string& input);
		const std::string				PickMsg(const std::string& input);

		void							NICK(const std::string& newName);
		void							PASS(const std::string& pass);
		void							JOIN(const std::string& chanName);
		void							JOIN(const std::string& chanName, const std::string& password);
		void							PRIVMSG(const std::string& destination, const std::string& msg);
		void							MODE(const std::string& channelName, const std::string& mode);
		void							MODE(const std::string& channelName, const std::string& mode, const std::string& arg);
		void							PART(const std::string& channelName, const std::string& partMsg);
		void							KICK(const std::string & channelName, const std::string& user_kicked, const std::string& reason);
		void							TOPIC(const std::string& channelName, const std::string& topic);void							TOPIC(const std::string& channelName);
		void							INVITE(const std::string& nick, const std::string& channelName);
		void							QUIT();
	public:
					Client();
					Client(Server& server);
					Client(const Client& cpy);
		Client		&operator=(const Client &rhs);
		virtual		~Client();
	
		int								getFd() const;
		void							setFd(int fd_input);
	
		const std::string&				getIp() const;
		void							setIp(int ip_input);
	
		const std::string&				getUsername() const;
		void							setUsername(const std::string& username_input);
	
		const std::vector<Channel*>&	getInChannels() const;

		const std::string&				getNick() const;
		void							setNick(const std::string& nick_input);
		
		const std::string&				getRealname() const;
		void							setRealname(const std::string& realname_input);

		const bool& 					getPassIsSet() const;
		void 							setPassIsSet(const bool& passIsSet_input);

		const bool& 					getNickIsSet() const;
		void 							setNickIsSet(const bool& nickIsSet_input);

		const bool& 					getIsConnected() const;
		void 							setIsConnected(const bool& isConnected_input);

		void							setOPChannels(Channel* chan);
		
		void							beInvited(Channel *channel);
		void	ParseAndRespond(std::string& input);
};