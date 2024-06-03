/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NcClient.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:24:57 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/02 16:14:55 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Client.hpp"

class	NcClient : public Client
{
	public:
					NcClient();
					NcClient(const NcClient& cpy);
		NcClient&	operator=(const NcClient& rhs);
					~NcClient();

		Destination		ParseAndRespond(std::string& input); //override
		void			formatText(std::string& input); //override
};