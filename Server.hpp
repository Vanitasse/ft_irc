/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:43:27 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/24 18:45:19 by mablatie         ###   ########.fr       */
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
		~Server();
		Server(const std::string& port, const std::string& password);
		void serverInit();
		void serverExec();
		static void signalHandler(int signum);
		void closeFds();
		std::vector<struct pollfd> poll_fds;
		std::vector<Client> clients;
		std::vector<Client> LOL;
		std::vector<Client> DOTA;
		void connectClient();
		void readData(int fd);
		void clearClient(int fd);
		void PromptName(Client& cli);
	private:
		Server();
		Server(const Server& toCpy);
		Server&	operator=(const Server& toCpy);
		int port;
		std::string password;
		int serv_sock_fd;
		static bool signal;
		
};