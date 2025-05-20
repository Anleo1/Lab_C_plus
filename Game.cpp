#include "Game.h"
#include <algorithm>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gems"),
isFirstTileSelected(false), isAnimating(false), score(0), moves(0),
scoreActive(false), currentBonusCount(0), gameOver(false), gameWon(false),
rng(std::random_device{}()) {

    window.setFramerateLimit(60);
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
    initUI();
    initializeGrid();
}

void Game::initUI() {
    setupText(gameOverText, "GAME OVER! Press Q to restart", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50, 34);
    setupText(gameWinText, "YOU WIN! Press Q to restart", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50, 34);

    background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - 50));
    background.setFillColor(sf::Color(50, 50, 70));
    background.setPosition(0, 0);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, WINDOW_HEIGHT - 40);
    scoreText.setString("Score: 0");

    movesText.setFont(font);
    movesText.setCharacterSize(24);
    movesText.setFillColor(sf::Color::White);
    movesText.setPosition(WINDOW_WIDTH - 120, WINDOW_HEIGHT - 40);
    movesText.setString("Moves: 0");
}

void Game::setupText(sf::Text& text, const std::string& str, float x, float y, int size) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setPosition(x, y);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Game::initializeGrid() {
    grid.resize(GRID_HEIGHT, std::vector<Tile>(GRID_WIDTH));
    std::uniform_int_distribution<int> dist(0, static_cast<int>(TileType::COUNT) - 1);

    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            grid[i][j].initialize(static_cast<TileType>(dist(rng)),
                j * TILE_SIZE + 2.0f,
                i * TILE_SIZE + 2.0f);
        }
    }

    while (checkMatches()) {
        removeMatches();
        fillEmptyTiles();
    }
}

void Game::resetGame() {
    score = 0;
    moves = 0;
    scoreActive = false;
    currentBonusCount = 0; 
    gameOver = false;
    gameWon = false;
    isFirstTileSelected = false;
    isAnimating = false;
    scoreText.setString("Score: 0");
    movesText.setString("Moves: 0");
    initializeGrid();
}

void Game::checkGameState() {
    if (score >= WIN_SCORE && !gameWon) {
        gameWon = true;
    }
    else if (moves >= MAX_MOVES && !gameOver) {
        gameOver = true;
    }
}

bool Game::checkMatches() {
    bool foundMatch = false;

    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            grid[i][j].setMatched(false);
        }
    }

    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH - 2; ++j) {
            if (grid[i][j].getType() == grid[i][j + 1].getType() &&
                grid[i][j].getType() == grid[i][j + 2].getType()) {

                int k = j;
                while (k < GRID_WIDTH && grid[i][k].getType() == grid[i][j].getType()) {
                    grid[i][k].setMatched(true);
                    foundMatch = true;
                    k++;
                }
                j = k - 1;
            }
        }
    }

    for (int j = 0; j < GRID_WIDTH; ++j) {
        for (int i = 0; i < GRID_HEIGHT - 2; ++i) {
            if (grid[i][j].getType() == grid[i + 1][j].getType() &&
                grid[i][j].getType() == grid[i + 2][j].getType()) {

                int k = i;
                while (k < GRID_HEIGHT && grid[k][j].getType() == grid[i][j].getType()) {
                    grid[k][j].setMatched(true);
                    foundMatch = true;
                    k++;
                }
                i = k - 1;
            }
        }
    }

    return foundMatch;
}

void Game::removeMatches() {
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            if (grid[i][j].isMatched()) {
                if (scoreActive) {
                    score += 5;
                    scoreText.setString("Score: " + std::to_string(score));
                }
                else {
                    scoreText.setString("Score: 0");
                }

                std::uniform_real_distribution<float> bonusDist(0.0f, 1.0f);
                if (bonusDist(rng) < 0.2f && currentBonusCount < MAX_BONUSES) {
                    std::uniform_int_distribution<int> bonusTypeDist(1, static_cast<int>(BonusType::COUNT) - 1);
                    grid[i][j].setBonus(static_cast<BonusType>(bonusTypeDist(rng)));
                    currentBonusCount++; 
                }

                grid[i][j].setType(static_cast<TileType>(-1));
            }
        }
    }
}

