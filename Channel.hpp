/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:33:24 by mablatie          #+#    #+#             */
/*   Updated: 2024/05/28 15:29:20 by mablatie         ###   ########.fr       */
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
		Client*				getOperateur();
		void				setOperateur(Client* client);
};