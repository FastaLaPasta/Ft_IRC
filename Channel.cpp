#include "Channel.hpp"

Channel::Channel(const std::string &s) : _nomduchannel(s)
{
	_invitmode = false;
	_topadm = false;
	_channelsize = false;
	_mode = "+";
	_maxuser = 1;
	_channelKey = "";
	_userNbr = 0;
}

Channel::~Channel(){};

void Channel::rejoindreChannel(const User &utilisateur, std::vector<Channel>::iterator channel)
{
	std::string userList(RPL_NAMEREPLY);
	for (std::vector<User>::iterator it = utilisateurs.begin(); it != utilisateurs.end(); it++)
		if (it->getSocket() == utilisateur.getSocket())
			return;
		
	if (utilisateurs.size() == 0)
		operators.push_back(utilisateur.getSocket());
	if (setUserNumber(1, utilisateur, channel) == 1)
	{
		utilisateurs.push_back(utilisateur);
		for (std::vector<User>::iterator it = utilisateurs.begin(); it != utilisateurs.end(); it++)
		{
			send(it->getSocket(), std::string(JOIN).c_str(), std::string(JOIN).length(), 0);
			if (findOperator(it->getSocket()))
				userList += "@";
			userList += it->getNick() + " ";
		}
		userList += "\r\n";
		send(utilisateur.getSocket(), userList.c_str(), userList.length(), 0);
		send(utilisateur.getSocket(), std::string(RPL_ENDOFNAMES).c_str(), std::string(RPL_ENDOFNAMES).length(), 0);
		send(utilisateur.getSocket(), std::string(RPL_TOPIC_JOIN).c_str(), std::string(RPL_TOPIC_JOIN).length(), 0);
	}
}

int Channel::findUser(std::string userName)
{
	std::vector<User>::iterator it;
	for (it = utilisateurs.begin(); it != utilisateurs.end(); it++)
		if (it->getNick() == userName)
			return (it->getSocket());
	return (0);
}

bool Channel::findOperator(int fd)
{
	std::vector<int>::iterator it;
	for (it = operators.begin(); it != operators.end(); it++)
		if (*it == fd)
			return (true);
	return (false);
}

bool Channel::findInvite(int fd)
{
	std::vector<int>::iterator it;
	for (it = invitedUsers.begin(); it != invitedUsers.end(); it++)
		if (*it == fd)
			return (true);
	return (false);
}

void Channel::kickuser(std::string usernamekick)
{
	std::vector<User>::iterator it;
	for (it = utilisateurs.begin(); it != utilisateurs.end();)
		if (it->getNick() == usernamekick)
			it = utilisateurs.erase(it);
		else
			++it;
}

void Channel::addmode(char c)
{
	if (_mode.find(c) == std::string::npos)
		_mode = _mode + c;
}

void Channel::submode(char c)
{
	if (_mode.find(c) != std::string::npos)
		_mode.erase(_mode.find(c), 1);
}

std::string const &Channel::getPass() const { return (_channelKey); }
int const &Channel::getUserNumber() const { return (_userNbr); }
int const &Channel::getMaxUser() const { return (_maxuser); }
std::string const &Channel::getNomduchannel() const { return (_nomduchannel); }
std::string const &Channel::getTopic() const { return (_topic); }
bool const &Channel::getInvitmode() const { return (_invitmode); }
bool const &Channel::getTopadm() const { return (_topadm); }
bool const &Channel::getChannelsize() const { return (_channelsize); }
std::string Channel::getMode() const
{
	char buffer[20]; // Assurez-vous que la taille du tampon est suffisante
	int n = sprintf(buffer, "%d", getMaxUser());
	std::string ok(_mode);
	if (getPass() != "")
		ok += " :" + _channelKey;
	if (_channelsize != false)
		ok += " " + std::string(buffer, n);
	return ok;
}


void Channel::setInvitmode(bool const &s) { _invitmode = s; }
void Channel::setTopadm(bool const &s) { _topadm = s; }
void Channel::setChannelsize(bool const &s) { _channelsize = s; }
void Channel::setPass(std::string const &s) { _channelKey = s; }
void Channel::setNomduchannel(std::string const &s) { _nomduchannel = s; }
void Channel::setTopic(std::string const &s) { _topic = s; }
void Channel::setMaxuser(int x)
{
	if (x > 0)
		_maxuser = x;
	else
		_maxuser = 0;
}

int Channel::setUserNumber(int operation, const User &from, std::vector<Channel>::iterator channel)
{
	_userNbr += operation;
	if (_userNbr <= _maxuser)
		return (1);
	if (getChannelsize() == true && operation > 0)
	{
		send(from.getSocket(), std::string(ERR_CHANNELISFULL).c_str(), std::string(ERR_CHANNELISFULL).length(), 0);
		_userNbr -= 1;
		return (0);
	}
	return (1);
}
