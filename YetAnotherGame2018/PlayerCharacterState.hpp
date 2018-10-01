#pragma once

#include <SFML\System\Vector2.hpp>
#include "RechargeableAttributeState.hpp"
#include "PlayerLifeState.hpp"

struct PlayerCharacterState
{
	PlayerCharacterState()
		: yaw(0.0f), stamina(100, 15, 7)
	{
	}

	static PlayerCharacterState interpolate(const PlayerCharacterState& left, const PlayerCharacterState& right, float alpha)
	{
		auto new_state = left;
		new_state.position = new_state.position * alpha + right.position * (1.0f - alpha);
		new_state.yaw = new_state.yaw * alpha + right.yaw * (1.0f - alpha);
		new_state.stamina = RechargeableAttributeState::interpolate(left.stamina, right.stamina, alpha);
		new_state.life = PlayerLifeState::interpolate(left.life, right.life, alpha);
		return new_state;
	}


	void update(float time_delta)
	{
		position += movement_vec * time_delta;
		stamina.update(time_delta);
		life.update(time_delta);
	}

	sf::Vector2f position;
	sf::Vector2f movement_vec;
	float yaw;
	RechargeableAttributeState stamina;
	PlayerLifeState life;
};