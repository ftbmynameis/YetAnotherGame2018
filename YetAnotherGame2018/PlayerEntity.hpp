#pragma once

#include <memory>
#include <SFML\System\Vector2.hpp>
#include "PlayerEntityRenderer.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"

class PlayerEntity : public Entity
{
public:
	friend class PlayerEntityRenderer;

	enum class MOVE_STATE
	{
		FORWARD = 1, // Left in case of side movement (pitch)
		NOTHING = 0,
		BACKWARD = -1 // Right in case of side movement (pitch)
	};

	PlayerEntity(PlayerEntityRenderer* renderer_);
	void render(sf::RenderWindow& window, float alpha);
	void update(float dt);
	void swap_fields();
	void set_ground_terrain(Terrain ground);
	const Terrain& get_ground_terrain() const;
	sf::Vector2f get_position() const;
	void set_yaw(float yaw_);
	float get_yaw() const;
	void set_wants_to_sprint(bool wants_to_sprint_);
	bool is_sprinting() const;
	float get_stamina_fill_percentage() const;
	void set_roll_state(MOVE_STATE roll_state_);
	MOVE_STATE get_roll_state() const;
	void set_pitch_state(MOVE_STATE pitch_state_);
	MOVE_STATE get_pitch_state() const;

private:
	std::unique_ptr<PlayerEntityRenderer> renderer;
	Terrain ground_terrain;
	sf::Vector2f position_old, position_new;
	float yaw_old, yaw_new;

	bool wants_to_sprint;
	float sprint_speed_modifier;
	float stamina_old, stamina_new;
	float stamina_regen_per_sec;
	float sprint_stamina_drain;
	float stamina_recovering_bounds;
	bool recovering;
	bool sprinting;
	float walking_speed;
	float strafe_speed;
	MOVE_STATE roll_state;
	MOVE_STATE pitch_state;
	
};
