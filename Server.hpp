/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:43:27 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/28 16:46:15 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class	Server
{
	public:
		Server();
		~Server();
		Server(const Server& toCpy);
		Server&	operator=(const Server& toCpy);
		Server(const std::string& port, const std::string& password);
	
		void						serverInit();
		void						createChannel(std::string input, Client *client);
		static void					signalHandler(int signum);

		int		handleCommands(std::string buffer, Client& client);
		void	JOIN(std::string buffer, Client& client);
		void	KICK(std::string buffer, Client& client);
		void	INVITE();
		void	TOPIC();
		void	MODE();
	
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
		std::vector<Channel>		channels;
};