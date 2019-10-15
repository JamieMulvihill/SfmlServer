#include "Level.h"
Level::Level(sf::RenderWindow* hwnd)
{
	window = hwnd;
}
Level::~Level()
{
}
void Level::update()
{
}
void Level::handleInput()
{
}
void Level::render()
{
	beginDraw();
	endDraw();
}
void Level::beginDraw()
{
	window->clear(sf::Color::Black);
}
void Level::endDraw()
{
	window->display();
}