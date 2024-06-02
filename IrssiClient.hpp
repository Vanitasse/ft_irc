/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrssiClient.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:06:30 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/02 01:28:39 by bvaujour         ###   ########.fr       */
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
		IrssiClient(const std::string& input, int fd);
		void	ParseAndRespond(std::string input);

};