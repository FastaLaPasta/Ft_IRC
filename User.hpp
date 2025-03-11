#pragma once

#include "gen.hpp"

class User
{
private:
	int				_socket, _end;
	int				_userNbr;
	std::string		_nickname, _username, _password;

public:
	User() {}
	
	User(int socket) : _socket(socket) {_end = 0; _userNbr = 0;}
	User(int socket, const std::string &username) : _socket(socket), _nickname(username) {_end = 0; _userNbr = 0;}

	std::string const	&getNick() const { return (_nickname); }
	std::string const	getID() const { std::string id =  _nickname + "!" + _username + "@";  return (id); }
	std::string const	&getUName() const { return (_username); }
	std::string const	&getPass() const { return (_password); }
	int					&getUserNumber() { return (_userNbr); }
	int const			&getSocket() const { return (_socket); }
	int const			&getEnd() const { return (_end); }
	void				setNick(std::string const &s) { _nickname = s; }
	void				setUName(std::string const &s) { _username = s; }
	void				setPass(std::string const &s) { _password = s; }
	void				setEnd(int x) { _end = x; }
	void				setUserNumber() { _userNbr += 1;}
	~User() {}
};