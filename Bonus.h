#pragma once
#include "Types.h"
#include "Constants.h"

class Bonus {
public:
    Bonus(float x, float y, BonusType t);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;

    void activate();
    void deactivate();
    void use();

    bool isActive() const;
    sf::FloatRect getBounds() const;
    BonusType getType() const;
    sf::Vector2f getPosition() const;

private:
    sf::RectangleShape shape;
    BonusType type;
    bool active;
    bool used;
};