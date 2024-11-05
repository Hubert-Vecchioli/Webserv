
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <vector>
# include <iomanip>
# include <ctime>
# include <string>
# include <cstdlib>
# include <limits>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <dirent.h>
# include <cstring>
# include <algorithm>

# include "Server.hpp"
# include "Socket.hpp"
# include "Client.hpp"
# include "CgiHandler.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ConfigurationFile.hpp"
# include "Client.hpp"
# include "LocationBlock.hpp"
# include "ServerBlock.hpp"

//# include "signal.cpp"



#define MAX_EVENTS 50 // to be increassed later on
#define EPOLL_MAX_WAIT_TIME_MS 1000 // to be modified later on
#define CLIENT_TIMEOUT_LIMIT_SEC 120
#define REQUEST_TIMEOUT_LIMIT_SEC 5
#define MAX_REQUEST_SIZE 50000
#define	MAX_URI_SIZE 65536
#define	FILE_CHUNK_THRESHOLD 163840 // randomly chose 5120 * 2^5
#define RESPONSE_BUFFER 20480 // randomly chose 5120 * 2^2
#define MAX_CLIENT_NUMBER 250

void	displayTimestamp(void);
std::string	displayTimestampResponseFormat(void);
void	print(int i, std::string message, int fd = -2);
std::vector<std::string> tokenize(std::string s, char delimiter);
void	modifyEpollCTL(int EpollFD, int listendFD, int epollAction, bool isReadyToSendResponse = false);
void 	ft_stop_server(int signum);

#endif
