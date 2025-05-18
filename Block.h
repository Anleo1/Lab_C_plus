#pragma once
#include "Types.h"
#include "Constants.h"

class Block {
public:
    Block(float x, float y, BlockType t, int h = 1, bool hb = false, BonusType bt = BonusType::BOTTOM_SHIELD);

    void updateColor();
    void hit();

    bool isDestroyed() const;
    sf::FloatRect getBounds() const;
    BlockType getType() const;
    bool hasBonusInside() const;
    BonusType getBonusType() const;
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window) const;

private:
    sf::RectangleShape shape;
    BlockType type;
    int health;
    int maxHealth;
    bool hasBonus;
    BonusType bonusType;
    bool destroyed;
};