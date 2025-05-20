#pragma once
#include <vector>
#include <memory>
#include <random>
#include "Block.h"
#include "Bonus.h"
#include "Constants.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void setupText(sf::Text& text, const std::string& str, float x, float y, unsigned int size = FONT_SIZE);
    void initGameObjects();
    void initUI();
    void createBlocks();
    void resetBall();
    bool checkWinCondition();
    void updateBall(float deltaTime);
    void handleBallLoss();
    void checkCollisions();
    void handlePaddleCollision();
    void handleBlockCollision(Block& block);
    void updateBonuses(float deltaTime);
    void applyBonus(BonusType type);
    void updateUI();
    void initGame();

    sf::RenderWindow window;
    sf::Font font;
    sf::RectangleShape paddle;
    sf::CircleShape ball;
    std::vector<std::unique_ptr<Block>> blocks;
    std::vector<std::unique_ptr<Bonus>> activeBonuses;
    sf::Vector2f ballVelocity;
    float ballSpeed;
    float paddleWidth;
    int score;
    int lives;
    bool gameOver;
    bool gameWon;
    bool stickyAvailable;
    bool ballStuck;
    bool bottomShieldActive;
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    sf::Text scoreText;
    sf::Text livesText;
    sf::Text gameOverText;
    sf::Text winText;
    sf::Text stickyText;
    sf::RectangleShape bottomShieldIndicator;
};