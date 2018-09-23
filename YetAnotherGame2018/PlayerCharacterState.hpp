#pragma once

#include <SFML\System\Vector2.hpp>
#include "RechargeableAttributeState.hpp"

struct PlayerCharacterState
{
	PlayerCharacterState()
		: yaw(0.0f), stamina(100, 15, 7)
	{
	}

	PlayerCharacterState& operator=(const PlayerCharacterState& rhs)
	{
		this->movement_vec = rhs.movement_vec;
		this->position = rhs.position;
		this->yaw = rhs.yaw;
		this->stamina = rhs.stamina;
		return *this;
	}


	void extrapolate(float time_delta)
	{
		position += movement_vec * time_delta;
		stamina.update(time_delta);
	}

	PlayerCharacterState interpolate(const PlayerCharacterState& state, float alpha)
	{
		// TODO: MAYBE NOT A GOOD DESIGN AS A MEMBER FUNCTION (FREE FUNCTION?)
		PlayerCharacterState new_state = *this;
		new_state = new_state * (1.0f - alpha) + state * alpha;
		return new_state;
	}

	friend PlayerCharacterState operator*(const PlayerCharacterState& state, float alpha)
	{
		PlayerCharacterState new_state = state;
		new_state.position *= alpha;
		new_state.yaw *= alpha;
		new_state.stamina *= alpha;
		return new_state;
	}

	PlayerCharacterState operator+(const PlayerCharacterState& rhs)
	{
		PlayerCharacterState new_state = *this;
		new_state.position += rhs.position;
		new_state.yaw += rhs.yaw;
		new_state.stamina += rhs.stamina;
		return new_state;
	}

	sf::Vector2f position;
	sf::Vector2f movement_vec;
	float yaw;
	RechargeableAttributeState stamina;
};