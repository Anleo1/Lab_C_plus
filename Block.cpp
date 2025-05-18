#include "Block.h"

Block::Block(float x, float y, BlockType t, int h, bool hb, BonusType bt): 
    type(t), health(h), maxHealth(h), hasBonus(hb), bonusType(bt), destroyed(false) 
{
    shape.setPosition(x, y);
    shape.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
    shape.setOrigin(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2);
    updateColor();
}

void Block::updateColor() {
    switch (type) {
    case BlockType::UNBREAKABLE:
        shape.setFillColor(sf::Color(150, 150, 150));
        break;
    case BlockType::REGULAR: {
        float ratio = static_cast<float>(health) / maxHealth;
        sf::Uint8 green = static_cast<sf::Uint8>(255 * ratio);
        shape.setFillColor(sf::Color(0, green, 0));
        break;
    }
    case BlockType::SPEED_CHANGE:
        shape.setFillColor(sf::Color(255, 255, 0));
        break;
    case BlockType::WITH_BONUS:
        shape.setFillColor(sf::Color(255, 0, 255));
        break;
    }
}

void Block::hit() {
    if (type != BlockType::UNBREAKABLE) {
        health--;
        updateColor();
        if (health <= 0) {
            destroyed = true;
        }
    }
}

bool Block::isDestroyed() const { 
    return destroyed; 
}
sf::FloatRect Block::getBounds() const { 
    return shape.getGlobalBounds(); 
}
BlockType Block::getType() const { 
    return type; 
}
bool Block::hasBonusInside() const { 
    return hasBonus; 
}
BonusType Block::getBonusType() const { 
    return bonusType; 
}
sf::Vector2f Block::getPosition() const { 
    return shape.getPosition(); 
}

void Block::draw(sf::RenderWindow& window) const {
    if (!destroyed) {
        window.draw(shape);
    }
}