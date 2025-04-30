#ifndef MACROS_HPP
# define MACROS_HPP

// colors
# define GREEN	"\033[32;1m"
# define RED	"\033[31;1m"
# define CYAN	"\033[36;1m"
# define BOLD	"\033[37;1;4m"
# define WHITE	"\033[37;1m"
# define YELLOW	"\033[33;1m"
# define BLUE	"\033[34;1m"
# define MAGENT	"\033[35;1m"
# define RESET	"\033[0m"

# define FTIRC std::string(":ft.irc")
# define CRLF std::string("\r\n")
# define COMMA std::string(",")
# define COLON std::string(":")
# define SPACE std::string(" ")

#define RPL_WELCOME(nick, user) (FTIRC + " 001 " + user + " :Welcome to the Internet Relay Chat " + nick + "!" + user + "@*" + CRLF)

# define RPL_QUIT(nick, user, host, reason)                  (FTIRC + " 001 " + nick + "!" + user + "@" + host + " QUIT :" + reason + CRLF)
# define RPL_ENDOFWHO(channel)                               (FTIRC + " 315 " + channel + " :End of /WHO list." + CRLF)
# define RPL_LISTSTART(nick)                                 (FTIRC + " 321 " + nick + " Channel    |   Users   |   Topic" + CRLF)
# define RPL_LIST(nick, channel, users, topic)               (FTIRC + " 322 " + nick + " " + channel + " " + users + " :" + topic + CRLF)
# define RPL_LISTEND(nick)                                   (FTIRC + " 323 " + nick + " :End of /LIST" + CRLF)
# define RPL_CHANNELMODEIS(channel, mode, modeParams)        (FTIRC + " 324 * " + channel + " " + mode + " " + modeParams + CRLF)
# define RPL_NOTOPIC(nick, channel)                          (FTIRC + " 331 " + nick + " " + channel + " :No topic is set" + CRLF)
# define RPL_TOPIC(nick, channel, topic)                     (FTIRC + " 332 " + nick + " " + channel + " :" + topic + CRLF)
# define RPL_INVITING(nick, recipient, channel)                         (FTIRC + " 341 " + nick + " " + recipient + " :" + channel + CRLF)
# define RPL_WHOREPLY(channel, user, nick, flags, realname)  (FTIRC + " 352 " + channel + " " + user + " 42sp.org.br ft.irc " + nick + " " + flags + ":0 " + realname + CRLF)
# define RPL_NAMREPLY(nick, channel, names)                  (FTIRC + " 353 " + nick + " = " + channel + " :" + names + CRLF)
# define RPL_ENDOFNAMES(nick, channel)                       (FTIRC + " 366 " + nick + " " + channel + " : End of names list" + CRLF)

# define ERR_NOSUCHCHANNEL(channel)                      (FTIRC + " 403 * " + channel + " :Invalid channel name!" + CRLF)
# define ERR_CANNOTSENDTOCHAN(nick, chan)				         (FTIRC + " 404 " + nick + " " + chan + " :Cannot send to channel" + CRLF)
# define ERR_NOSUCHNICK(recipient)                       (FTIRC + " 406 " + recipient + " :No such nick" + CRLF)
# define ERR_NORECIPIENT(user)                           (FTIRC + " 411 " + user + " :No recipient to message" + CRLF)
# define ERR_NOTEXTTOSEND(user)                          (FTIRC + " 412 " + user + " :No message to send" + CRLF)
# define ERR_NONICKNAMEGIVEN()                           (FTIRC + " 431 :No nickname given" + CRLF)
# define ERR_ERRONEUSNICKNAME(nick)                      (FTIRC + " 432 * " + nick + " :Nickname is invalid" + CRLF)
# define ERR_NICKNAMEINUSE(nick)                         (FTIRC + " 433 * " + nick + " :Nickname is already in use" + CRLF)
# define ERR_USERNOTINCHANNEL(operator, client, channel) (FTIRC + " 441 " + operator + SPACE + client + SPACE + channel + " :They aren't on that channel" + CRLF)
# define ERR_NOTONCHANNEL(channel)                       (FTIRC + " 442 * " + channel + " :Client not on channel!" + CRLF)
# define ERR_USERONCHANNEL(nick, channel)                (FTIRC + " 443 * " + nick + " " + channel + " :Client on channel!" + CRLF)
# define ERR_NEEDMOREPARAMS(command, reason)             (FTIRC + " 461 * " + command + " :" + reason + CRLF)
# define ERR_ALREADYREGISTERED(user)                     (FTIRC + " 462 " + user + " :User already registered" + CRLF)
# define ERR_PASSWDMISMATCH()                            (FTIRC + " 464 * :Password was either not given or was incorrect" + CRLF)
# define ERR_CHANNELISFULL(channel)                      (FTIRC + " 471 * " + channel + " :Channel is full!" + CRLF)
# define ERR_UNKNOWNMODE(nick, modechar)                 (FTIRC + " 472 " + nick + " " + modechar + " :is unknown mode char to me" + CRLF)
# define ERR_INVITEONLYCHAN(channel)                     (FTIRC + " 473 * " + channel + " :Channel is invite-only!" + CRLF)
# define ERR_BADCHANNELKEY(user, channel)                (FTIRC + " 475 " + user + " " + channel + " :Password for channel was either not given or incorrect" + CRLF)
# define ERR_CHANOPRISNEEDED(user, channel)              (FTIRC + " 482 " + user + " " + channel + " :You're not a channel operator!" + CRLF)
# define ERR_UMODEUNKNOWNFLAG(nick)                      (FTIRC + " 501 " + nick + " MODE :Unknown MODE flag" + CRLF)

# define JOIN(user, channel)                                         (COLON + user + " JOIN " + channel + CRLF)
# define PRIVMSG_BROADCAST(nick, user, channel, topic)               (COLON + nick + "!~" + user + "@ft.irc TOPIC " + channel + SPACE + topic + CRLF)
# define RPL_PRIVMSG(user, dest, message)                            (COLON + user + " PRIVMSG " + dest + " :" + message + CRLF)
# define RPL_PARTMSG(nick, user, dest, message)                      (COLON + nick + "!~" + user + "@* PART " + dest + " :" + message + CRLF)
# define RPL_PARTNOMSG(nick, user, dest)                             (COLON + nick + "!" + user + "@* PART " + dest + CRLF)
# define RPL_KICKREASON(op_nick, op_user, channel, client, reason)   (COLON + op_nick + "!" + op_user + "@ft.irc KICK " + channel + SPACE + client + SPACE + COLON + reason + CRLF)
# define RPL_KICKNOREASON(op_nick, op_user, channel, client)         (COLON + op_nick + "!" + op_user + "@ft.irc KICK " + channel + SPACE + client + CRLF)
# define RPL_KICKFEEDBACK(op_nick, op_user, channel, client)         (COLON + op_nick + "!" + op_user + "@ft.irc KICK " + channel + SPACE + client + CRLF)
# define RPL_MODEBASE(nick, user, channel)                           (COLON + nick + "!" + user + "@ft.irc MODE " + channel + " ")
# define RPL_INVITEMSG(nick, user, recipient, channel)               (COLON + nick + "!~" + user + "@ft.irc INVITE " + recipient + " :" + channel + CRLF)


#endif
