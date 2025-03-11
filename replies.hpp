// replies.hpp
#ifndef REPLIES_HPP
#define REPLIES_HPP

// Codes numériques IRC

#define RPL_WELCOME             ":irc.fastace.com 001 " + newUser.getNick() + " :Welcome to the Network ," + newUser.getNick() +"\r\n";
#define RPL_YOURHOST            ":irc.fastace.com 002 " + newUser.getNick() + " :Your host is irc.fastace.com, running version 1\r\n"; 
#define RPL_CREATED			    ":irc.fastace.com 003 " + newUser.getNick() + " :This server was created 20/01/24 \r\n";
#define RPL_MYINFO	      	    ":irc.fastace.com 004 " + newUser.getNick() + " :irc.fastace.com 1 itkol itkol\r\n"; 
//#define RPL_BOUNCE	      	":irc.fastace.com 005 " + newUser.getNick() + ":Try server irc.fastace.com, port 6667 \r\n"

#define PRIVMSG                 ":" + from.getID() + " PRIVMSG " + target + std::string(buffer) + "\r\n";
#define JOIN                    ":" + utilisateur.getID() + " JOIN " + _nomduchannel + "\r\n"
#define KICK                    ":" + from.getID() + " KICK " + channelName + " " + kiuser + " :" + motif + "\r\n"
#define INVITE                  ":" + from.getID() + " INVITE " + inviteUser + " " + channelName + "\r\n"

#define RPL_INVITING            ":irc.fastace.com 341 " + from.getNick() + " " + inviteUser + " " + channelName + "\r\n"
#define RPL_NAMEREPLY           ":irc.fastace.com 353 " + utilisateur.getNick() + " = " + _nomduchannel + " :@"
#define RPL_ENDOFNAMES          ":irc.fastace.com 366 " + utilisateur.getNick() + " " + _nomduchannel + " :End of /NAMES list\r\n"
#define RPL_TOPIC_JOIN          ":irc.fastace.com 332 " + utilisateur.getNick() + " " + _nomduchannel + " :" + _topic + "\r\n"
#define RPL_TOPIC               ":irc.fastace.com 332 " + itu->getNick() + " " + it->getNomduchannel() + " :" + option + "\r\n"
#define RPL_NOTOPIC             ":irc.fastace.com 331 " + itu->getNick() + " " + it->getNomduchannel() + " :No topic is set\r\n"
#define RPL_CHANNELMODEIS		":irc.fastace.com 324 " + from.getNick() + " " + channel.getNomduchannel() + " " + channel.getMode() +"\r\n"


#define ERR_NEEDMOREPARAMS      ":irc.fastace.com 461 " + from.getNick() + " " + tmp + ":Not enough parameters\r\n"
#define ERR_NICKNAMEINUSE	    ":irc.fastace.com 433 * " + newUser.getNick() + " :Nickname is already in use.\r\n";
#define ERR_CHANOPRIVSNEEDED    ":irc.fastace.com 482 " + from.getNick() + " " + it->getNomduchannel() + " :You're not channel operator\r\n"
#define ERR_NOTOCHANNEL         ":irc.fastace.com 442 " + from.getNick() + " " + it->getNomduchannel() + " :You're not on that channel\r\n"
#define ERR_NOSUCHCHANNEL       ":irc.fastace.com 403 " + from.getNick() + " " + channelName + " :No such channel\r\n"
#define ERR_USERNOTINCHANNEL	":irc.fastace.com 441 " + from.getNick() + " " + kiuser + " " + channelName + " :They aren't on that channel\r\n"
#define ERR_USERONCHANNEL       ":irc.fastace.com 443 "  + inviteUser + " " + channelName + " :already know " + ":" + inviteUser + "\r\n"
#define ERR_UNKNOWNCOMMAND		":irc.fastace.com 421 " + firstWord + " :Unknown command\r\n"
#define ERR_CHANNELISFULL       ":irc.fastace.com 471 " + from.getNick() + " " + channel->getNomduchannel() + " :Cannot join channel\r\n"
#define ERR_BADCHANNELKEY       ":irc.fastace.com 475 " + from.getNick() + " " + it->getNomduchannel() + " :Cannot join channel (+k)\r\n"
#define ERR_INVALIDKEY          ":irc.fastace.com 525 " + from.getNick() + " " + channel.getNomduchannel() + " :Key is not well-formed\r\n"
#define ERR_INVITONLYCHAN       ":irc.fastace.com 473 " + from.getNick() + " " + it->getNomduchannel() + " :Cannot join channel (+i)\r\n"
#define ERR_PASSWDMISMATCH		":irc.fastace.com 464  :Password incorrect\r\n"
#define ERR_NOSUCHNICK          ":irc.fastace.com 401 " + from.getNick() + " " + target + " :No such nick/channel\r\n"
#define ERR_NORECIPIENT         ":irc.fastace.com 411 " + from.getNick() + " :No recipient given (" + firstWord + " " + target + ")\r\n"
#define ERR_NOTEXTTOSEND        ":irc.fastace.com 412 " + from.getNick() + " :No text to send\r\n"
#define ERR_ERRONEUSNICKNAME    ":irc.fastace.com 432 " + nickname + " :Erroneus nickname\r\n" 

// ... Ajoutez d'autres codes numériques au besoin

#endif // REPLIES_HPP