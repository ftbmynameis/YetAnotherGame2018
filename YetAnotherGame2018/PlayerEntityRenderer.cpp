#include "PlayerEntityRenderer.hpp"
#include "PlayerEntity.hpp"
#include "util.hpp"

PlayerEntityRenderer::PlayerEntityRenderer(const sf::Texture& texture, const sf::IntRect& rect)
	: the_sprite(texture, rect)
{
	the_sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
}

void PlayerEntityRenderer::render(sf::RenderWindow& window, const PlayerEntity* entity, float alpha)
{
	// Update sprite data
	the_sprite.setPosition(entity->position_old + lerp<sf::Vector2f>(entity->position_old, entity->position_new, alpha));
	the_sprite.setRotation(rad2deg(entity->yaw_old + lerp(entity->yaw_old, entity->yaw_new, alpha)) + 90.0f);

	window.draw(the_sprite);
}