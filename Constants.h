#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

#define M_PI 3.1415926535897932384f

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const float INITIAL_PADDLE_WIDTH = 100.0;
const float PADDLE_HEIGHT = 20.0;
const float BALL_RADIUS = 10.0;
const float BLOCK_WIDTH = 80.0;
const float BLOCK_HEIGHT = 30.0;
const float BONUS_WIDTH = 30.0;
const float BONUS_HEIGHT = 20.0;
const float INITIAL_BALL_SPEED = 300.0;
const float PADDLE_SPEED = 500.0;
const float BONUS_SPEED = 150.0;
const int LOSE_PENALTY = 10;
const int WIN_SCORE = 100;
const unsigned int FONT_SIZE = 24;
const sf::Color TEXT_COLOR = sf::Color::White;
const int INITIAL_LIVES = 3;
