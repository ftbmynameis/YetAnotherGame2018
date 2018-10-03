#include "PlayerEntity.hpp"

#include "util.hpp"
#include "Game.hpp"

PlayerEntity::PlayerEntity(PlayerEntityRenderer* renderer_)
	: renderer(std::move(renderer_)),
	ground_terrain(Terrain::DEFAULT),
	roll_state(MOVE_STATE::NOTHING),
	pitch_state(MOVE_STATE::NOTHING)
{
	position_new = sf::Vector2f(400, 300);
	yaw_new = 0.0f;

	wants_to_sprint = false;
	sprint_speed_modifier = 1.5f;
	stamina_new = 100.0f;
	stamina_regen_per_sec = 5.0f;
	sprinting = false;
	walking_speed = 50.0f;
	strafe_speed = 15.0f;
	sprint_stamina_drain = 12.5f;
	stamina_recovering_bounds = 15.0f;
	recovering = false;
}

void PlayerEntity::render(sf::RenderWindow& window, float alpha)
{
	if (renderer)
	{
		renderer->render(window, this, alpha);
	}
}

void PlayerEntity::update(float dt)
{
	swap_fields();

	float speed_modifier = 1.0f;
	// process sprinting
	if (wants_to_sprint && (stamina_new > 0.0f) && ground_terrain.is_sprinting_allowed() && !recovering)
	{
		sprinting = true;
		speed_modifier *= sprint_speed_modifier;
		// subtract some stamina
		stamina_new -= sprint_stamina_drain * dt;

		if (stamina_new < 0.0f)
		{
			recovering = true;
		}
	}
	else
		sprinting = false;
	speed_modifier *= ground_terrain.get_speed_modifier();

	// regenerate stamina
	if (!is_sprinting() && !ground_terrain.is_stamina_regen_disabled())
		stamina_new += stamina_regen_per_sec * dt;
	stamina_new = clamp(stamina_new, 0.0f, 100.0f);

	if (stamina_new > stamina_recovering_bounds)
		recovering = false;

	// update position
	sf::Vector2f character_direction(std::cosf(yaw_new), std::sinf(yaw_new));
	sf::Vector2f strafe_direction(character_direction.y, -character_direction.x);
	position_new = position_old + dt * speed_modifier * (to_underlying(roll_state) * walking_speed * character_direction + to_underlying(pitch_state) * strafe_speed * strafe_direction);

	// DEBUG STAMINA
	Game::instance()->debug_number_in_title(speed_modifier);
}

void PlayerEntity::swap_fields()
{
	position_old = position_new;
	yaw_old = yaw_new;
	stamina_old = stamina_new;
}

void PlayerEntity::set_ground_terrain(Terrain ground)
{
	ground_terrain = ground;
}

const Terrain& PlayerEntity::get_ground_terrain() const
{
	return ground_terrain;
}

sf::Vector2f PlayerEntity::get_position() const
{
	return position_new;
}

void PlayerEntity::set_yaw(float yaw_)
{
	yaw_new = yaw_;
}

float PlayerEntity::get_yaw() const
{
	return yaw_new;
}

void PlayerEntity::set_wants_to_sprint(bool wants_to_sprint_)
{
	wants_to_sprint = wants_to_sprint_;
}

bool PlayerEntity::is_sprinting() const
{
	return sprinting;
}

float PlayerEntity::get_stamina_fill_percentage() const
{
	return stamina_new / 100.0f;
}

void PlayerEntity::set_roll_state(MOVE_STATE roll_state_)
{
	roll_state = roll_state_;
}

PlayerEntity::MOVE_STATE PlayerEntity::get_roll_state() const
{
	return roll_state;
}

void PlayerEntity::set_pitch_state(MOVE_STATE pitch_state_)
{
	pitch_state = pitch_state_;
}

PlayerEntity::MOVE_STATE PlayerEntity::get_pitch_state() const
{
	return pitch_state;
}