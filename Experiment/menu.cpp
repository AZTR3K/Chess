#include "menu.h"

MainMenu::MainMenu(float width, float height)
{
	if (!font.loadFromFile("fonts/Alexandriaflf.ttf"))
	{
		cout << "No font is here";
	}

	// Play
	mainMenu[0].setFont(font);
	mainMenu[0].setFillColor(Color::Black);
	mainMenu[0].setString("Play");
	mainMenu[0].setCharacterSize(250);
	mainMenu[0].setPosition(2000, 400);

	// Exit
	mainMenu[1].setFont(font);
	mainMenu[1].setFillColor(Color::Black);
	mainMenu[1].setString("Exit");
	mainMenu[1].setCharacterSize(250);
	mainMenu[1].setPosition(2000, 1000);

	MainMenuSelected = -1;
}

MainMenu::~MainMenu()
{
	cout << "Destructor called" << endl;
}


// Draw MainMenu
void MainMenu::draw(RenderWindow& window)
{
	for (int i = 0; i < Max_main_menu; i++)
	{
		window.draw(mainMenu[i]);
	}
}

void MainMenu::handleMouseInput(const Vector2f& mousePosition)
{
	for (int i = 0; i < Max_main_menu; i++)
	{
		if (mainMenu[i].getGlobalBounds().contains(mousePosition))
		{
			// Highlight the menu item under the mouse
			mainMenu[MainMenuSelected].setFillColor(Color::White);
			MainMenuSelected = i;
			mainMenu[MainMenuSelected].setFillColor(Color::Blue);
			break;
		}
	}
}