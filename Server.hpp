#pragma once

#include "gen.hpp"
#include <vector>

class Channel;

class Server
{
private:
	uint16_t _port;
	std::string _password;
	int _master_socket;
	int _epoll_fd;

public:
	std::map<std::string, User> users;
	std::vector<Channel> Channels;
	User administrator;
	Server();
	~Server();

	uint16_t const &getPort() const;
	std::string const &getPass() const;
	int &getMastersocket();
	int const &getEpollfd() const;

	void setPort(uint16_t x);
	void setPass(std::string const &s);
	void setMastersocket(int x);
	void setEpollfd(int x);
};