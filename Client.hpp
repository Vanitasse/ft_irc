/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:20:22 by vanitas           #+#    #+#             */
/*   Updated: 2024/05/24 18:27:15 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

class Client
{
	private:
		std::string	channel;
		int nb;
		int fd;
		std::string ip_addr;
		std::string username;
	public:
		Client();
		Client(const Client &cpy);
		~Client();
		Client    &operator=(const Client &rhs);
		
		int getFd() const;
		void setFd(int fd_input);
		const std::string& getIp() const;
		void setIp(int ip_input);
		int getNb() const ;
		void setNb(int nb_input);
		const std::string& getUsername() const ;
		void setUsername(std::string user_input);
		const std::string& getChannel() const ;
		void setChannel(std::string channel_input);
};