/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:07:40 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/09 01:31:02 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include "webserv.hpp"

class LocationBlock
{
	private:
		std::string					_location;
		std::string					_root;
		std::string					_index;
		bool						_dirlisting;
		std::vector<std::string>	_methods;
		std::string					_redirect;

		void parseLocation(std::string block);
		void parseRoot(std::string block);
		void parseIndex(std::string block);
		void parseDirlisting(std::string block);
		void parseMethods(std::string block);
		void parseRedirect(std::string block);
		

	public:
		LocationBlock(std::string block);
		LocationBlock(const LocationBlock &copy);
		LocationBlock &operator=(const LocationBlock &copy);
		~LocationBlock(void);

		std::string getLocation(void) const;
		std::string getRoot(void) const;
		std::string getIndex(void) const;
		bool getDirlisting(void) const;
		std::vector<std::string> getMethods(void) const;
		std::string getRedirect(void) const;
};

#endif
