#include "Terrain.hpp"

Terrain::Terrain(TerrainType type_,
	bool allows_sprinting_,
	float speed_modifier_,
	bool disables_stamina_regen_)
	: type(type_),
	allows_sprinting(allows_sprinting_),
	speed_modifier(speed_modifier_),
	disables_stamina_regen(disables_stamina_regen_)
{
}

Terrain::TerrainType Terrain::get_type() const
{
	return type;
}

bool Terrain::is_sprinting_allowed() const
{
	return allows_sprinting;
}

float Terrain::get_speed_modifier() const
{
	return speed_modifier;
}

bool Terrain::is_stamina_regen_disabled() const
{
	return disables_stamina_regen;
}

const Terrain Terrain::DEFAULT = Terrain();
const Terrain Terrain::SWAMP = Terrain(TerrainType::SWAMP, false, 0.5f, true);