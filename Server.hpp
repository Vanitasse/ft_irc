/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:43:27 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/27 16:29:00 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <sys/socket.h> //-> for socket()
# include <sys/types.h> //-> for socket() options
# include <netinet/in.h> //-> for sockaddr_in
# include <fcntl.h> //-> for fcntl()
# include <unistd.h> //-> for close()
# include <arpa/inet.h> //-> for inet_ntoa()
# include <poll.h> //-> for poll()
# include <csignal> //-> for signal()
# include <string.h>
# include "Client.hpp"
# include <sstream>

class	Server
{
	public:
		Server();
		~Server();
		Server(const Server& toCpy);
		Server&	operator=(const Server& toCpy);
		Server(const std::string& port, const std::string& password);
	
		void serverInit();
		static void signalHandler(int signum);
	
	private:
		int port;
		std::string password;
		void serverExec();
		void closeFds();
		void connectClient();
		void readData(Client& client);
		void clearClient(int fd);
		static bool signal;
		int serv_sock_fd;
		std::vector<struct pollfd> poll_fds;
		std::vector<Client> clients;
};