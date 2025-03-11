#pragma once

#include "gen.hpp"
#include <vector>

class Channel
{
private:
	int _maxuser;
	int _userNbr;
	std::string _channelKey;
	std::string _nomduchannel, _topic, _mode;
	bool _invitmode, _topadm, _channelsize;

public:
	std::vector<User> utilisateurs;
	std::vector<int> operators;
	std::vector<int> invitedUsers;

	Channel(const std::string &s);
	~Channel();
	void rejoindreChannel(const User &utilisateur, std::vector<Channel>::iterator channel);
	int findUser(std::string userName);
	bool findOperator(int fd);
	bool findInvite(int fd);
	void kickuser(std::string usernamekick);
	void addmode(char c);
	void submode(char c);
	std::string getMode() const;
	std::string const &getPass() const;
	int const &getUserNumber() const;
	int const &getMaxUser() const;
	std::string const &getNomduchannel() const;
	std::string const &getTopic() const;
	bool const &getInvitmode() const;
	bool const &getTopadm() const;
	bool const &getChannelsize() const;
	void setInvitmode(bool const &s);
	void setTopadm(bool const &s);
	void setChannelsize(bool const &s);
	void setPass(std::string const &s);
	void setMaxuser(int x);
	int setUserNumber(int operation, const User &from, std::vector<Channel>::iterator channel);
	void setNomduchannel(std::string const &s);
	void setTopic(std::string const &s);
};