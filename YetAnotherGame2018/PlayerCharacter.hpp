#pragma once

#include <math.h>
#include "PlayerCharacterState.hpp"
#include "util.hpp"
#include "globals.hpp"

class PlayerCharacter
{
public:
	static const int walking_speed = 50;
	static const float running_multiplier;
	static const int strafe_speed = 15;

	PlayerCharacter(const PlayerCharacterState& initial_state)
		: state(initial_state)
	{
	}

	// Get all new input updates from the controller
	void process_controller_update()
	{
		sf::Vector2i mouse_pos = the_controller.get_mouse_pos();
		float yaw = atan2f(mouse_pos.y - state.position.y, mouse_pos.x - state.position.x);
		sf::Vector2f character_direction(std::cosf(yaw), std::sinf(yaw));
		sf::Vector2f strafe_direction(character_direction.y, -character_direction.x);

		float roll_axis_speed = walking_speed;
		if (the_controller.get_running_state() && state.stamina.get_value())
		{
			roll_axis_speed *= running_multiplier;
		}
		state.stamina.set_draining(the_controller.get_running_state());

		state.movement_vec = to_underlying(the_controller.get_roll_state()) * roll_axis_speed * character_direction +
			to_underlying(the_controller.get_pitch_state()) * static_cast<float>(strafe_speed) * strafe_direction;
		state.yaw = yaw;
	}

	void update_state()
	{
		state.extrapolate(timestep_f);
	}

	PlayerCharacterState get_state() const
	{
		return state;
	}

private:
	PlayerCharacterState state;
};
const float PlayerCharacter::running_multiplier = 1.75f; // Because no reason.. (c++)
