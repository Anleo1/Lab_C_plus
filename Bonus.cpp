#include "Bonus.h"

Bonus::Bonus(float x, float y, BonusType t)
    : type(t), active(true), used(false) {
    shape.setPosition(x, y);
    shape.setSize({ BONUS_WIDTH, BONUS_HEIGHT });
    shape.setOrigin(BONUS_WIDTH / 2, BONUS_HEIGHT / 2);

    switch (type) {
    case BonusType::PADDLE_SIZE_INCREASE:
        shape.setFillColor(sf::Color(0, 255, 0));
        break;
    case BonusType::PADDLE_SIZE_DECREASE:
        shape.setFillColor(sf::Color(255, 0, 0));
        break;
    case BonusType::BALL_SPEED_INCREASE:
        shape.setFillColor(sf::Color(255, 165, 0));
        break;
    case BonusType::BALL_SPEED_DECREASE:
        shape.setFillColor(sf::Color(0, 255, 255));
        break;
    case BonusType::STICKY_PADDLE:
        shape.setFillColor(sf::Color(255, 192, 203));
        break;
    case BonusType::BOTTOM_SHIELD:
        shape.setFillColor(sf::Color(255, 255, 255));
        break;
    }
}

void Bonus::update(float deltaTime) {
    if (isActive()) {
        shape.move(0, BONUS_SPEED * deltaTime);
    }
}

void Bonus::draw(sf::RenderWindow& window) const {
    if (isActive()) {
        window.draw(shape);
    }
}

void Bonus::activate() { active = true; }
void Bonus::deactivate() { active = false; }
void Bonus::use() { used = true; }
bool Bonus::isActive() const { return active && !used; }
sf::FloatRect Bonus::getBounds() const { return shape.getGlobalBounds(); }
BonusType Bonus::getType() const { return type; }
sf::Vector2f Bonus::getPosition() const { return shape.getPosition(); }