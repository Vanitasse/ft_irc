/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:20:22 by vanitas           #+#    #+#             */
/*   Updated: 2024/05/28 13:54:08 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Irc.hpp"


class	Client
{
	private:
		int			fd;
		std::string	channel;
		std::string	ip_addr;
		std::string	username;
		State		state;
	
	public:
		Client();
		Client(const Client &cpy);
		Client	&operator=(const Client &rhs);
		~Client();
	
		int		getFd() const;
		void	setFd(int fd_input);
	
		const	std::string& getIp() const;
		void	setIp(int ip_input);
	
		const	std::string& getUsername() const;
		void	setUsername(std::string username_input);
	
		const	std::string& getChannel() const;
		void	setChannel(std::string channel_input);
	
		const	State& getState() const;
		void	setState(const State& state_input);
		
		void	loginRecv(std::string input);
		void	passwordRecv(std::string input, const std::string& password);
		void	connectedRecv(std::string input, std::vector<Client> clients);

		void	printPrompt(std::string prompt);
};