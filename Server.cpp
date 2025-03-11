#include "Server.hpp"

Server::Server() : administrator(0, "Administrator") { administrator.setEnd(1), administrator.setUName("admin"); };
Server::~Server(){};

uint16_t const &Server::getPort() const { return (_port); }
std::string const &Server::getPass() const { return (_password); }
int &Server::getMastersocket() { return (_master_socket); }
int const &Server::getEpollfd() const { return (_epoll_fd); }

void Server::setPort(uint16_t x) { _port = x; }
void Server::setPass(std::string const &s) { _password = s; }
void Server::setMastersocket(int x) { _master_socket = x; }
void Server::setEpollfd(int x) { _epoll_fd = x; }