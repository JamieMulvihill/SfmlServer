#include "Connection.h"
#include <iostream>

// Constructor.
// sock: the socket that we've accepted the client connection on.
Connection::Connection(sf::TcpSocket *sock)
	:readCount_(0), readWriteState(READ)
{
	printf("New connection\n");
	sock_ = sock;
}

// Destructor.
Connection::~Connection()
{
	printf("Closing connection\n");
	status_ = sf::Socket::Disconnected;
	sock_->disconnect();
}

sf::TcpSocket* Connection::sock()
{
	return sock_;
}

// Return whether this connection is in a state where we want to try
// reading from the socket.
bool Connection::wantRead()
{
	// At present, we always do.
	if (readWriteState == READ)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Call this when the socket is ready to read.
bool Connection::doRead()
{
	// Receive as much data from the client as will fit in the buffer.
	//size_t spaceLeft = (sizeof readBuffer_) - readCount_;
	std::string positionX, positionY;

	status_ = sock_->receive(readBuffer_, MESSAGESIZE, recieved);

	if (status_ == sf::Socket::Disconnected)
	{
		//sock_->disconnect();
		printf("Socket no longer connected");
		return true;
		
	}
	else if (status_ != sf::Socket::Done) {
		// error handling for server recieve
		printf("Failed to recieve msg, program blocked until recieves info");
		return true;
	}
	else {
		bool stillX = true;

		for (int i = 0; i < recieved; i++) {

			if (stillX && readBuffer_[i] != '#') {
				positionX += readBuffer_[i];
			}
			else if (!stillX && readBuffer_[i] != '#') {
				positionY += readBuffer_[i];
			}
			else {
				stillX = false;
			}
		}


		// We've successfully read some more data into the buffer.
		//readCount_ += recieved;

		//if (readCount_ < MESSAGESIZE) {
			// ... but we've not received a complete message yet.
			// So we can't do anything until we receive some more.
		//	return false;
		//}

		// We've got a complete message.
		printf("Received message from the client:");
		//fwrite(readBuffer_, 1, MESSAGESIZE, stdout);
		std::cout << positionX << " " << positionY << std::endl;
		printf("\n");

		if (memcmp(readBuffer_, "quit", 4) == 0) {
			printf("Client asked to quit\n");
			return true;
		}
	}
	// Clear the buffer, ready for the next message.
	readCount_ = 0;
	readWriteState = WRITE;
	return false;
}
bool Connection::wantWrite()
{
	// At present, we always do.
	if (readWriteState == WRITE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Call this when the socket is ready to write.
bool Connection::doWrite()
{
	// Send the same data back to the client.
	// FIXME: the socket might not be ready to send yet -- so this could block!
	// FIXME: and we might not be able to write the entire message in one go...
	std::string positionX, positionY;

	bool stillX = true;

	for (int i = 0; i < recieved; i++) {

		if (stillX && readBuffer_[i] != '#') {
			positionX += readBuffer_[i];
		}
		else if (!stillX && readBuffer_[i] != '#') {
			positionY += readBuffer_[i];
		}
		else {
			stillX = false;
		}
	}

	std::string position_info;

	position_info.append(positionX);
	position_info.append("#");
	position_info.append(positionY);
	size_t msg_size = position_info.size();

	if (sock_->send(position_info.c_str(), msg_size) != sf::Socket::Done)
	{
		// error handling of send
		printf("Sending failed from server.\n");
		return true;
	}
	printf("sending msg\n");
	// Clear the buffer, ready for the next message.
	readCount_ = 0;
	readWriteState = READ;
	return false;
}

