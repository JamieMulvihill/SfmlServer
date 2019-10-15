#pragma once

#include <SFML/Network.hpp>

// The (fixed) size of message that we send between the two programs
#define MESSAGESIZE 40

enum CONNECTION_STATE { READ, WRITE };

class Connection
{
public:
	// Constructor.
	// sock: the socket that we've accepted the client connection on.
	Connection(sf::TcpSocket *sock);

	// Destructor.
	~Connection();

	// Return the client's socket.
	sf::TcpSocket* sock();

	// Return whether this connection is in a state where we want to try
	// reading from the socket.
	bool wantRead();

	// Call this when the socket is ready to read.
	bool doRead();

	// Return whether this connection is in a state where we want to try
	// writing from the socket.
	bool wantWrite();

	// Call this when the socket is ready to write.
	bool doWrite();
	sf::Socket::Status getStatus() { return status_; }


	CONNECTION_STATE readWriteState;
private:
	sf::TcpSocket * sock_;
	size_t recieved;
	sf::Socket::Status status_;
	// The data we've read from the client.
	int readCount_;
	char readBuffer_[MESSAGESIZE];

};

