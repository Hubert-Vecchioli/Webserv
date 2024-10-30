#include "webserv.hpp"

void	displayTimestamp(void)
{
	std::time_t now = time(0);
    std::tm *date = localtime(&now);
	std::cout << "[";
	std::cout << std::setfill('0') << 1900 + date->tm_year;
	std::cout << "-";
	std::cout << std::setfill('0') << std::setw(2) << 1 + date->tm_mon;
	std::cout << "-";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_mday;
	std::cout << " ";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_hour;
	std::cout << ":";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_min;
	std::cout << ":";
	std::cout << std::setfill('0') << std::setw(2) << date->tm_sec;
	std::cout << "] ";
}

std::string	displayTimestampResponseFormat(void)
{
	std::time_t now = time(0);
    std::tm *date = localtime(&now);
    std::ostringstream oss;
	oss << std::setfill('0') << 1900 + date->tm_year;
	oss << " - ";
	oss << std::setfill('0') << std::setw(2) << 1 + date->tm_mon;
	oss << " - ";
	oss << std::setfill('0') << std::setw(2) << date->tm_mday;
	oss << "T";
	oss << std::setfill('0') << std::setw(2) << date->tm_hour;
	oss << " : ";
	oss << std::setfill('0') << std::setw(2) << date->tm_min;
	oss << " : ";
	oss << std::setfill('0') << std::setw(2) << date->tm_sec;
	oss << "Z";
    return oss.str();
}

void	print(int i, std::string message, int fd)
{
	displayTimestamp();
	if(i == 1 && fd == -2)
		std::cout << message << std::endl;
	else if(i == 1 && fd != -2)
		std::cout << message << fd << std::endl;
	else
		std::cerr << message << std::endl;
}


std::vector<std::string> tokenize(std::string s, char delimiter) {
	std::stringstream ss(s);
	std::string token;
	std::vector<std::string> tokens;
	while (std::getline(ss, token, delimiter)) {
		if (token != "")
			tokens.push_back(token);
	}
	return tokens;
}

void	modifyEpollCTL(int EpollFD, int listendFD, int epollAction, bool isReadyToSendResponse)
{
	epoll_event ev;
	if (epollAction != EPOLL_CTL_DEL)
		ev.events = EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN;
	if (epollAction == EPOLL_CTL_MOD && isReadyToSendResponse)
		ev.events = EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN;
	ev.data.fd = listendFD;
	if (epoll_ctl(EpollFD, epollAction, listendFD, &ev) == -1)
		throw Server::FailureModifyFDEpollException();
}
