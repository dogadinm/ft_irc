
# Description
This project is about creating IRC server. We used an actual IRC client to connect to our server and test it.
**IRC** (Internet Relay Chat) is a protocol for real-time text messaging between internet-connected computers created in **1988**. It is mainly used for group discussion in chat rooms called “**channels**” although it supports private messages between two users and various client-side commands.

## Mandatory part
  - We have to develop an IRC server in C++ 98.
  - The server must be capable of handling multiple clients at the same time and never hang.
  - Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).
  - An IRC client must be able to connect to your server without encountering any error.
  - Communication between client and server has to be done via TCP/IP (v4).
  - Using the IRC client with our server must be similar to using it with any official IRC server. However, we only have to implement the following features:
    -  We must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using the IRC client.
    -  All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
    -  We must have operators and regular users.
    -  Then, we have to implement the commands that are specific to operators.

# IRC Commands
These are the available commands in our IRC Server:
  - **PASS**: The **PASS** command is used to set a 'connection password'.
  - **NICK**: NICK command is used to give user a nickname or change the existing one.
  - **USER**: he USER command is used at the beginning of connection to specify the username, hostname and realname of a new user.
  - **OPER**: A normal user uses the OPER command to obtain operator privileges.
  - **MODE**: The user MODE's are typically changes which affect either how the client is seen by others or what 'extra' messages the client is sent.
  - **QUIT**: A client session is terminated with a quit message.
  - **JOIN**: The JOIN command is used by a user to request to start listening to the specific channel.
  - **PART**: The PART command causes the user sending the message to be removed from the list of active members for all given channels listed in the parameter string.
  - **TOPIC**: The TOPIC command is used to change or view the topic of a channel.
  - **KICK**: The KICK command can be used to request the forced removal of a user from a channel.
  - **PRIVMSG**: PRIVMSG is used to send private messages between users, as well as to send messages to channels.
  - **NOTICE**: The NOTICE command is used similarly to PRIVMSG.
  - **kill**: Disconnect a user from the IRC server(need oprer privileges).
  - **LIST**: Displays a list of channels currently available on the IRC server, along with the number of users in each channel and a brief topic.
  - **WHOIS**: Provides detailed information about a specific user.
  - **NAMES**: Lists all users currently in a specific channel.
  - **INVITE**: Invites a user to join a specific channel, typically used for invite-only channels.

# ⚙️ Start IRC Server
To compile the program, use:
  - `make`
To start the Server, use:
  - `./ircserv <port> <password>`
    - **port**: The port number on which your IRC server will be listening to for incoming IRC connections.
    - **password**: The connection password. It will be needed by any IRC client that tries to connect to your server.
To connect to the server, you can use:
  - `nc <IP ADDRESS> <PORT>`
    - **IP ADDRESS**: Host IP address.
    - **PORT**: The PORT that the server listening on.
  - to register:
    "PASS <pass>"
    "NICK <nickname>"
    "USER <username> 0 * :<realname>"

  - You can also use an IRC Client e.g IRSSI
    -to connect with IRSSI: "`/connect <IP ADDRESS> <PORT> <PASSWORD> <NICKNAME>`"
