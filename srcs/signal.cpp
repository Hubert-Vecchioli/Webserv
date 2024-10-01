/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:41:09 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/01 12:18:26 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void ft_stop_server(int signum)
{
	print(1, "[Info] - Signal request to stop the server received");
	ft_get_main_server().stop();
}