#pragma once

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>

class PlayerEntity;

class PlayerEntityRenderer
{
public:
	PlayerEntityRenderer(const sf::Texture& texture, const sf::IntRect& rect);
	void render(sf::RenderWindow& window, const PlayerEntity* entity, float alpha);
private:
	sf::Sprite the_sprite;
};