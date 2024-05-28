/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:43:27 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/28 10:48:29 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Irc.hpp"
# include "Client.hpp"

class	Server
{
	public:
		Server();
		~Server();
		Server(const Server& toCpy);
		Server&	operator=(const Server& toCpy);
		Server(const std::string& port, const std::string& password);
	
		void						serverInit();
		static void					signalHandler(int signum);
	
	private:
		static bool					signal;

		int							port;
		std::string					password;
	
		void						serverExec();
		void						closeFds();
		void						connectClient();
		void						readData(Client& client);
		void						clearClient(int fd);

		int							serv_sock_fd;
	
		std::vector<struct pollfd>	poll_fds;
		std::vector<Client> 		clients;
};