/*#pragma once
#include "SFML/Graphics.hpp"
constexpr int MAX_NUMBER_OF_ITEMS = 4;
class Menu {
public:
	Menu(float width, float height);
	~Menu(); 
	void draw(sf::RenderWindow& window);
	void Moveup();
	void Movedown();
	int GetpressedItem() { return selectedItemIndex; }
private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text text[MAX_NUMBER_OF_ITEMS];
	
};*/