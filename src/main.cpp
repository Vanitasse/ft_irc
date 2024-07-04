#include "../Server.hpp"
#include "../Client.hpp"

int	main(int ac, char **av)
{
	if (ac == 3)
	{
		Server server(std::atoi(av[1]), av[2]);
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
