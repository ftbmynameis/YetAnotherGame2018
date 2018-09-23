#pragma once



struct RechargeableAttributeState
{
	const int max_value;
	const int loss_per_second;
	const int recharge_per_second;
	float value;
	bool draining;

	RechargeableAttributeState(int max_value_, int loss_per_second_, int recharge_per_second_)
		: max_value(max_value_),
		loss_per_second(loss_per_second_),
		recharge_per_second(recharge_per_second_),
		value(max_value),
		draining(false)
	{
	}

	RechargeableAttributeState& operator=(const RechargeableAttributeState& rhs)
	{
		this->value = rhs.value;
		this->draining = rhs.draining;
		return *this;
	}

	friend RechargeableAttributeState operator*=(const RechargeableAttributeState& state, float alpha)
	{
		RechargeableAttributeState new_state = state;
		new_state.value *= alpha;
		return new_state;
	}

	RechargeableAttributeState operator+=(const RechargeableAttributeState& rhs)
	{
		RechargeableAttributeState new_state = *this;
		new_state.value += rhs.value;
		return new_state;
	}

	void set_draining(bool drain)
	{
		draining = drain;
	}

	int get_value() const
	{
		return value;
	}

	float get_fill() const
	{
		return static_cast<float>(value) / max_value;
	}

	void update(float time_delta)
	{
		if (draining)
		{
			float lost_val = loss_per_second * time_delta;
			if (value < lost_val)
			{
				value = 0.0f;
			}
			else
			{
				value -= lost_val;
			}
		}
		else
		{
			value += recharge_per_second * time_delta;
			if (value > max_value)
			{
				value = max_value;
			}
		}
	}
};