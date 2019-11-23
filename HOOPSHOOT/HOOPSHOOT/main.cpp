/*#include "SFML/Graphics.hpp"
#include<iostream>
#include "Menu.h"
int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "HOOPSHOOT");
	Menu text(window.getSize().x, window.getSize().y);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyReleased:
				switch (event.key.code) {
				case sf::Keyboard::Up:
					text.Moveup();
					break;
				case sf::Keyboard::Down:
					text.Movedown();
					break;
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Keyboard::Return:
					switch (text.GetpressedItem()) {
					case 0:
						std::cout << "Play" << std::endl;
						break;
					case 1:
						std::cout << "Options" << std::endl;
						break;
					case 2:
						std::cout << "How to play" << std::endl;
						break;
					case 3:
						window.close();
						break;

					} 
				break;
				
			
				}
			
			}
		}
		window.clear();
		text.draw(window);
		window.display();
	}  
}
*/