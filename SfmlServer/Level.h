#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
class Level {
public:
	Level(sf::RenderWindow* hwnd);
	~Level();
	void handleInput();
	void update();
	void render();
private:
	sf::RenderWindow* window;
	void beginDraw();
	void endDraw();
};