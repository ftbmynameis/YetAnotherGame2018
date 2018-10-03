#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "PlayerEntity.hpp"
#include "Terrain.hpp"

using namespace std::chrono_literals;

class Game
{
private:
	Game();
public:
	static Game* instance();
	void run();
	void update(float dt);
	void render(float alpha);
	void debug_number_in_title(float number);

private:
	static Game* _instance;
	sf::VideoMode window_resolution;
	sf::RenderWindow window_handle;
	sf::Vector2f window_center;
	float timestep_f;
	// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
	const std::chrono::nanoseconds timestep;
	bool should_quit_game;

	std::vector<std::unique_ptr<Entity> > entities;
	std::unique_ptr<PlayerEntity> player_entity;
	std::vector<std::pair<sf::FloatRect, Terrain> > terrain_areas;

	// TEMP DEBUG
	sf::Texture player_texture;

	std::vector<sf::Event> get_events_this_frame();
	void read_input();
	PlayerEntity::MOVE_STATE key_press_to_state(bool pressed, PlayerEntity::MOVE_STATE old_state, PlayerEntity::MOVE_STATE target_state);
	void handle_event_for_game(const sf::Event& event);
	void render_filled_bar(int margin_screen,
		int margin_bars,
		int border_width,
		const sf::Vector2f bar_size,
		int bar_idx,
		const sf::Color& background_color,
		const sf::Color& fill_color,
		float fill_value
	);
	void render_game_interface();

};