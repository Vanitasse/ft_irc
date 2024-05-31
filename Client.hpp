/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:20:22 by vanitas           #+#    #+#             */
/*   Updated: 2024/05/31 18:05:50 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Irc.hpp"
# include "Channel.hpp"

class	Client
{
	protected:
		int			_fd;
		std::string	_channel;
		std::string	_pass;
		std::string	_ip_addr;
		std::string	_username;
		std::string	_nickname;
		State		_state;
	
	public:
		Client();
		Client(const Client &cpy);
		Client	&operator=(const Client &rhs);
		virtual ~Client();
	
		int				getFd() const;
		void			setFd(int fd_input);
	
		const			std::string& getIp() const;
		void			setIp(int ip_input);
	
		const			std::string& getUsername() const;
		void			setUsername(std::string username_input);
	
		const			std::string& getChannel() const;
		void			setChannel(std::string channel_input);
	
		const			State& getState() const;
		void			setState(const State& state_input);

		const			std::string& getPass() const;
		void			setPass(std::string pass_input);

		void			smiley(std::string& input);
		virtual void	parseInput(std::string input) = 0;
};