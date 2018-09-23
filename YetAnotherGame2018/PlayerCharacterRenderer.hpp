#pragma once

#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include "PlayerCharacterState.hpp"

#include "util.hpp"
#include "globals.hpp"

class PlayerCharacterRenderer
{
public:
	PlayerCharacterRenderer(const sf::Texture& texture, const sf::IntRect& rect)
		: the_sprite(texture, rect)
	{
		the_sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
	}

	void render(const PlayerCharacterState& state)
	{
		// Update sprite data
		the_sprite.setPosition(state.position);
		the_sprite.setRotation(rad2deg(state.yaw) + 90.0f);

		window_handle.draw(the_sprite);
	}
private:
	sf::Sprite the_sprite;
};