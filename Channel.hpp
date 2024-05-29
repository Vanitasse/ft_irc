/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:33:24 by mablatie          #+#    #+#             */
/*   Updated: 2024/05/29 10:11:31 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Client.hpp"
#include "Irc.hpp"

class Client;	

class Channel
{
	private:
		std::string	name;
		Client 		*operateur;
	public:
		Channel();
		Channel(const Channel &cpy);
		~Channel();
		Channel    &operator=(const Channel &rhs);

		const std::string&	getName() const;
		void				setName(std::string &input);
		Client*				getOperateur() const;
		void				setOperateur(Client* client);
};