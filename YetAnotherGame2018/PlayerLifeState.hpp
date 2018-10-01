#pragma once

class PlayerLifeState
{
public:
	PlayerLifeState()
		: life_value(max_life)
	{
	}

	static PlayerLifeState interpolate(const PlayerLifeState& left, const PlayerLifeState& right, float alpha)
	{
		auto new_state = left;
		new_state.life_value = new_state.life_value * alpha + right.life_value * (1.0f - alpha);
		return new_state;
	}

	void set_channel_heal(float heal_per_second)
	{
		channel_heal_per_second = heal_per_second;
	}

	void set_instant_heal(float heal)
	{
		instant_heal = heal;
	}

	void set_instant_damage(float damage)
	{
		instant_damage = damage;
	}

	void update(float dt)
	{
		if (instant_heal > 0.0f)
		{
			life_value += instant_heal;
			instant_heal = 0.0f;
		}

		if (instant_damage > 0.0f)
		{
			life_value -= instant_damage;
			instant_damage = 0.0f;
		}

		if (life_value < 0.0f)
		{
			life_value = 0.0f;
			channel_heal_per_second = 0.0f;
		} else if (channel_heal_per_second > 0.0f)
		{
			life_value += channel_heal_per_second * dt;
			
			if (life_value > max_life)
			{
				life_value = max_life;
			}
		}
	}

	int get_health() const
	{
		return static_cast<int>(life_value);
	}

	float get_fill() const
	{
		return life_value / max_life;
	}

private:
	float max_life = 100.0f;
	float life_value;

	float channel_heal_per_second;
	float instant_heal;
	float instant_damage;
};
