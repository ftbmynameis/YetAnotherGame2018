#pragma once

#include <SFML\System\Vector2.hpp>

class PlayerCharacterController
{
private:
	enum class RollAxisMovementState
	{
		FORWARD = 1,
		NO_MOVEMENT = 0,
		BACKWARD = -1
	};

	enum class PitchAxisMovementState
	{
		STRAFE_LEFT = 1,
		NO_MOVEMENT = 0,
		STRAFE_RIGHT = -1
	};
public:
	void key_forward(bool pressed)
	{
		if (pressed)
		{
			roll_state = RollAxisMovementState::FORWARD;
		}
		else if (roll_state == RollAxisMovementState::FORWARD)
		{
			roll_state = RollAxisMovementState::NO_MOVEMENT;
		}
	}

	void key_backward(bool pressed)
	{
		// Pressing forward has priority in case both keys are pressed simultanously
		if (roll_state != RollAxisMovementState::FORWARD)
		{
			if (pressed)
			{
				roll_state = RollAxisMovementState::BACKWARD;
			}
			else
			{
				roll_state = RollAxisMovementState::NO_MOVEMENT;
			}
		}
	}

	void key_strafe_left(bool pressed)
	{
		if (pressed)
		{
			pitch_state = PitchAxisMovementState::STRAFE_LEFT;
		}
		else if (pitch_state == PitchAxisMovementState::STRAFE_LEFT)
		{
			pitch_state = PitchAxisMovementState::NO_MOVEMENT;
		}
	}

	void key_strafe_right(bool pressed)
	{
		if (pitch_state != PitchAxisMovementState::STRAFE_LEFT)
		{
			if (pressed)
			{
				pitch_state = PitchAxisMovementState::STRAFE_RIGHT;
			}
			else
			{
				pitch_state = PitchAxisMovementState::NO_MOVEMENT;
			}
		}
	}

	void key_channeling(bool pressed)
	{
		is_channeling = pressed;
	}

	void mouse_moved(const sf::Vector2i& mouse_pos)
	{
		this->mouse_pos = mouse_pos;
	}

	void key_running(bool pressed)
	{
		running_state = pressed;
	}

	RollAxisMovementState get_roll_state() const
	{
		return roll_state;
	}

	PitchAxisMovementState get_pitch_state() const
	{
		return pitch_state;
	}

	bool get_running_state() const
	{
		return running_state;
	}

	sf::Vector2i get_mouse_pos() const
	{
		return mouse_pos;
	}

	bool get_channeling() const
	{
		return is_channeling;
	}

private:
	RollAxisMovementState roll_state;
	PitchAxisMovementState pitch_state;
	bool running_state;
	sf::Vector2i mouse_pos;
	bool is_channeling;
};
PlayerCharacterController the_controller;