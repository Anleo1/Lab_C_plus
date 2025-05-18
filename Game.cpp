#include "Game.h"
#include <algorithm>

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid"),
    ballSpeed(INITIAL_BALL_SPEED), 
    paddleWidth(INITIAL_PADDLE_WIDTH),
    score(0), lives(INITIAL_LIVES), 
    gameOver(false), 
    gameWon(false),
    stickyAvailable(false), 
    ballStuck(false),
    bottomShieldActive(false),
    rng(std::random_device{}()), dist(-0.3f, 0.3f) {

    window.setFramerateLimit(60);

    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    initGameObjects();
    initUI();
    createBlocks();
    resetBall();
}

void Game::setupText(sf::Text& text, const std::string& str, float x, float y, unsigned int size) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(TEXT_COLOR);
    text.setPosition(x, y);
}

void Game::initGameObjects() {
    paddle.setSize({paddleWidth, PADDLE_HEIGHT});
    paddle.setOrigin(paddleWidth / 2, PADDLE_HEIGHT / 2);
    paddle.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
    paddle.setFillColor(sf::Color::White);

    ball.setRadius(BALL_RADIUS);
    ball.setOrigin(BALL_RADIUS, BALL_RADIUS);
    ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    ball.setFillColor(sf::Color::White);
}

void Game::initUI() {
    setupText(scoreText, "Score: 0", 10, 10);
    setupText(livesText, "Lives: " + std::to_string(lives), WINDOW_WIDTH - 150, 10);
    setupText(gameOverText, "GAME OVER! Press Q to restart", WINDOW_WIDTH / 2 - 280, WINDOW_HEIGHT / 2 - 20, 40);
    setupText(winText, "YOU WIN! Press Q to restart", WINDOW_WIDTH / 2 - 280, WINDOW_HEIGHT / 2 - 20, 40);
    setupText(stickyText, "Press SPACE to launch ball", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 100, 20);

    bottomShieldIndicator.setSize({ 50, 5 });
    bottomShieldIndicator.setFillColor(sf::Color::Yellow);
    bottomShieldIndicator.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 5);
}

void Game::createBlocks() {
    blocks.clear();
    activeBonuses.clear();

    const int rows = 4;
    const int cols = 8;
    const float setX = 60.0;
    const float setY = 50.0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = setX + j * (BLOCK_WIDTH + 10);
            float y = setY + i * (BLOCK_HEIGHT + 10);

            BlockType type;
            int health = 1;
            bool hasBonus = false;
            BonusType bonusType = BonusType::BOTTOM_SHIELD;

            int randVal = std::uniform_int_distribution<>(0, 9)(rng);

            if (randVal == 0) { 
                type = BlockType::UNBREAKABLE; 
            }
            else if (randVal == 1) {
                type = BlockType::SPEED_CHANGE;
            }
            else if (randVal == 2) {
                type = BlockType::WITH_BONUS;
                hasBonus = true;
                bonusType = static_cast<BonusType>(std::uniform_int_distribution<>(0, 6)(rng));
            }
            else {
                type = BlockType::REGULAR;
                health = std::uniform_int_distribution<>(1, 3)(rng);
            }

            blocks.push_back(std::make_unique<Block>(x, y, type, health, hasBonus, bonusType));
        }
    }
}

void Game::resetBall() {
    ball.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddle.getSize().y / 2 - BALL_RADIUS);
    ballStuck = true;
    ballVelocity = sf::Vector2f(0, 0);
}

bool Game::checkWinCondition() {
    return std::all_of(blocks.begin(), blocks.end(), [](const auto& block) {
        return block->isDestroyed() || block->getType() == BlockType::UNBREAKABLE;
        });
}

void Game::updateBall(float deltaTime) {
    if (!ballStuck) {
        ball.move(ballVelocity * deltaTime);

        if (ball.getPosition().x - BALL_RADIUS < 0) {
            ball.setPosition(BALL_RADIUS, ball.getPosition().y);
            ballVelocity.x = -ballVelocity.x;
        }

        if (ball.getPosition().x + BALL_RADIUS > WINDOW_WIDTH) {
            ball.setPosition(WINDOW_WIDTH - BALL_RADIUS, ball.getPosition().y);
            ballVelocity.x = -ballVelocity.x;
        }

        if (ball.getPosition().y - BALL_RADIUS < 0) {
            ball.setPosition(ball.getPosition().x, BALL_RADIUS);
            ballVelocity.y = -ballVelocity.y;
        }

        if (ball.getPosition().y + BALL_RADIUS > WINDOW_HEIGHT) {
            handleBallLoss();
        }
    }
}

