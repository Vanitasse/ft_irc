/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrssiClient.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:06:30 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/31 15:27:28 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "Irc.hpp"
# include "Client.hpp"

class	IrssiClient : public Client
{
	public:
		IrssiClient();
		IrssiClient(const IrssiClient &cpy);
		IrssiClient	&operator=(const IrssiClient &rhs);
		~IrssiClient();
		void	parseInput(std::string input);
};