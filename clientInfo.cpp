#include "gen.hpp"

std::string extractData(std::string extractSentence)
{
	std::string infoExtract;
	std::istringstream iss(extractSentence);
	iss >> infoExtract;

	return (infoExtract);
}

bool verifNickSyntax(std::string nickname)
{
	for (size_t i = 0; nickname[i]; i++)
		if (nickname[i] == '#' || nickname[i] == ':')
			return (false);
	return (true);
}

int processUserInfo(User &newUser, Server &master, int client_socket, std::string buffer)
{
	std::string message(buffer);

	size_t capstart = message.find("CAP LS");
	if (capstart != std::string::npos)
	{
		std::string mes = "CAP * LS :\r\n";
		send(client_socket, mes.c_str(), mes.length(), 0);
		std::cout << "\033[1;35m>> \033[0m" << mes;
	}

	size_t userPass = message.find("PASS ");
	if (userPass != std::string::npos)
		newUser.setPass(extractData(message.erase(0, userPass + 5)));

	size_t nickPos = message.find("NICK ");
	if (nickPos != std::string::npos)
	{
		if (master.getPass() != newUser.getPass())
		{
			send(newUser.getSocket(), std::string(ERR_PASSWDMISMATCH).c_str(), std::string(ERR_PASSWDMISMATCH).length(), 0);
			std::cout << "\033[1;35m>> \033[0m" << ERR_PASSWDMISMATCH;
			sleep(1);
			close(client_socket);
			return (1);
		}
		std::string nickname(extractData(message.erase(0, nickPos + 5)));
		if (verifNickSyntax(nickname))
		{
			if (master.users.find(nickname) != master.users.end())
			{
				char tmp[5];
				int usernbr = sprintf(tmp, "%d", master.users.find(nickname)->second.getUserNumber());
				newUser.setNick(master.users.find(nickname)->second.getNick() + "_" + std::string(tmp, usernbr));
				master.users.find(nickname)->second.setUserNumber();
			}
			else
				newUser.setNick(nickname);
		}
		else
		{
			send(client_socket, std::string(ERR_ERRONEUSNICKNAME).c_str(), std::string(ERR_ERRONEUSNICKNAME).length(), 0);
			sleep(1);
			close(client_socket);
			return (1);
		}
		master.users[newUser.getNick()] = newUser;
		std::string mes = ":" + newUser.getID() + " NICK " + newUser.getNick() + "\r\n";
		send(client_socket, mes.c_str(), mes.length(), 0);
		std::cout << "\033[1;35m>> \033[0m" << mes;
	}

	size_t userPos = message.find("USER ");
	if (userPos != std::string::npos)
		newUser.setUName(extractData(message.erase(0, userPos + 5)));

	size_t capend = message.find("CAP END");
	if (capend != std::string::npos)
		newUser.setEnd(1);
	return (0);
}