void Game::fillEmptyTiles() {
    for (int j = 0; j < GRID_WIDTH; ++j) {
        int emptyCount = 0;
        for (int i = GRID_HEIGHT - 1; i >= 0; --i) {
            if (grid[i][j].getType() == static_cast<TileType>(-1)) {
                emptyCount++;
            }
            else if (emptyCount > 0) {
                grid[i + emptyCount][j] = grid[i][j];
                grid[i + emptyCount][j].setFalling(true);
                grid[i + emptyCount][j].setFallDistance(static_cast<float>(emptyCount * TILE_SIZE));
                grid[i][j].setType(static_cast<TileType>(-1));
            }
        }

        std::uniform_int_distribution<int> dist(0, static_cast<int>(TileType::COUNT) - 1);
        for (int i = 0; i < emptyCount; ++i) {
            grid[i][j].initialize(static_cast<TileType>(dist(rng)),
                j * TILE_SIZE + 2.0f,
                i * TILE_SIZE + 2.0f - (emptyCount - i) * TILE_SIZE);
            grid[i][j].setFalling(true);
            grid[i][j].setFallDistance(static_cast<float>((emptyCount - i) * TILE_SIZE));
        }
    }
}

void Game::processMatches() {
    removeMatches();
    fillEmptyTiles();
    isAnimating = true;
}

bool Game::checkPotentialMatches(const std::vector<std::vector<Tile>>& tempGrid, int row, int col) {
    int left = std::max(0, col - 2);
    int right = std::min(GRID_WIDTH - 1, col + 2);

    for (int j = left; j <= right - 2; ++j) {
        if (tempGrid[row][j].getType() == tempGrid[row][j + 1].getType() &&
            tempGrid[row][j].getType() == tempGrid[row][j + 2].getType()) {
            return true;
        }
    }

    int top = std::max(0, row - 2);
    int bottom = std::min(GRID_HEIGHT - 1, row + 2);

    for (int i = top; i <= bottom - 2; ++i) {
        if (tempGrid[i][col].getType() == tempGrid[i + 1][col].getType() &&
            tempGrid[i][col].getType() == tempGrid[i + 2][col].getType()) {
            return true;
        }
    }

    return false;
}

void Game::swapTiles(int row1, int col1, int row2, int col2) {
    std::swap(grid[row1][col1], grid[row2][col2]);
    grid[row1][col1].setFalling(true);
    grid[row1][col1].setFallDistance(0);
    grid[row2][col2].setFalling(true);
    grid[row2][col2].setFallDistance(0);
}

void Game::activateBonus(int row, int col) {
    switch (grid[row][col].getBonus()) {
    case BonusType::RECOLOR:
        applyRecolorBonus(row, col);
        break;
    case BonusType::BOMB:
        applyBombBonus(row, col);
        break;
    default:
        break;
    }

    grid[row][col].setBonus(BonusType::NONE);
    currentBonusCount--; 
    processMatches();
}

void Game::applyRecolorBonus(int row, int col) {
    TileType targetColor = grid[row][col].getType();
    std::vector<std::pair<int, int>> candidates;

    for (int i = std::max(0, row - BONUS_RADIUS); i <= std::min(GRID_HEIGHT - 1, row + BONUS_RADIUS); ++i) {
        for (int j = std::max(0, col - BONUS_RADIUS); j <= std::min(GRID_WIDTH - 1, col + BONUS_RADIUS); ++j) {
            if (!(abs(i - row) <= 1 && abs(j - col) <= 1)) {
                candidates.emplace_back(i, j);
            }
        }
    }

    std::shuffle(candidates.begin(), candidates.end(), rng);

    int count = std::min(3, static_cast<int>(candidates.size()));
    for (int k = 0; k < count; ++k) {
        int i = candidates[k].first;
        int j = candidates[k].second;
        grid[i][j].setType(targetColor);
    }
}

void Game::applyBombBonus(int row, int col) {
    for (int i = std::max(0, row - 2); i <= std::min(GRID_HEIGHT - 1, row + 2); ++i) {
        for (int j = std::max(0, col - 2); j <= std::min(GRID_WIDTH - 1, col + 2); ++j) {
            grid[i][j].setMatched(true);
        }
    }
}