void Game::handleBallLoss() {
    if (bottomShieldActive) {
        bottomShieldActive = false;
        ball.setPosition(ball.getPosition().x, WINDOW_HEIGHT - BALL_RADIUS);
        ballVelocity.y = -std::abs(ballVelocity.y);
    }
    else {
        lives--;
        score -= LOSE_PENALTY;

        if (lives <= 0) {
            gameOver = true;
        }
        else {
            resetBall();
        }
    }
}

void Game::checkCollisions() {
    if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
        handlePaddleCollision();
    }

    for (auto& block : blocks) {
        if (!block->isDestroyed() && ball.getGlobalBounds().intersects(block->getBounds())) {
            handleBlockCollision(*block);
            break;
        }
    }
}

void Game::handlePaddleCollision() {
    float hitPos = (ball.getPosition().x - paddle.getPosition().x) / (paddle.getSize().x / 2);
    float angle = hitPos * (M_PI / 4);

    float speed = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
    ballVelocity.x = std::sin(angle) * speed;
    ballVelocity.y = -std::cos(angle) * speed;

    if (stickyAvailable) {
        ballStuck = true;
        stickyAvailable = false;
    }
}

void Game::handleBlockCollision(Block& block) {
    sf::FloatRect ballRect = ball.getGlobalBounds();
    sf::FloatRect blockRect = block.getBounds();

    float overlapLeft = ballRect.left + ballRect.width - blockRect.left;
    float overlapRight = blockRect.left + blockRect.width - ballRect.left;
    float overlapTop = ballRect.top + ballRect.height - blockRect.top;
    float overlapBottom = blockRect.top + blockRect.height - ballRect.top;

    bool fromLeft = std::abs(overlapLeft) < std::abs(overlapRight);
    bool fromTop = std::abs(overlapTop) < std::abs(overlapBottom);

    float minOverlapX = fromLeft ? overlapLeft : overlapRight;
    float minOverlapY = fromTop ? overlapTop : overlapBottom;

    if (std::abs(minOverlapX) < std::abs(minOverlapY)) {
        if (fromLeft) {
            ballVelocity.x = -std::abs(ballVelocity.x);
        }
        else {
            ballVelocity.x = std::abs(ballVelocity.x);
        }
    }
    else {
        if (fromTop) {
            ballVelocity.y = -std::abs(ballVelocity.y);
        }
        else {
            ballVelocity.y = std::abs(ballVelocity.y);
        }
    }

    block.hit();
    if (block.getType() != BlockType::UNBREAKABLE) {
        if (block.hasBonusInside() && block.isDestroyed()) {
            sf::Vector2f pos = block.getPosition();
            activeBonuses.push_back(std::make_unique<Bonus>(pos.x, pos.y, block.getBonusType()));
        }

        if (block.getType() == BlockType::SPEED_CHANGE) {
            score += 4;
            ballSpeed *= 1.2f;
            ballVelocity = ballVelocity / std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y) * ballSpeed;
        }
        
        score += 1;
        
    }
}

void Game::updateBonuses(float deltaTime) {
    for (auto bonusIter = activeBonuses.begin(); bonusIter != activeBonuses.end();) {
        auto& currentBonus = *bonusIter;
        currentBonus->update(deltaTime);

        bool collectedByPaddle = currentBonus->getBounds().intersects(paddle.getGlobalBounds());
        bool fellOffScreen = currentBonus->getPosition().y > WINDOW_HEIGHT;

        if (collectedByPaddle) {
            applyBonus(currentBonus->getType());
            bonusIter = activeBonuses.erase(bonusIter);
        }
        else if (fellOffScreen) {
            bonusIter = activeBonuses.erase(bonusIter);
        }
        else {
            ++bonusIter;
        }
    }
}

