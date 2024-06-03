/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:43:27 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/03 17:21:40 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include "Irc.hpp"
# include "Client.hpp"

class	Server
{
	private:
		const std::string			_password;
		int							_port;

		static bool					_signal;
	
		std::vector<struct pollfd>	_pfds;
		std::vector<Client>			_Clients;
	
		void						serverInit();
		void						serverExec();
		void						connectClient();
		void						readData(Client& client);
		int							ServerRecv(int fd);
		void						sendWithCode(const Client& client, const std::string& code, const std::string& msg, const std::string& color) const;
		void						sendBasic(const Client& client, const std::string& msg, const std::string& color) const;
		
		std::string					_receivedBuffer;
		std::string					_date;

	public:
								Server();
								~Server();
								Server(const int& port, const std::string& password);
								Server(const Server& toCpy);
		Server&					operator=(const Server& toCpy);

		static void					signalHandler(int signum);

		void						clearClient(Client& client);

		void						run();

		void					detailString(const std::string& str)const ; //debug
		void					getServerCreationTime() ;
		const std::string&		getDate() const ;

		int						checkAllNicknames(const std::string& nickname);
};