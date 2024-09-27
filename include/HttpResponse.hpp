/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:00 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/27 17:15:11 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <iostream>
# include <map>

class HttpResponse
{
private:

public:
	HttpResponse(void);
	HttpResponse(HttpResponse const & rhs);
	HttpResponse &operator=(HttpResponse const & rhs);
	~HttpResponse(void);
	
};

#endif