#pragma once

#include <SFML\Graphics\RenderWindow.hpp>

class Entity
{
public:
	virtual void render(sf::RenderWindow& window, float alpha) = 0;
	virtual void update(float dt) = 0;
	virtual void swap_fields() = 0;

	unsigned int get_id() const
	{
		return id;
	}
protected:
	unsigned int id;
};
