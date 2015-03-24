// Maze.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "SFML\Graphics.hpp"
#include "SFML\Main.hpp"
#include "SFML\Window.hpp"
#include "Box2D\Box2D.h"

#include <Windows.h>

#define SCALE 30.0f

void CreateGround(b2World& World, float X, float Y);
void CreateBox(b2World& World, int MouseX, int MouseY);

void CreateGround(b2World& World, float X, float Y)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(X/30.f, Y/30.f);
    BodyDef.type = b2_staticBody;
    b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
    Shape.SetAsBox((800.f/2)/SCALE, (16.f/2)/SCALE); // Creates a box shape. Divide your desired width and height by 2.
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;  // Sets the density of the body
    FixtureDef.shape = &Shape; // Sets the shape
    Body->CreateFixture(&FixtureDef); // Apply the fixture definition

    
}

void CreateBox(b2World& World, int MouseX, int MouseY)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(MouseX/SCALE, MouseY/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Window that will house the game
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800, 600), "Maze Game");

	// Event used to hold events the window has queued
	sf::Event eventCatcher;

	sf::Texture GroundTexture;
	sf::Texture BoxTexture;
	GroundTexture.loadFromFile("ground.png");
	BoxTexture.loadFromFile("box.png");

	// Setting up box2d World
	b2Vec2 gravity(0.0f, 10.0f);
	b2World* world = new b2World(gravity);
	

	CreateGround(*world, 400.0f, 500.0f);




	



	//Stuff for timesteps
	float timenow = (float)GetTickCount();
	float timethen = (float)GetTickCount();
	float timeelapsed = 0.0f;


	while (window.isOpen())
	{
		timeelapsed = (float)(timenow - timethen) / 1000.0f;
		// Checking window events
		while(window.pollEvent(eventCatcher))
		{
			switch (eventCatcher.type)
			{
			case sf::Event::EventType::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if(eventCatcher.key.code == sf::Mouse::Left)
					CreateBox(*world, eventCatcher.mouseButton.x,  eventCatcher.mouseButton.y);
				break;

			default:
				break;
			}
		}

	

		world->Step(timeelapsed, 6, 2);

	
		

		//Clears the window to a color and ready's for displaying
		window.clear(sf::Color::White);

		for (b2Body* BodyIterator = world->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
        {
            if (BodyIterator->GetType() == b2_dynamicBody)
            {
                sf::Sprite Sprite;
                Sprite.setTexture(BoxTexture);
                Sprite.setOrigin(16.f, 16.f);
                Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
                Sprite.setRotation(BodyIterator->GetAngle() * 180/b2_pi);
                window.draw(Sprite);
            }
            else
            {
                sf::Sprite GroundSprite;
                GroundSprite.setTexture(GroundTexture);
                GroundSprite.setOrigin(400.f, 8.f);
                GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
                GroundSprite.setRotation(180/b2_pi * BodyIterator->GetAngle());
                window.draw(GroundSprite);
            }
        }
    


		// Draws to the window
		window.display();


		timethen = timenow;
		timenow = (float)GetTickCount();
	}
	return 0;
}

