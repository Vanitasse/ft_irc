/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:20:22 by vanitas           #+#    #+#             */
/*   Updated: 2024/06/02 16:21:56 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Irc.hpp"

class	Client
{
	protected:
		int			_fd;
		std::string	_channel;
		std::string	_ip_addr;
		State		_state;

		std::string	_username;
		std::string	_nick;
		std::string	_realname;
		std::string	_pass;

		std::string	_message;
	public:
					Client();
					Client(const Client& cpy);
		Client		&operator=(const Client &rhs);
		virtual		~Client();
	
		int				getFd() const;
		void			setFd(int fd_input);
	
		const			std::string& getIp() const;
		void			setIp(int ip_input);
	
		const			std::string& getUsername() const;
		void			setUsername(const std::string& username_input);
	
		const			std::string& getChannel() const;
		void			setChannel(const std::string& channel_input);
	
		const			State& getState() const;
		void			setState(const State& state_input);

		const			std::string& getPass() const;
		void			setPass(const std::string& pass_input);

		const			std::string& getNick() const;
		void			setNick(const std::string& nick_input);
		
		const			std::string& getRealname() const;
		void			setRealname(const std::string& realname_input);

		void			smiley(std::string& input);

		static std::vector<std::string>	splitInput(const std::string& input);
		
		virtual Destination				ParseAndRespond(std::string& input) = 0;
		virtual void					formatText(std::string& input) = 0;
};