#pragma once
#include <SFML/Graphics.hpp>

const int TILE_SIZE = 64;

enum class TileType { 
    RED, GREEN, BLUE, YELLOW, PURPLE, COUNT 
};

enum class BonusType { 
    NONE, RECOLOR, BOMB, COUNT 
};

class Tile {
private:
    TileType type;
    BonusType bonus;
    sf::RectangleShape shape;
    sf::CircleShape bonusShape;
    bool matched;
    bool falling;
    float fallDistance;
    bool bonusAnimating;
    float bonusScale;

public:
    Tile();
    void initialize(TileType tileType, float x, float y);
    void updateColor();
    void setupBonusAppearance();
    void setPosition(float x, float y);
    void move(float offsetX, float offsetY);
    void draw(sf::RenderWindow& window);

    TileType getType() const;
    void setType(TileType newType);
    BonusType getBonus() const;
    void setBonus(BonusType newBonus);
    bool isMatched() const;
    void setMatched(bool value);
    bool isFalling() const;
    void setFalling(bool value);
    float getFallDistance() const;
    void setFallDistance(float distance);
    bool isBonusAnimating() const;
    void setBonusAnimating(bool value);
    float getBonusScale() const;
    void setBonusScale(float scale);
    void setOutline(sf::Color color, float thickness);
};