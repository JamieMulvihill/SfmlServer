#include "Level.h"
#include <SFML/Network.hpp>
#include "Connection.h"
#include <list>

// The IP address of the server
#define SERVERIP "127.0.0.1"

// The TCP port number for the server to listen on
#define SERVERPORT 5555

// The (fixed) size of message that we send between the two programs
#define MESSAGESIZE 40

void main()
{

	// Create window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Server");
	// Initialise level object
	Level level(&window);
	
	sf::TcpSocket server_Socket_;
	sf::TcpListener listener;
	listener.setBlocking(false);
	//sf::TcpSocket client_;
	sf::SocketSelector selector_;
	int max_players = 4;
	
	// bind the listener to a port
	if (listener.listen(SERVERPORT) != sf::Socket::Done)
	{
		// error handling for binding server to port 
		printf("Server has not been bound to port");
	}

	selector_.add(listener);
	std::list<Connection *> conns;
	// Game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				window.setView(sf::View(sf::FloatRect(0, 0,
					(float)event.size.width, (float)event.size.height)));
				break;
			default:
				// don't handle other events
				break;
			}
		}

		// Make the selector wait for data on any socket
		if (selector_.wait())
		{
			// Test the listener
			if (selector_.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client_ = new sf::TcpSocket;
				if (listener.accept(*client_) == sf::Socket::Done)
				{
					if (conns.size() < max_players) {
						// Add the new client to the clients list
						conns.push_back(new Connection(client_));
						// Add the new client to the selector so that we will
						// be notified when he sends something
						selector_.add(*conns.back()->sock());
						std::string welcome_message = "Connected to Server.";
						size_t msg_size = welcome_message.size();
						client_->send(welcome_message.c_str(), msg_size);

					}
					else {
						std::string error_message = "Server Full!... try again later.";
						size_t msg_size = error_message.size();
						client_->send(error_message.c_str(), msg_size);
						delete client_;
					}
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					delete client_;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				for (std::list<Connection*>::iterator it = conns.begin(); it != conns.end(); ++it)
				{
					Connection& client = **it;
					if (selector_.isReady(*client.sock()))
					{
						// this socket is ready, you can receive (or accept if it's a listener)
						if (client.wantRead()) {
							client.doRead();
							std::cout << "Connection addres: " << client.sock()->getRemoteAddress() << std::endl;
							std::cout << "No. of Connections: " << conns.size() << std::endl;
							printf("Recieving msg\n");
						}
					}
				}
			}
		}
//***** Accpept Clients
				// using the listner to accept new clients and storing in socket
			//	selector_.add(listener);
		//sf::TcpSocket client_;
		//if (listener.accept(client_) == sf::Socket::Done)
		//{
		//	// error handling for accepting new clients
		//	printf("client to accept\n");

		//	conns.push_back(new Connection(&client_));
		//	selector_.add(*conns.back()->sock());
		//}





		////***** RECIEVE
		//		// recieve the position information from the client, store into a char array and seperate into two seperate strings for X and Y poions
		//char data[MESSAGESIZE];
		//std::size_t received;
		//std::string positionX, positionY;
		////if (selector_.wait(sf::seconds(15.f))) {

		//if (selector_.wait())
		//{
		//	for (Connection * conect : conns) {

		//		if (selector_.isReady(*conect->sock()))
		//		{
		//			// this socket is ready, you can receive (or accept if it's a listener)
		//			if (conect->wantRead()) {
		//				conect->doRead();
		//				std::cout << "Connection addres: " << conect->sock()->getRemoteAddress() << std::endl;
		//				printf("Recieving msg\n");
		//			}
		//		}
		//	}
		//}

		// TCP socket:
		for (Connection * client : conns) {

			if (client->wantWrite()) {
				client->doWrite();
			}
			
		}


		//std::list<Connection*>::iterator end_connection = conns.end();
		for (std::list<Connection*>::iterator it = conns.begin(); it != conns.end();)
		{

			Connection* client = *it;
			sf::Socket::Status status_ = client->getStatus();
			if (status_ == sf::Socket::Disconnected) {
				it = conns.erase(it);
				printf("Connection Terminated with client\n");
				std::cout << "No. of Connections: " << conns.size() << std::endl;
			}
			else {
				++it;
			}
		}

			//if (client_.receive(data, MESSAGESIZE, received) != sf::Socket::Done)
			//{
			//	// error handling for server recieve
			//	printf("Failed to recieve msg, program blocked until recieves info");
			//}
			//std::cout << "Received " << received << " bytes" << std::endl;
			//std::cout << "Received from: " << client_.getRemoteAddress() << std::endl;

			//bool stillX = true;
			//for (int i = 0; i < received; i++) {
			//	if (stillX && data[i] != '#') {
			//		positionX += data[i];
			//	}
			//	else if (!stillX && data[i] != '#') {
			//		positionY += data[i];
			//	}
			//	else {
			//		stillX = false;
			//	}
			//}
		//}
		//else {
		//	printf("Connection Timed out");
		//	std::string disconnect_msg = "Conection Timed out, Client disconnected";
		//	size_t size_ = disconnect_msg.size();
		//	if (client_.send(disconnect_msg.c_str(), size_) != sf::Socket::Done)
		//	{
		//		// error handling of send
		//		printf("Sending failed from server.\n");
		//	}
		//	client_.disconnect();
		//}
	//***** Sending
			//creating string to store the posion into to send back to the client, serperate the x and y value by #
			//std::string position_info;
			//
			//position_info.append(positionX);
			//position_info.append("#");
			//position_info.append(positionY);
			//size_t msg_size = position_info.size();

			//if (client_.send(position_info.c_str(), msg_size) != sf::Socket::Done)
			//{
			//	// error handling of send
			//	printf("Sending failed from server.\n");
			//}
			//std::cout << "Sent " << msg_size << " bytes" << std::endl;

	//***** Game Updates
			level.handleInput();
			level.update();
			level.render();
		
	}
}
