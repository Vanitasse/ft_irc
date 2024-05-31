/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:43:27 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/31 16:35:44 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include "Irc.hpp"
# include "IrssiClient.hpp"
# include "NcClient.hpp"

class	Server
{
	private:
		const std::string			_password;
		std::vector<struct pollfd>	_pfds;
		std::vector<Client*>		_Clients;
		void						serverExec();
		void						connectClient();
		void						clearClient(Client& client);
		static bool					_signal;
		void						readData(Client& client);
		int							ServerRecv(int fd);
		void						ServerSend(Client& Sender);
		void						addIrssiClient(int fd);
		void						addNcClient(int fd);

		std::string					_strBuf;
	public:
		Server();
		~Server();
		Server(const int& port, const std::string password);
		Server(const Server& toCpy);
		Server&	operator=(const Server& toCpy);
		static void					signalHandler(int signum);
};