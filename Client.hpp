/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vanitas <vanitas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:20:22 by vanitas           #+#    #+#             */
/*   Updated: 2024/05/22 16:21:51 by vanitas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


class Client
{
	private:
	public:
		Client();
		Client(const Client &cpy);
		~Client();
		Client    &operator=(const Client &rhs);
};