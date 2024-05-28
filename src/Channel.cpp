/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:36:16 by mablatie          #+#    #+#             */
/*   Updated: 2024/05/28 15:29:24 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(const Channel& cpy)
{
	*this = cpy;
}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel& rhs)
{
	if (this != &rhs)
	{
		this->name = rhs.getName();
		// this->operateur = rhs.getOperateur();
	}
	return *this;
}

const std::string& Channel::getName() const
{
	return this->name;
}

void Channel::setName(std::string &input)
{
	this->name = input;
}

Client* Channel::getOperateur()
{
	return this->operateur;
}

void Channel::setOperateur(Client* client)
{
	this->operateur = client;
}