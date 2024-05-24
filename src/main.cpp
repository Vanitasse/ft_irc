/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vanitas <vanitas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:12:38 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/22 17:45:10 by vanitas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Irc.hpp"

int	main(int ac, char **av)
{
	if (ac == 3)
	{
		try
		{
			signal(SIGINT, Server::signalHandler);
			signal(SIGQUIT, Server::signalHandler);
			Server server(av[1], av[2]);
		}
		catch(const std::exception& e)
		{
			server.closeFds();
			std::cerr << e.what() << '\n';
		}
		

	}
	return (0);
}