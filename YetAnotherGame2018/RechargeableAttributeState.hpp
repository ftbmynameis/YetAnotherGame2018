#pragma once



struct RechargeableAttributeState
{
	int max_value;
	int loss_per_second;
	int recharge_per_second;
	bool draining;
	bool recovering;
	int recovering_bounds;

	float value;

	RechargeableAttributeState(int max_value_, int loss_per_second_, int recharge_per_second_)
		: max_value(max_value_),
		loss_per_second(loss_per_second_),
		recharge_per_second(recharge_per_second_),
		value(max_value),
		draining(false),
		recovering(false),
		recovering_bounds(50)
	{
	}

	static RechargeableAttributeState interpolate(const RechargeableAttributeState& left, const RechargeableAttributeState& right, float alpha)
	{
		auto new_state = left;
		new_state.value = new_state.value * alpha + right.value * (1.0f - alpha);
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

	bool get_recovering() const
	{
		return recovering;
	}

	float get_fill() const
	{
		return static_cast<float>(value) / max_value;
	}

	void update(float time_delta)
	{
		if (draining && !recovering)
		{
			float lost_val = loss_per_second * time_delta;
			if (value < lost_val)
			{
				value = 0.0f;
				recovering = true;
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

			if (recovering && value >= recovering_bounds)
				recovering = false;
		}
	}
};