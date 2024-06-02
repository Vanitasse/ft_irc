/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:43:27 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/02 02:32:18 by bvaujour         ###   ########.fr       */
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
		int							_port;
		std::vector<struct pollfd>	_pfds;
		std::vector<Client*>		_Clients;
		void						serverInit();
		void						serverExec();
		void						connectClient();
		void						clearClient(Client& client);
		static bool					_signal;
		void						readData(Client& client);
		int							ServerRecv(int fd);
		void						addIrssiClient(int fd);
		void						addNcClient(int fd);
		void						sendError(int fd, const std::string& code, const std::string& nick, const std::string& msg);
		std::string					_strBuf;
	public:
		Server();
		~Server();
		Server(const int& port, const std::string password);
		Server(const Server& toCpy);
		Server&	operator=(const Server& toCpy);
		static void					signalHandler(int signum);

		void			run();
};