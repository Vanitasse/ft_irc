/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:12:38 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/27 12:48:20 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Irc.hpp"

int	main(int ac, char **av)
{
	if (ac == 3)
	{
		Server server(av[1], av[2]);
		try
		{
			signal(SIGINT, Server::signalHandler);
			
			signal(SIGQUIT, Server::signalHandler);
			server.serverInit();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return (0);
}