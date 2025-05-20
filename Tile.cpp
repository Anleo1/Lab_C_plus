#include "Tile.h"

Tile::Tile() : type(TileType::RED), bonus(BonusType::NONE), matched(false),
falling(false), fallDistance(0), bonusAnimating(false), bonusScale(1.0f) {
    shape.setSize(sf::Vector2f(TILE_SIZE - 4.0f, TILE_SIZE - 4.0f));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);

    bonusShape.setRadius(TILE_SIZE / 4.0f - 2.0f);
    bonusShape.setOrigin(TILE_SIZE / 4.0f - 2.0f, TILE_SIZE / 4.0f - 2.0f);
    bonusShape.setOutlineThickness(0);
}

void Tile::initialize(TileType tileType, float x, float y) {
    type = tileType;
    updateColor();
    setPosition(x, y);
}

void Tile::updateColor() {
    switch (type) {
    case TileType::RED:    shape.setFillColor(sf::Color(255, 70, 70)); break;
    case TileType::GREEN:  shape.setFillColor(sf::Color(70, 255, 70)); break;
    case TileType::BLUE:   shape.setFillColor(sf::Color(70, 70, 255)); break;
    case TileType::YELLOW: shape.setFillColor(sf::Color(255, 255, 70)); break;
    case TileType::PURPLE: shape.setFillColor(sf::Color(220, 70, 255)); break;
    default: shape.setFillColor(sf::Color::White);
    }
}

void Tile::setupBonusAppearance() {
    if (bonus == BonusType::RECOLOR) {
        bonusShape.setFillColor(sf::Color::White);
        bonusShape.setOutlineColor(sf::Color(255, 255, 0));
        bonusShape.setPointCount(3);
    }
    else if (bonus == BonusType::BOMB) {
        bonusShape.setFillColor(sf::Color(255, 50, 50));
        bonusShape.setOutlineColor(sf::Color(255, 150, 150));
        bonusShape.setPointCount(32);
    }
}

void Tile::setPosition(float x, float y) {
    shape.setPosition(x, y);
    bonusShape.setPosition(x + TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f);
}

void Tile::move(float offsetX, float offsetY) {
    shape.move(offsetX, offsetY);
    bonusShape.move(offsetX, offsetY);
}

void Tile::draw(sf::RenderWindow& window) {
    if (type != static_cast<TileType>(-1)) {
        window.draw(shape);
        if (bonus != BonusType::NONE) {
            window.draw(bonusShape);
        }
    }
}

TileType Tile::getType() const { return type; }
void Tile::setType(TileType newType) { type = newType; updateColor(); }
BonusType Tile::getBonus() const { return bonus; }
void Tile::setBonus(BonusType newBonus) {
    bonus = newBonus;
    if (bonus != BonusType::NONE) {
        setupBonusAppearance();
        bonusAnimating = true;
        bonusScale = 0.1f;
    }
}

bool Tile::isMatched() const { 
    return matched; 
}

void Tile::setMatched(bool value) {
    matched = value; 
}

bool Tile::isFalling() const { 
    return falling; 

}
void Tile::setFalling(bool value) { 
    falling = value; 
}

float Tile::getFallDistance() const { 
    return fallDistance; 
}

void Tile::setFallDistance(float distance) { 
    fallDistance = distance; 
}

bool Tile::isBonusAnimating() const { 
    return bonusAnimating; 
}

void Tile::setBonusAnimating(bool value) { 
    bonusAnimating = value; 
}

float Tile::getBonusScale() const { 
    return bonusScale; 
}

void Tile::setBonusScale(float scale) {
    bonusScale = scale;
    bonusShape.setScale(scale, scale);
}

void Tile::setOutline(sf::Color color, float thickness) {
    shape.setOutlineColor(color);
    shape.setOutlineThickness(thickness);
}