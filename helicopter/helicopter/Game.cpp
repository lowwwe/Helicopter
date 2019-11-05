/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// helicopter game to use mouse and vectors
/// </summary>

#include "Game.h"
#include "MyVector2.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonReleased == newEvent.type)
		{
			processMouseButtonUp(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	animateHelicopter();
	moveHelicopter();
	if (m_exitGame)
	{
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_welcomeMessage);
	m_window.draw(m_heloSprite);
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("AirWolf");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the helicopter
/// </summary>
void Game::setupSprite()
{
	if (!m_heloTexture.loadFromFile("ASSETS\\IMAGES\\helicopter.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_heloSprite.setTexture(m_heloTexture);
	m_heloSprite.setOrigin(sf::Vector2f{ 90.0f,32.0f }); // used so we can flip helo
	m_heloSprite.setPosition(m_heloPosition );
	m_heloSprite.setTextureRect(sf::IntRect{ 0,0,180,64 });
}

/// <summary>
/// add our frame increment to the frame counter this is done 60 times per second
/// make the frame be the interger part of the frame counter mod 4 [1,2,3,0]
/// then set the texture rectangle to the corresponding frame of the animation
/// </summary>
void Game::animateHelicopter()
{
	m_heloFrameCounter += m_heloFrameIncrement;
	m_heloFrame = static_cast<int>(m_heloFrameCounter) % 4;
	m_heloSprite.setTextureRect(sf::IntRect{ 0,m_heloFrame * 64,180,64 });
}


/// <summary>
/// if moving left stop when the x co-ordinate is less than the desired
/// we may never arrive at exact position.
/// when treaveleing add the velocity to the position to get new position
/// adjust the animation speed when we stop at new position
/// </summary>
void Game::moveHelicopter()
{
	if (Direction::Left == m_travelDirection )
	{
		m_heloPosition += m_heloVelocity;
		if (m_desiredPosition.x > m_heloPosition.x)
		{
			m_travelDirection = Direction::None;
			m_heloFrameIncrement = 0.25f;
		}		
	}
	if (Direction::Right == m_travelDirection)
	{
		m_heloPosition += m_heloVelocity;
		if (m_desiredPosition.x < m_heloPosition.x)
		{
			m_travelDirection = Direction::None;
			m_heloFrameIncrement = 0.25f;
		}
	}


	m_heloSprite.setPosition(m_heloPosition);
}


/// <summary>
/// when mouse button is released
/// get location of mouse click 
/// subtract current position to get path to new location
/// mormalise path to getunit vector velocity
/// multiply by speed
/// set direction based on difference in x co-ordinates
/// if they are the same true vertical movement then set it to none
/// adjust animation speed
/// 
/// </summary>
/// <param name="t_event"></param>
void Game::processMouseButtonUp(sf::Event t_event)
{
	sf::Vector2f   headingVector{ 0.0f,0.0f };
	m_desiredPosition = sf::Vector2f{ static_cast<float>(t_event.mouseButton.x), static_cast<float>(t_event.mouseButton.y) };
	if (m_desiredPosition.x < m_heloPosition.x)
	{
		m_travelDirection = Direction::Left;
		m_heloSprite.setScale(sf::Vector2f{ -1.0f, 1.0f });
		m_heloFrameIncrement = 0.76f;
	}
	else
	{
		m_travelDirection = Direction::Right;
		m_heloSprite.setScale(sf::Vector2f{1.0f, 1.0f});
		m_heloFrameIncrement = 0.76f;
	}
	if (m_desiredPosition.x == m_heloPosition.x)
	{
		m_travelDirection = Direction::None;
	}
	headingVector = m_desiredPosition - m_heloPosition;
	headingVector = vectorUnitVector(headingVector);
	m_heloVelocity = headingVector * m_heloSpeed;
}
