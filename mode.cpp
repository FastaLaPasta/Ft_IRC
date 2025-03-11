#include "gen.hpp"

void handleModeI(char mode, Channel &channel, std::string &args, User &from)
{
	(void)args;
	(void)from;
	std::cout << "\033[36mMode 'i' traité.\033[0m" << std::endl;
	if (mode == '+')
	{
		channel.setInvitmode(true);
		channel.addmode('i');
	}
	else if (mode == '-')
	{
		channel.setInvitmode(false);
		channel.submode('i');
	}
	std::cout << "Channel invitmode: " << std::boolalpha << channel.getInvitmode() << std::endl;
}

void handleModeT(char mode, Channel &channel, std::string &args, User &from)
{
	(void)from;
	(void)args;
	std::cout << "\033[36mMode 't' traité.\033[0m" << std::endl;
	if (mode == '+')
	{
		channel.setTopadm(true);
		channel.addmode('t');
	}
	else if (mode == '-')
	{
		channel.setTopadm(false);
		channel.submode('t');
	}
	std::cout << "Channel Topic admin only: " << std::boolalpha << channel.getTopadm() << std::endl;
}

void handleModeK(char mode, Channel &channel, std::string &args, User &from)
{
	(void)from;
	std::cout << "\033[36mMode 'k' traité.\033[0m" << std::endl;
	if (mode == '+')
	{
		std::istringstream iss(args);
		std::string key, rest;
		iss >> key;
		iss >> rest;
		channel.setPass(key);
		channel.addmode('k');
		args.erase(0, key.size() + 1);
	}
	else if (mode == '-')
	{
		channel.setPass("");
		channel.submode('k');
	}
	std::cout << "Channel Pass:" << channel.getPass() << std::endl;
}

void handleModeO(char mode, Channel &channel, std::string &args, User &from)
{
	(void)from;
	std::cout << "\033[36mMode 'o' traité.\033[0m" << std::endl;
	if (mode == '+')
	{
		std::istringstream iss(args);
		std::string op;
		iss >> op;
		if (!channel.findOperator(channel.findUser(op)) && channel.findUser(op))
		{
			channel.operators.push_back(channel.findUser(op));
			std::cout << "User:" << op << " become operator!" << std::endl;
		}
		args.erase(0, op.size() + 1);
	}
	else if (mode == '-')
	{
		std::istringstream iss(args);
		std::string op;
		iss >> op;
		if (channel.findOperator(channel.findUser(op)))
		{
			for (std::vector<int>::iterator it = channel.operators.begin(); it != channel.operators.end();)
			{
				if (*it == channel.findUser(op))
					channel.operators.erase(it);
				else
					++it;
			}
		}
		args.erase(0, op.size() + 1);
	}
}

// Fonction pour gérer le mode 'l'
void handleModeL(char mode, Channel &channel, std::string &args, User &from)
{
	(void)from;
	std::cout << "\033[36mMode 'l' traité.\033[0m" << mode << std::endl;
	if (mode == '+')
	{
		std::istringstream iss(args);
		int key;
		iss >> key;
		if ((iss.rdstate() & std::ifstream::failbit) != 0)
			return;
		if (key > 0)
		{
			channel.setMaxuser(key);
			channel.addmode('l');
			channel.setChannelsize(true);
		}
		while (key > 9)
		{
			args.erase(0, 1);
			key = key / 10;
		}
		args.erase(0, 1);
	}
	else if (mode == '-')
	{
		channel.setChannelsize(false);
		channel.setMaxuser(1);
		channel.submode('l');
	}
	std::cout << "set max user: " << channel.getMaxUser() << std::endl;
}

typedef void (*ModeHandler)(char, Channel &, std::string &, User &);

void modeHandlersAttribution(std::map<char, ModeHandler> &modeHandlers)
{
	modeHandlers['i'] = handleModeI;
	modeHandlers['t'] = handleModeT;
	modeHandlers['k'] = handleModeK;
	modeHandlers['o'] = handleModeO;
	modeHandlers['l'] = handleModeL;
}

void handleChannelModes(Channel &channel, const std::string &modestring, User from)
{
	std::map<char, ModeHandler> modeHandlers;
	std::map<char, ModeHandler>::iterator it;
	std::istringstream iss(modestring);
	std::string options, tmp;
	ModeHandler handler;
	char mode;

	modeHandlersAttribution(modeHandlers);
	iss >> mode;
	iss >> options;
	iss >> tmp;
	if (tmp[0] == '-')
		options += tmp;
	size_t argsPos = modestring.find(options) + options.length() + 1;
	std::string args = (argsPos < modestring.length()) ? modestring.substr(argsPos) : "";
	for (size_t i = 0; options[i]; i++)
	{
		if (options[i] == '-')
		{
			mode = '-';
			continue;
		}
		it = modeHandlers.find(options[i]);
		if (it != modeHandlers.end())
		{
			handler = it->second;
			handler(mode, channel, args, from);
		}
		else
			break;
	}
	send(from.getSocket(), std::string(RPL_CHANNELMODEIS).c_str(), std::string(RPL_CHANNELMODEIS).length(), 0);
}