void Game::applyBonus(BonusType type) {
    switch (type) {
        score += 4;
    case BonusType::PADDLE_SIZE_INCREASE:
        paddleWidth = std::min(paddleWidth * 1.5f, INITIAL_PADDLE_WIDTH * 2.0f);
        paddle.setSize({ paddleWidth, PADDLE_HEIGHT });
        paddle.setOrigin(paddleWidth / 2, PADDLE_HEIGHT / 2);
        break;

    case BonusType::PADDLE_SIZE_DECREASE:
        paddleWidth = std::max(paddleWidth * 0.75f, INITIAL_PADDLE_WIDTH * 0.5f);
        paddle.setSize({ paddleWidth, PADDLE_HEIGHT });
        paddle.setOrigin(paddleWidth / 2, PADDLE_HEIGHT / 2);
        break;

    case BonusType::BALL_SPEED_INCREASE:
        ballSpeed *= 1.2f;
        ballVelocity = ballVelocity / std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y) * ballSpeed;
        break;

    case BonusType::BALL_SPEED_DECREASE:
        ballSpeed *= 0.8f;
        ballVelocity = ballVelocity / std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y) * ballSpeed;
        break;

    case BonusType::STICKY_PADDLE:
        stickyAvailable = true;
        break;

    case BonusType::BOTTOM_SHIELD:
        bottomShieldActive = true;
        break;

    case BonusType::RANDOM_TRAJECTORY_CHANGE:
        float randomAngle = std::uniform_real_distribution<float>(-0.785f, 0.785f)(rng);
        ballVelocity = sf::Vector2f(std::sin(randomAngle) * ballSpeed, -std::cos(randomAngle) * ballSpeed);
        break;
    }
}


void Game::updateUI() {
    scoreText.setString("Score: " + std::to_string(score));
    livesText.setString("Lives: " + std::to_string(lives));
}

void Game::render() {
    window.clear();

    window.draw(paddle);
    window.draw(ball);

    for (const auto& block : blocks) {
        block->draw(window);
    }

    for (const auto& bonus : activeBonuses) {
        bonus->draw(window);
    }

    window.draw(scoreText);
    window.draw(livesText);

    if (bottomShieldActive) {
        window.draw(bottomShieldIndicator);
    }

    if (ballStuck) {
        window.draw(stickyText);
    }

    if (gameOver) {
        window.draw(gameOverText);
    }
    else if (gameWon) {
        window.draw(winText);
    }

    window.display();
}

void Game::initGame() {
    ballSpeed = INITIAL_BALL_SPEED;
    paddleWidth = INITIAL_PADDLE_WIDTH;
    stickyAvailable = false;
    ballStuck = false;
    bottomShieldActive = false;
    score = 0;
    lives = INITIAL_LIVES;
    gameOver = false;
    gameWon = false;

    initGameObjects();
    createBlocks();
    resetBall();
    updateUI();
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();

        if (!gameOver && !gameWon) {
            update(deltaTime.asSeconds());

            if (checkWinCondition() || score >= WIN_SCORE) {
                gameWon = true;
            }
        }

        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space && ballStuck) {
                ballStuck = false;
                float angle = dist(rng);
                ballVelocity.x = std::sin(angle) * ballSpeed;
                ballVelocity.y = -std::abs(std::cos(angle)) * ballSpeed;
            }
            else if (event.key.code == sf::Keyboard::Q && (gameOver || gameWon)) {
                initGame();
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
        paddle.getPosition().x - paddle.getSize().x / 2 > 0) {
        paddle.move(-PADDLE_SPEED * deltaTime, 0);
        if (ballStuck) {
            ball.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddle.getSize().y / 2 - BALL_RADIUS);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        paddle.getPosition().x + paddle.getSize().x / 2 < WINDOW_WIDTH) {
        paddle.move(PADDLE_SPEED * deltaTime, 0);
        if (ballStuck) {
            ball.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddle.getSize().y / 2 - BALL_RADIUS);
        }
    }

    if (!ballStuck) {
        updateBall(deltaTime);
        checkCollisions();
    }

    updateBonuses(deltaTime);
    updateUI();
}