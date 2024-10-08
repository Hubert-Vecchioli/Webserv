/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:19 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/08 08:08:27 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void HttpResponse::getResponseContent(void)
{
	if(/*request is CGI*/)
		// trigger the CGI
	switch(/*Request Method*/)
	{
		case "GET":
			/**/;
			break;
		case "POST":
			/**/;
			break;
		case "PUT":
			/**/;
			break;
		case "DEL":
			/**/;
			break;
		default:
			throw /*INVALID METHOD CODE 405*/();
	}
}