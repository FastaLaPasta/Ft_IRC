#include "gen.hpp"

void ft_kick(std::string input, User from, std::vector<Channel> &Channels, std::string firstWord, std::string kiuser)
{
	std::cout << "\033[32mCommande KICK détectée.\033[0m" << std::endl;
	input.erase(input.end() - 2, input.end());
	std::string tmp(input), channelName, motif;
	std::istringstream topicOption(input.erase(0, firstWord.length()));
	std::vector<Channel>::iterator it;
	topicOption >> channelName;
	for (it = Channels.begin(); it != Channels.end(); it++)
	{
		if (it->getNomduchannel() == channelName)
			break;
	}
	motif = "OMG SALE SEXISTE LE BOT TE KICK DIRECT";
	if (it->findUser(kiuser))
	{
		send(it->findUser(kiuser), std::string(KICK).c_str(), std::string(KICK).length(), 0);
		it->kickuser(kiuser);
		send(from.getSocket(), std::string(KICK).c_str(), std::string(KICK).length(), 0);
		it->setUserNumber(-1, from, it);
	}
}

void cmd(std::string buf, User from, std::vector<Channel> &Channels, std::map<std::string, User> users)
{
	std::string input(buf);
	std::istringstream iss(input);
	std::string firstWord;
	iss >> firstWord;
	// Comparer le premier mot avec différentes commandes
	if (firstWord == "PRIVMSG")
	{
		std::cout << "\033[32mCommande PRIVMSG détectée.\033[0m" << std::endl;

		std::string target, message;
		char buffer[514];
		iss >> target;
		iss.getline(buffer, sizeof(buffer), '\r');
		message = PRIVMSG;

		std::vector<Channel>::iterator ite = Channels.begin();
		std::map<std::string, User>::iterator ituser = users.find(target);

		if (ituser != users.end() && !std::string(buffer).empty())
		{
			send(ituser->second.getSocket(), message.c_str(), message.length(), 0);
			return;
		}
		else
		{
			if (std::string(buffer).find("cuisine") != std::string::npos)
			{
				ft_kick(input, users["Administrator"], Channels, firstWord, from.getNick());
				return;
			}
			while (ite != Channels.end())
			{
				if (target == ite->getNomduchannel())
				{
					for (std::vector<User>::iterator it2 = ite->utilisateurs.begin(); it2 != ite->utilisateurs.end(); it2++)
						if (it2->getSocket() != from.getSocket())
							send(it2->getSocket(), message.c_str(), message.length(), 0);
					return;
				}
				ite++;
			}
			if (std::string(buffer).empty() && users.find(target) == users.end())
				send(from.getSocket(), std::string(ERR_NORECIPIENT).c_str(), std::string(ERR_NORECIPIENT).length(), 0);
			else if (std::string(buffer).empty() && users.find(target) != users.end())
				send(from.getSocket(), std::string(ERR_NOTEXTTOSEND).c_str(), std::string(ERR_NOTEXTTOSEND).length(), 0);
			else
				send(from.getSocket(), std::string(ERR_NOSUCHNICK).c_str(), std::string(ERR_NOSUCHNICK).length(), 0);
		}
	}
	else if (firstWord == "JOIN")
	{
		std::cout << "\033[32mCommande JOIN détectée.\033[0m" << std::endl;
		std::string channel, channelKey;
		std::vector<Channel>::iterator it;
		char buffer[1024];
		iss >> channel;
		iss >> channelKey;
		iss.getline(buffer, sizeof(buffer));
		for (it = Channels.begin(); it != Channels.end(); it++)
		{
			if (it->getNomduchannel() == channel)
			{
				if (it->getPass().empty() || channelKey == it->getPass())
				{
					if ((it->getInvitmode() == true && it->findInvite(from.getSocket())) || it->getInvitmode() == false)
					{
						it->rejoindreChannel(from, it);
						std::string message = ":" + users["Administrator"].getID() + " PRIVMSG " + channel + " Quel est la place de la femme ? (1 mot)\r\n";
						send(from.getSocket(), message.c_str(), message.length(), 0);
					}
					else if ((it->getInvitmode() == true) && (it->findInvite(from.getSocket()) == false))
						send(from.getSocket(), std::string(ERR_INVITONLYCHAN).c_str(), std::string(ERR_INVITONLYCHAN).length(), 0);
				}
				else if (!it->getPass().empty() && channelKey != it->getPass())
					send(from.getSocket(), std::string(ERR_BADCHANNELKEY).c_str(), std::string(ERR_BADCHANNELKEY).length(), 0);
				return;
			}
		}
		std::cout << "\033[34mCreation d'un nouveau CHANNEL !!\033[0m" << std::endl;
		if (channel[0] != '#')
			channel.insert(channel.begin(), '#');
		Channel newChan(channel);
		newChan.rejoindreChannel(from, it);
		newChan.operators.push_back(0);
		Channels.push_back(newChan);
	}
	else if (firstWord == "KICK")
	{
		std::cout << "\033[32mCommande KICK détectée.\033[0m" << std::endl;
		input.erase(input.end() - 2, input.end());
		std::string tmp(input), channelName, kiuser, motif;
		std::istringstream topicOption(input.erase(0, firstWord.length()));
		std::vector<Channel>::iterator it;
		topicOption >> channelName;
		for (it = Channels.begin(); it != Channels.end(); it++)
			if (it->getNomduchannel() == channelName)
				break;
		if (it == Channels.end())
			send(from.getSocket(), std::string(ERR_NOSUCHCHANNEL).c_str(), std::string(ERR_NOSUCHCHANNEL).length(), 0);
		else if (!it->findUser(from.getNick()))
			send(from.getSocket(), std::string(ERR_NOTOCHANNEL).c_str(), std::string(ERR_NOTOCHANNEL).length(), 0);
		else if (!it->findOperator(from.getSocket()))
			send(from.getSocket(), std::string(ERR_CHANOPRIVSNEEDED).c_str(), std::string(ERR_CHANOPRIVSNEEDED).length(), 0);
		else
		{
			topicOption >> kiuser;
			motif = input.erase(0, (channelName.length() + 2) + (kiuser.length() + 2));
			if (it->findUser(kiuser))
			{
				send(it->findUser(kiuser), std::string(KICK).c_str(), std::string(KICK).length(), 0);
				it->kickuser(kiuser);
				send(from.getSocket(), std::string(KICK).c_str(), std::string(KICK).length(), 0);
				it->setUserNumber(-1, from, it);
			}
			else
				send(from.getSocket(), std::string(ERR_USERNOTINCHANNEL).c_str(), std::string(ERR_USERNOTINCHANNEL).length(), 0);
		}
	}
	else if (firstWord == "INVITE")
	{
		std::cout << "\033[32mCommande INVITE détectée.\033[0m" << std::endl;
		input.erase(input.end() - 2, input.end());
		std::string tmp(input), channelName, inviteUser, motif;
		std::istringstream topicOption(input.erase(0, firstWord.length()));
		std::vector<Channel>::iterator it;
		topicOption >> inviteUser;
		topicOption >> channelName;
		for (it = Channels.begin(); it != Channels.end(); it++)
		{
			if (it->getNomduchannel() == channelName)
				break;
		}
		if (it == Channels.end())
			send(from.getSocket(), std::string(ERR_NOSUCHCHANNEL).c_str(), std::string(ERR_NOSUCHCHANNEL).length(), 0);
		else if (!it->findUser(from.getNick()))
			send(from.getSocket(), std::string(ERR_NOTOCHANNEL).c_str(), std::string(ERR_NOTOCHANNEL).length(), 0);
		else if (!it->findOperator(from.getSocket()))
			send(from.getSocket(), std::string(ERR_CHANOPRIVSNEEDED).c_str(), std::string(ERR_CHANOPRIVSNEEDED).length(), 0);
		else
		{
			if ((!it->findUser(inviteUser)) && (users.find(inviteUser) != users.end()))
			{
				send(from.getSocket(), std::string(RPL_INVITING).c_str(), std::string(RPL_INVITING).length(), 0);
				send(users.find(inviteUser)->second.getSocket(), std::string(INVITE).c_str(), std::string(INVITE).length(), 0);
				it->invitedUsers.push_back(users.find(inviteUser)->second.getSocket());
			}
			else if (users.find(inviteUser) != users.end())
				send(from.getSocket(), std::string(ERR_USERONCHANNEL).c_str(), std::string(ERR_USERONCHANNEL).length(), 0);
		}
	}
	else if (firstWord == "TOPIC")
	{
		std::cout << "\033[32mCommande TOPIC détectée.\033[0m" << std::endl;
		input.erase(input.end() - 2, input.end());
		std::string tmp(input), channelName, option;
		std::istringstream topicOption(input.erase(0, firstWord.length()));
		std::vector<Channel>::iterator it;
		topicOption >> channelName;
		for (it = Channels.begin(); it != Channels.end(); it++)
		{
			if (it->getNomduchannel() + ":" == channelName || it->getNomduchannel() == channelName)
				break;
		}
		if (it == Channels.end())
			send(from.getSocket(), std::string(ERR_NOSUCHCHANNEL).c_str(), std::string(ERR_NOSUCHCHANNEL).length(), 0);
		else if (!it->findUser(from.getNick()))
			send(from.getSocket(), std::string(ERR_NOTOCHANNEL).c_str(), std::string(ERR_NOTOCHANNEL).length(), 0);
		else if (!it->findOperator(from.getSocket()) && it->getTopadm() == true)
			send(from.getSocket(), std::string(ERR_CHANOPRIVSNEEDED).c_str(), std::string(ERR_CHANOPRIVSNEEDED).length(), 0);
		else
		{
			option = input.erase(0, channelName.length() + 2);
			if (option.empty() || option == ":")
			{
				option.erase(0, 1);
				channelName.erase(channelName.end() - 1, channelName.end());
				for (std::vector<User>::iterator itu = it->utilisateurs.begin(); itu != it->utilisateurs.end(); itu++)
					send(itu->getSocket(), std::string(RPL_TOPIC).c_str(), std::string(RPL_TOPIC).length(), 0);
			}
			else if (option.length() > 1)
			{
				option.erase(0, 1);
				it->setTopic(option);
				for (std::vector<User>::iterator itu = it->utilisateurs.begin(); itu != it->utilisateurs.end(); itu++)
					send(itu->getSocket(), std::string(RPL_TOPIC).c_str(), std::string(RPL_TOPIC).length(), 0);
			}
		}
	}
	else if (firstWord == "MODE")
	{
		std::cout << "\033[32mCommande MODE détectée.\033[0m" << std::endl;
		input.erase(input.end() - 2, input.end());
		std::string tmp(input), channelName;
		char modestring[514];
		std::istringstream topicOption(input.erase(0, firstWord.length()));
		std::vector<Channel>::iterator it;
		topicOption >> channelName;
		if (channelName[0] != '#')
			return;
		for (it = Channels.begin(); it != Channels.end(); it++)
		{
			if (it->getNomduchannel() == channelName)
				break;
		}
		topicOption.getline(modestring, sizeof(modestring), '\r');
		if (it == Channels.end())
			send(from.getSocket(), std::string(ERR_NOSUCHCHANNEL).c_str(), std::string(ERR_NOSUCHCHANNEL).length(), 0);
		else if (!it->findUser(from.getNick()))
			send(from.getSocket(), std::string(ERR_NOTOCHANNEL).c_str(), std::string(ERR_NOTOCHANNEL).length(), 0);
		else if (!it->findOperator(from.getSocket()) && modestring[0] != '\0')
			send(from.getSocket(), std::string(ERR_CHANOPRIVSNEEDED).c_str(), std::string(ERR_CHANOPRIVSNEEDED).length(), 0);
		else
			handleChannelModes(*it, modestring, from);
	}
	else if (firstWord == "PING")
	{
		std::cout << "\033[32mCommande PING détectée.\033[0m" << std::endl;
		char buffer[514];
		iss.getline(buffer, sizeof(buffer), '\r');
		std::string vn(buffer);
		std::string mes = ":irc.fastace.com PONG irc.fastace.com" + vn + "\r\n";
		send(from.getSocket(), mes.c_str(), mes.length(), 0);
		std::cout << "\033[1;35m>> \033[0m" << mes;
	}
	else if (firstWord == "QUIT")
	{
		std::cout << "\033[32mCommande QUIT détectée.\033[0m" << std::endl;
		for (std::vector<Channel>::iterator it = Channels.begin(); it != Channels.end();)
		{
			std::vector<User>::iterator it2 = it->utilisateurs.begin();
			while (it2 != it->utilisateurs.end())
			{
				std::cout << it2->getNick() << " ";
				if (it2->getSocket() == from.getSocket())
				{
					it2 = it->utilisateurs.erase(it2);
					std::cout << from.getNick() << ": a left the channel == " << it->getNomduchannel() << std::endl;
				}
				else
					++it2;
			}
			if (it->utilisateurs.empty())
				it = Channels.erase(it);
			else
				++it;
		}
	}
	else
	{
		std::cout << "\033[33mCommande inconnue.\033[0m" << std::endl;
		send(from.getSocket(), std::string(ERR_UNKNOWNCOMMAND).c_str(), std::string(ERR_UNKNOWNCOMMAND).length(), 0);
		std::cout << "\033[1;35m>> \033[0" << ERR_UNKNOWNCOMMAND << std::endl;
	}
}