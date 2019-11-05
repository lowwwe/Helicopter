/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();
	void animateHelicopter();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	

	sf::Texture	m_heloTexture; // 4 frame sprite sheet of helicopter
	sf::Sprite m_heloSprite; // sprite used to draw helo
	sf::Vector2f m_heloPosition; // position of helicopter geometric centre
	float m_heloFrameIncrement{ 0.25f }; // number of frames of animation we go throught per frame of game 1/60
	float m_heloFrameCounter{ 0.0f }; // current frame number as decimal
	int m_heloFrame{ 0 }; // current frame for animation an integer
	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

