#pragma once

# include "Irc.hpp"

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
	public:
					Client();
					Client(Server& server);
					Client(const Client& cpy);
		Client		&operator=(const Client &rhs);
		virtual		~Client();
	
		int					getFd() const;
		void				setFd(int fd_input);
	
		const std::string&	getIp() const;
		void				setIp(int ip_input);
	
		const std::string&	getUsername() const;
		void				setUsername(const std::string& username_input);
	
		const std::vector<Channel*>&	getInChannels() const;

		const std::string&	getNick() const;
		void				setNick(const std::string& nick_input);
		
		const std::string&	getRealname() const;
		void				setRealname(const std::string& realname_input);

		const bool& 		getPassIsSet() const;
		void 				setPassIsSet(const bool& passIsSet_input);

		const bool& 		getNickIsSet() const;
		void 				setNickIsSet(const bool& nickIsSet_input);

		const bool& 		getIsConnected() const;
		void 				setIsConnected(const bool& isConnected_input);

		void				setOPChannels(Channel* chan);

		void							smiley(std::string& input);

		static std::vector<std::string>	splitInput(const std::string& input);
		
		void							ParseAndRespond(std::string& input);
		void							NICK(const std::string& newName);
		void							PASS(const std::string& pass);
		void							JOIN(const std::string& chanName);

		void							PRIVMSG(const std::string& destination, const std::string& msg);
		void							WHO(const std::string& channelName);
		void							PART(const std::string& channelName, const std::string& partMsg);
		void							KICK(const std::string & chanName, const std::string& user_kicked, const std::string& reason);

		void							QUIT();

		// virtual void		formatText(std::string& input) = 0;
};