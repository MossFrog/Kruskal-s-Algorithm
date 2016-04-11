#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;


int main()
{
	//-- Create the render Window --//
	sf::RenderWindow mainWindow(sf::VideoMode(1024, 700), "Kruskal's Minimum Spanning Tree", sf::Style::Close);
	//-- Limit the given framerate --//
	mainWindow.setFramerateLimit(60);
	//-- Disable Key Repetition to enable button pressed events. --//
	mainWindow.setKeyRepeatEnabled(false);

	//-- Declarations Section --//
	//--------------------------//

	sf::Vector2i localPosition;

	sf::Texture gemTexture;
	gemTexture.loadFromFile("SphereGem.png");

	sf::Sprite gemSprite;
	gemSprite.setTexture(gemTexture);
	gemSprite.setOrigin(6, 6);
	gemSprite.setScale(3, 3);

	sf::RectangleShape primitiveLine;

	vector<sf::Vector2i> pointVector;
	vector<sf::VertexArray> lineVector;

	//-- Temporary active node storage vector --//
	vector<sf::Vector2i> activeTemp;


	//-- Main Game Loop --//
	while (mainWindow.isOpen())
	{
		//-- Event Methods --//
		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainWindow.close();

			//-- If the specified key is pressed push the player upwards --//
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					bool validPos = true;

					for (int i = 0; i < pointVector.size(); i++)
					{
						if (sqrt(pow((localPosition.x - pointVector[i].x), 2) + pow((localPosition.y - pointVector[i].y), 2)) < 25)
						{
							validPos = false;
						}
					}

					if (validPos)
					{
						//-- Place a pinpoint on the given click (add it to the main vector) --//
						pointVector.push_back(localPosition);
					}

				}


				//-- Right click to select two nodes to link --//
				if (event.key.code == sf::Mouse::Right)
				{
					//-- Search for a close local point, if found then anchor and set it as "active" --//
					for (int i = 0; i < pointVector.size(); i++)
					{
						if (sqrt(pow((localPosition.x - pointVector[i].x), 2) + pow((localPosition.y - pointVector[i].y), 2)) < 25)
						{
							if (activeTemp.size() < 2)
							{

								if (activeTemp.size() == 1)
								{
									if (activeTemp[0] != pointVector[i])
									{
										activeTemp.push_back(pointVector[i]);
										cout << "done" << endl;

										//-- Clear the "active temporary" vector after the line is added to the lineVector --//
										sf::VertexArray tempLine(sf::Lines, 2);

										tempLine[0].position = sf::Vector2f(activeTemp[1].x, activeTemp[1].y);
										tempLine[1].position = sf::Vector2f(activeTemp[0].x, activeTemp[0].y);

										tempLine[0].color = sf::Color::Red;
										tempLine[1].color = sf::Color::Blue;


										lineVector.push_back(tempLine);

										activeTemp.clear();

									}
								}

								else
								{
									activeTemp.push_back(pointVector[i]);
									cout << "done" << endl;
								}

							}
						}
					}
				}
			}
		}

		localPosition = sf::Mouse::getPosition(mainWindow);

		//-- Debugging Section --//
		//cout << localPosition.x << " " << localPosition.y << endl;


		mainWindow.clear(sf::Color::Black);


		//-- Draw all the given sprites --//
		for (int i = 0; i < pointVector.size(); i++)
		{
			gemSprite.setPosition(pointVector[i].x, pointVector[i].y);
			mainWindow.draw(gemSprite);
		}

		for (int i = 0; i < lineVector.size(); i++)
		{
			mainWindow.draw(lineVector[i]);
		}







		mainWindow.display();
	}

	//-- END PROGRAM --//
	return 0;
}