void Game::handleMouseClick(int mouseX, int mouseY) {
    int col = mouseX / TILE_SIZE;
    int row = mouseY / TILE_SIZE;

    if (col < 0 || col >= GRID_WIDTH || row < 0 || row >= GRID_HEIGHT) {
        isFirstTileSelected = false;
        return;
    }

    if (grid[row][col].getBonus() != BonusType::NONE) {
        if (!scoreActive) scoreActive = true;
        activateBonus(row, col);
        return;
    }

    if (!isFirstTileSelected) {
        selectedTile = sf::Vector2i(row, col);
        isFirstTileSelected = true;
        grid[row][col].setOutline(sf::Color::White, 3);
    }
    else {
        grid[selectedTile.x][selectedTile.y].setOutline(sf::Color::Black, 1);

        if ((abs(selectedTile.x - row) == 1 && selectedTile.y == col) ||
            (abs(selectedTile.y - col) == 1 && selectedTile.x == row)) {

            auto tempGrid = grid;
            std::swap(tempGrid[selectedTile.x][selectedTile.y], tempGrid[row][col]);

            bool hasMatches = checkPotentialMatches(tempGrid, selectedTile.x, selectedTile.y) ||
                checkPotentialMatches(tempGrid, row, col);

            if (hasMatches) {
                if (!scoreActive) scoreActive = true;
                swapTiles(selectedTile.x, selectedTile.y, row, col);
                isAnimating = true;
            }
            else {
                score -= 50;
                scoreText.setString("Score: " + std::to_string(score));
                selectedTile = sf::Vector2i(row, col);
                grid[row][col].setOutline(sf::Color::White, 3);
            }
            moves++;
            movesText.setString("Moves: " + std::to_string(moves));
            isFirstTileSelected = false;
            grid[selectedTile.x][selectedTile.y].setOutline(sf::Color::Black, 1);
        }
        else {
            selectedTile = sf::Vector2i(row, col);
            grid[row][col].setOutline(sf::Color::White, 3);
        }
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (gameOver || gameWon) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                resetGame();
            }
            continue;
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left &&
            !isAnimating) {
            handleMouseClick(event.mouseButton.x, event.mouseButton.y);
        }
    }
}

void Game::update() {
    bool anyFalling = false;
    bool anyAnimating = false;

    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            if (grid[i][j].isFalling()) {
                if (grid[i][j].getFallDistance() > 0) {
                    float fallStep = FALL_SPEED * TILE_SIZE;
                    grid[i][j].setFallDistance(std::max(0.0f, grid[i][j].getFallDistance() - fallStep));
                    grid[i][j].move(0, fallStep);
                    anyFalling = true;
                }
                else {
                    grid[i][j].setFalling(false);
                    grid[i][j].setPosition(j * TILE_SIZE + 2.0f, i * TILE_SIZE + 2.0f);
                }
            }

            if (grid[i][j].isBonusAnimating()) {
                grid[i][j].setBonusScale(std::min(1.0f, grid[i][j].getBonusScale() + BONUS_ANIMATION_SPEED));

                if (grid[i][j].getBonusScale() >= 1.0f) {
                    grid[i][j].setBonusAnimating(false);
                }
                anyAnimating = true;
            }
        }
    }

    if (isAnimating && !anyFalling && !anyAnimating) {
        isAnimating = false;
        if (checkMatches()) {
            processMatches();
        }
    }
}

void Game::render() {
    window.clear();

    window.draw(background);

    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            grid[i][j].draw(window);
        }
    }

    sf::RectangleShape panel(sf::Vector2f(WINDOW_WIDTH, 50));
    panel.setFillColor(sf::Color(30, 30, 50));
    panel.setPosition(0, WINDOW_HEIGHT - 50);
    window.draw(panel);

    window.draw(scoreText);
    window.draw(movesText);
    checkGameState();

    if (gameOver) {
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        window.draw(gameOverText);
    }
    else if (gameWon) {
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        window.draw(gameWinText);
    }

    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}