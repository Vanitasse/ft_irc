/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:12:38 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/02 11:22:37 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Client.hpp"

int	main(int ac, char **av)
{
	if (ac == 3)
	{
		Server server(std::stoi(av[1]), av[2]);
		try
		{
			signal(SIGINT, Server::signalHandler);
			signal(SIGQUIT, Server::signalHandler);
			server.run();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return (0);
}
