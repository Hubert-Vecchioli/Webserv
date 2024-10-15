/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:07:40 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/15 12:58:00 by ebesnoin         ###   ########.fr       */
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
		std::pair<int, std::string>	_redirect;
		std::map<std::string, std::string> cgi_extension;

		void parseLocationBlock(std::vector<std::string> block);
		void parseLocation(std::vector<std::string> args);
		void parseRoot(std::vector<std::string> args);
		void parseIndex(std::vector<std::string> args);
		void parseDirlisting(std::vector<std::string> args);
		void parseMethods(std::vector<std::string> args);
		void parseRedirect(std::vector<std::string> args);
		

	public:
		LocationBlock(std::vector<std::string> block);
		LocationBlock(const LocationBlock &copy);
		LocationBlock &operator=(const LocationBlock &copy);
		~LocationBlock(void);

		std::string getLocation(void) const;
		std::string getRoot(void) const;
		std::string getIndex(void) const;
		bool getDirlisting(void) const;
		std::vector<std::string> getMethods(void) const;
		std::pair<int, std::string> getRedirect(void) const;
};

#endif
