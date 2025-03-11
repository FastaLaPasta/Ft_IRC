#pragma once

#include <cstdio>
#include <stdlib.h>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <sys/epoll.h>
#include <cstdlib>
#include "replies.hpp"
#include <cstring>
#include <map>
#include <sstream>
#include "User.hpp"
#include "Channel.hpp"
#include <vector>
#include <signal.h>
#include <termios.h>
#include "Server.hpp"
#include <fstream>

class Server;

#define TRUE 1
#define FALSE 0
const int MAX_EVENTS = 10;

void        cmd(std::string buf, User from, std::vector<Channel> &Channels, std::map<std::string, User> users);
void        mastersockinit(int &master_socket);
void        bindandlistenmaster(int &master_socket, struct sockaddr_in &address);
int         processUserInfo(User &newUser, Server &master, int client_socket, std::string buffer);
std::string extractData(std::string extractSentence);


void		handleChannelModes(Channel& channel, const std::string& modestring, User from);
void		handleModeT(char mode, Channel& channel);
void		handleModeI(std::istringstream& args, User from);
void		handleModeK(char mode, Channel& channel,std::string& args);

