/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:20:22 by vanitas           #+#    #+#             */
/*   Updated: 2024/06/03 16:03:55 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Irc.hpp"

class Server;

class	Client
{
	protected:
		int			_fd;
		std::string	_channel;
		std::string	_ip_addr;
		bool		_nickIsSet;
		bool		_passIsSet;
		bool		_isConnected;
		std::string	_username;
		std::string	_nick;
		std::string	_realname;
		std::string	_pass;
		// const std::string& server_password;
		std::string	_message;
		Server* 	_server;

	public:
					Client();
					Client(Server& server);
					Client(const Client& cpy);
		Client		&operator=(const Client &rhs);
					~Client();
	
		int					getFd() const;
		void				setFd(int fd_input);
	
		const std::string&	getIp() const;
		void				setIp(int ip_input);
	
		const std::string&	getUsername() const;
		void				setUsername(const std::string& username_input);
	
		const std::string&	getChannel() const;
		void				setChannel(const std::string& channel_input);

		const std::string&	getPass() const;
		void				setPass(const std::string& pass_input);

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

		void				smiley(std::string& input);

		static std::vector<std::string>	splitInput(const std::string& input);
		
		void				ParseAndRespond(std::string& input);
		void				Answer(const std::string& answer);
		// virtual void		formatText(std::string& input) = 0;
};