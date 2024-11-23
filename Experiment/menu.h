#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

#define Max_main_menu 2
class MainMenu
{
public:
	MainMenu(float width, float height);

	void draw(RenderWindow& window);
	void handleMouseInput(const Vector2f& mousePosition);

	int MainMenuPressed()
	{
		return MainMenuSelected;
	}

	~MainMenu();

private:
	int MainMenuSelected;
	Font font;
	Text mainMenu[Max_main_menu];
};
