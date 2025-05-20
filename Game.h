#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Tile.h"

const int GRID_WIDTH = 8;
const int GRID_HEIGHT = 8;
const int WINDOW_WIDTH = GRID_WIDTH * TILE_SIZE;
const int WINDOW_HEIGHT = GRID_HEIGHT * TILE_SIZE + 50;
const int BONUS_RADIUS = 3;
const float FALL_SPEED = 0.5f;
const float BONUS_ANIMATION_SPEED = 0.08f;
const int WIN_SCORE = 1500;
const int MAX_MOVES = 40;
const int MAX_BONUSES = 3;

class Game {
private:
    sf::RenderWindow window;
    std::vector<std::vector<Tile>> grid;
    sf::Vector2i selectedTile;
    bool isFirstTileSelected;
    bool isAnimating;
    bool gameOver;
    bool gameWon;
    int score;
    int moves;
    bool scoreActive;
    int currentBonusCount; 
    sf::Font font;
    sf::Text scoreText;
    sf::Text movesText;
    sf::Text gameOverText;
    sf::Text gameWinText;
    sf::RectangleShape background;
    std::mt19937 rng;

    void initUI();
    void setupText(sf::Text& text, const std::string& str, float x, float y, int size);
    void initializeGrid();
    void resetGame();
    void checkGameState();
    bool checkMatches();
    void removeMatches();
    void fillEmptyTiles();
    void processMatches();
    bool checkPotentialMatches(const std::vector<std::vector<Tile>>& tempGrid, int row, int col);
    void swapTiles(int row1, int col1, int row2, int col2);
    void activateBonus(int row, int col);
    void applyRecolorBonus(int row, int col);
    void applyBombBonus(int row, int col);
    void handleMouseClick(int mouseX, int mouseY);

public:
    Game();
    void run();
    void handleEvents();
    void update();
    void render();
};