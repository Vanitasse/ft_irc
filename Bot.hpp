#pragma once

# include "Channel.hpp"

enum	e_LGphase
{
	Day,
	Night,
	Witch,
	
};
class	Bot : public Channel
{
	private:
	public:
		Bot();
		~Bot();
		Bot(const Bot& toCpy);
		Bot&	operator=(const Bot& toCpy);
		void	addClient(Client *client);
		void	parseMsg(const Client* client, const std::string& msg);

		void	sender(int fd, const std::string& format);
		void	help(const Client* client);
		void	joke(const Client* client);



};

typedef void (Bot::*t_ptr) (const Client* client);
