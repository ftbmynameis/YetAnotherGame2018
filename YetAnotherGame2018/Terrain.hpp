#pragma once

class Terrain
{
public:
	enum class TerrainType
	{
		DEFAULT,
		SWAMP
	};	

	Terrain(TerrainType type_ = TerrainType::DEFAULT,
		bool allows_sprinting_ = true,
		float speed_modifier_ = 1.0f,
		bool disables_stamina_regen_ = false);

	TerrainType get_type() const;
	bool is_sprinting_allowed() const;
	float get_speed_modifier() const;
	bool is_stamina_regen_disabled() const;

	static const Terrain DEFAULT;
	static const Terrain SWAMP;
private:
	TerrainType type;
	bool allows_sprinting;
	float speed_modifier;
	bool disables_stamina_regen;
};