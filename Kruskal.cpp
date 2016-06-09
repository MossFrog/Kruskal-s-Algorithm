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

	//-- Structure for storing the "Tree/Node" data --//
	struct Node
	{
		int TreeID;
		int Xpos;
		int Ypos;
	};

	struct Edge
	{
		sf::Vector2i vertexOne;
		sf::Vector2i vertexTwo;
		double length;
	};

	//-- Every node is assigned a Unique Tree ID in the Beginning --//
	int treeID = 0;

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
	vector<sf::VertexArray> linkedVector;

	//-- Temporary active node storage vector --//
	vector<sf::Vector2i> activeTemp;

	//-- Node vector for mathematical calculation --//
	vector<Node> nodeVect;
	vector<Edge> edgeVect;

	//-- Bool for starting the calculations --//
	bool calcStarted = false;

	int delayAmount = 0;
	int solutionIndex = 0;


	//-- Main Game Loop --//
	while (mainWindow.isOpen())
	{
		//-- Event Methods --//
		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainWindow.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (!calcStarted)
				{
					if (event.key.code == sf::Mouse::Left)
					{
						bool validPos = true;

						for (int i = 0; i < pointVector.size(); i++)
						{
							if (sqrt(pow((localPosition.x - pointVector[i].x), 2) + pow((localPosition.y - pointVector[i].y), 2)) < 100)
							{
								validPos = false;
							}
						}

						if (validPos)
						{
							//-- Place a pinpoint on the given click (add it to the main vector) --//
							pointVector.push_back(localPosition);

							//-- Update the Node Vector --//
							Node newNode;
							newNode.TreeID = treeID;
							treeID++;

							newNode.Xpos = localPosition.x;
							newNode.Ypos = localPosition.y;

							nodeVect.push_back(newNode);

							cout << "- Node Added -" << endl;
							cout << "X - Position: " << newNode.Xpos << endl;
							cout << "Y - Position: " << newNode.Ypos << endl;
							cout << "TreeID: " << treeID << endl;
							cout << endl << endl;
						}

					}


					//-- Right click to select two nodes to link --//
					if (event.key.code == sf::Mouse::Right)
					{
						//-- Search for a close local point, if found then anchor and set it as "active" --//
						for (int i = 0; i < pointVector.size(); i++)
						{
							if (sqrt(pow((localPosition.x - pointVector[i].x), 2) + pow((localPosition.y - pointVector[i].y), 2)) < 100)
							{
								if (activeTemp.size() < 2)
								{

									if (activeTemp.size() == 1)
									{
										if (activeTemp[0] != pointVector[i])
										{
											activeTemp.push_back(pointVector[i]);

											//-- Clear the "active temporary" vector after the line is added to the lineVector --//
											sf::VertexArray tempLine(sf::Lines, 2);

											tempLine[0].position = sf::Vector2f(activeTemp[1].x, activeTemp[1].y);
											tempLine[1].position = sf::Vector2f(activeTemp[0].x, activeTemp[0].y);

											tempLine[0].color = sf::Color::Red;
											tempLine[1].color = sf::Color::Blue;

											lineVector.push_back(tempLine);

											activeTemp.clear();


											//-- Add the new line to the Edge Vector, make sure it doesn't already exist. --//
											bool valid = true;
											for (int i = 0; i < edgeVect.size(); i++)
											{
												if ((edgeVect[i].vertexOne.x == activeTemp[1].x) && (edgeVect[i].vertexOne.y == activeTemp[1].y))
												{
													if (((edgeVect[i].vertexTwo.x == activeTemp[0].x) && (edgeVect[i].vertexTwo.y == activeTemp[0].y)))
													{
														valid = false;
													}
												}

												if ((edgeVect[i].vertexTwo.x == activeTemp[1].x) && (edgeVect[i].vertexTwo.y == activeTemp[1].y))
												{
													if (((edgeVect[i].vertexOne.x == activeTemp[0].x) && (edgeVect[i].vertexOne.y == activeTemp[0].y)))
													{
														valid = false;
													}
												}
											}

											//-- No duplicates found, add the edge to the Vector --//

											if (valid)
											{
												Edge newEdge;
												newEdge.length = sqrt(pow((activeTemp[1].x - activeTemp[0].x), 2) + pow((activeTemp[1].y - activeTemp[0].y), 2));
												newEdge.vertexOne.x = activeTemp[1].x;
												newEdge.vertexOne.y = activeTemp[1].y;

												newEdge.vertexTwo.x = activeTemp[0].x;
												newEdge.vertexTwo.y = activeTemp[0].y;

												edgeVect.push_back(newEdge);

												//-- Output Edge Data to the console --//
												cout << "- Edge Created --" << endl;
												cout << "Vertex - 1: " << activeTemp[1].x << " " << activeTemp[1].y << endl;
												cout << "Vertex - 2: " << activeTemp[0].x << " " << activeTemp[0].y << endl;
												cout << "Length: " << newEdge.length << endl << endl;
											}

										}
									}

									else
									{
										activeTemp.push_back(pointVector[i]);
									}

								}
							}
						}
					}
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					delayAmount = 500;

					//-- Inefficiently Sort the edge Vector from small to large --//

					if (!calcStarted)
					{
						for (int i = 0; i < edgeVect.size(); i++)
						{
							for (int j = i; j < edgeVect.size(); j++)
							{
								if (edgeVect[i].length > edgeVect[j].length)
								{
									swap(edgeVect[i], edgeVect[j]);
								}
							}
						}
					}

					//-- Prevent additional nodes from being added and Start the Calculation --//
					calcStarted = true;
				}
			}
		}

		localPosition = sf::Mouse::getPosition(mainWindow);

		//-- Do the necessary calculation here --//

		if (calcStarted)
		{
			for (int i = 0; i < nodeVect.size(); i++)
			{
				if (edgeVect[solutionIndex].vertexOne.x == nodeVect[i].Xpos)
				{
					if (edgeVect[solutionIndex].vertexOne.y == nodeVect[i].Ypos)
					{
						for (int j = 0; j < nodeVect.size(); j++)
						{
							if (edgeVect[solutionIndex].vertexTwo.x == nodeVect[j].Xpos)
							{
								if (edgeVect[solutionIndex].vertexTwo.y == nodeVect[j].Ypos)
								{
									if (nodeVect[j].TreeID != nodeVect[i].TreeID)
									{
										//-- Convert all nodes to the new treeID preventing loop creation in the next iteration --//
										for (int y = 0; y < nodeVect.size(); y++)
										{
											if ((nodeVect[y].TreeID == nodeVect[j].TreeID) && j != y)
											{
												nodeVect[y].TreeID = nodeVect[i].TreeID;
											}
										}
										nodeVect[j].TreeID = nodeVect[i].TreeID;

										//-- Add a new overlay to the linkedVector --//
										sf::VertexArray tempLine(sf::Lines, 2);

										tempLine[0].position = sf::Vector2f(edgeVect[solutionIndex].vertexOne.x, edgeVect[solutionIndex].vertexOne.y);
										tempLine[1].position = sf::Vector2f(edgeVect[solutionIndex].vertexTwo.x, edgeVect[solutionIndex].vertexTwo.y);

										tempLine[0].color = sf::Color::Yellow;
										tempLine[1].color = sf::Color::White;

										linkedVector.push_back(tempLine);
									}
								}
							}
						}
					}
				}
			}

			//-- Increment the solution index (Edge to check in the next iteration) --//
			solutionIndex++;
		}

		//-- Debugging Section --//
		//cout << localPosition.x << " " << localPosition.y << endl;

		//-- Delay Section (Only Active When Calculating) --//

		Sleep(delayAmount);

		mainWindow.clear(sf::Color::Black);


		//-- Draw all the given Sprites and Primitives --//
		for (int i = 0; i < lineVector.size(); i++)
		{
			mainWindow.draw(lineVector[i]);
		}

		for (int i = 0; i < linkedVector.size(); i++)
		{
			mainWindow.draw(linkedVector[i]);
		}

		for (int i = 0; i < pointVector.size(); i++)
		{
			gemSprite.setPosition(pointVector[i].x, pointVector[i].y);
			mainWindow.draw(gemSprite);
		}



		//-- Call the display method --//
		mainWindow.display();
	}

	//-- END PROGRAM --//
	return 0;
}