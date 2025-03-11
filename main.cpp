#include "gen.hpp"

Server master;
//MODE #ok +ioltk Ace 1 password
void closeeverything()
{
	// Fermer le descripteur epoll et le socket principal avant de quitter
	// Fermer le descripteur epoll
	close(master.getEpollfd());
	// Fermer le socket principal
	close(master.getMastersocket());
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	std::istringstream po(av[1]);
	uint16_t tmport;
	po >> tmport;
	master.setPort(tmport);
	if ((po.rdstate() & std::ifstream::failbit) != 0)
		return 1;
	std::istringstream pa(av[2]);
	std::string tmpass;
	pa >> tmpass;
	master.setPass(tmpass);
	if ((pa.rdstate() & std::ifstream::failbit) != 0)
		return 1;
	mastersockinit(master.getMastersocket());
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(master.getPort());
	bindandlistenmaster(master.getMastersocket(), address);
	std::cout << "Listener on port " << master.getPort() << std::endl;
	master.setEpollfd(epoll_create1(0));
	if (master.getEpollfd() == -1)
	{
		perror("epoll_create1");
		return EXIT_FAILURE;
	}
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = master.getMastersocket();
	if (epoll_ctl(master.getEpollfd(), EPOLL_CTL_ADD, master.getMastersocket(), &event) == -1)
	{
		perror("epoll_ctl");
		return EXIT_FAILURE;
	}
	master.users[master.administrator.getNick()] = master.administrator;
	while (true)
	{
		struct epoll_event events[MAX_EVENTS];
		int num_events = epoll_wait(master.getEpollfd(), events, MAX_EVENTS, -1);
		if (num_events == -1)
		{
			perror("epoll_wait");
			return EXIT_FAILURE;
		}
		for (int i = 0; i < num_events; ++i)
		{
			if (events[i].data.fd == master.getMastersocket())
			{
				int client_socket = accept(master.getMastersocket(), NULL, NULL);
				if (client_socket == -1)
				{
					perror("accept");
					continue;
				}
				struct epoll_event client_event;
				client_event.events = EPOLLIN;
				client_event.data.fd = client_socket;
				if (epoll_ctl(master.getEpollfd(), EPOLL_CTL_ADD, client_socket, &client_event) == -1)
				{
					perror("epoll_ctl (add client)");
					close(client_socket);
					continue;
				}
				std::string nickname, username, password;
				User newUser(client_socket);
				char buffer[1024];
				ssize_t bytesRead;
				std::cout << "Nouvelle connexion: " << client_socket << std::endl;
				while (newUser.getEnd() != 1)
				{
					std::string boeuf;
					while (boeuf[boeuf.length() - 1] != '\n' && boeuf[boeuf.length() - 2] != '\r')
					{
						memset(buffer, '\0', sizeof(buffer));
						bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
						buffer[bytesRead] = '\0';
						boeuf += buffer;
						if (bytesRead == -1)
							break;
						else if (bytesRead == 0)
							break;
					}
					boeuf.push_back('\0');
					std::cout << "\033[1;33m<< \033[0m " << boeuf;
					if (bytesRead == -1)
					{
						perror("recv");
						close(client_socket);
						break;
					}
					else if (bytesRead == 0)
					{
						std::cout << "end" << std::endl;
						break;
					}
					else if (processUserInfo(newUser, master, client_socket, boeuf) != 0)
						break;
					i++;
				}
				if (newUser.getEnd() != 0)
				{

					std::string ok = RPL_WELCOME;
					send(client_socket, ok.c_str(), ok.length(), 0);
					std::cout << "\033[1;35m>> \033[0m" << ok;
					ok = RPL_YOURHOST;
					send(client_socket, ok.c_str(), ok.length(), 0);
					std::cout << "\033[1;35m>> \033[0m" << ok;
					ok = RPL_CREATED;
					send(client_socket, ok.c_str(), ok.length(), 0);
					std::cout << "\033[1;35m>> \033[0m" << ok;
					ok = RPL_MYINFO;
					send(client_socket, ok.c_str(), ok.length(), 0);
					std::cout << "\033[1;35m>> \033[0m" << ok;
				}
			}
			else
			{
				int client_socket = events[i].data.fd;
				char buffer[1024];
				ssize_t bytesRead;
				std::string boeuf;
				while (boeuf[boeuf.length() - 1] != '\n' && boeuf[boeuf.length() - 2] != '\r')
				{
					bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
					buffer[bytesRead] = '\0';
					boeuf += buffer;
					if (bytesRead == -1)
						break;
					else if (bytesRead == 0)
						break;
				}
				if (bytesRead == -1)
				{
					perror("recv");
					epoll_ctl(master.getEpollfd(), EPOLL_CTL_DEL, client_socket, NULL);

					std::map<std::string, User>::iterator it = master.users.begin();
					while (it != master.users.end())
					{
						if (it->second.getSocket() == client_socket)
						{
							std::cout << "Utilisateur déconnecté: " << it->first << std::endl;
							master.users.erase(it++);
						}
						else
							++it;
					}
					close(client_socket);
					continue;
				}
				else if (bytesRead == 0)
				{
					epoll_ctl(master.getEpollfd(), EPOLL_CTL_DEL, client_socket, NULL);
					std::map<std::string, User>::iterator it = master.users.begin();
					while (it != master.users.end())
					{
						if (it->second.getSocket() == client_socket)
						{
							std::cout << "Utilisateur déconnecté: " << it->first << std::endl;
							master.users.erase(it++);
						}
						else
							++it;
					}
					close(client_socket);
				}
				else
				{
					std::cout << "\033[1;33m<< \033[0m " << client_socket << ": " << boeuf;
					std::map<std::string, User>::iterator it = master.users.begin();
					while (it != master.users.end())
					{
						if (it->second.getSocket() == client_socket)
						{
							cmd(boeuf, it->second, master.Channels, master.users);
							break;
						}
						++it;
					}
				}
			}
		}
	}
	closeeverything();
	return 0;
}