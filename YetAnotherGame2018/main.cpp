#include <chrono>
#include <cmath>

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "PlayerCharacterController.hpp"
#include "PlayerCharacterState.hpp"
#include "PlayerCharacterRenderer.hpp"
#include "PlayerCharacter.hpp"

#include "globals.hpp"

// original game loop implementation from: https://gist.github.com/mariobadr/673bbd5545242fcf9482

using namespace std::chrono_literals;

// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);
bool should_quit_game = false;

// class TerrainArea -> Class to represent terrain that modifies properties like movement speed or reduces life over time"

void handle_event_for_game(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
	{
		bool pressed = event.type == sf::Event::KeyPressed;
		if (event.key.code == sf::Keyboard::W)
		{
			the_controller.key_forward(pressed);
		}
		if (event.key.code == sf::Keyboard::S)
		{
			the_controller.key_backward(pressed);
		}
		if (event.key.code == sf::Keyboard::A)
		{
			the_controller.key_strafe_left(pressed);
		}
		if (event.key.code == sf::Keyboard::D)
		{
			the_controller.key_strafe_right(pressed);
		}
		if (event.key.code == sf::Keyboard::LShift)
		{
			the_controller.key_running(pressed);
		}
	}

	if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2i new_mouse_pos(event.mouseMove.x, event.mouseMove.y);
		the_controller.mouse_moved(new_mouse_pos);
	}
}

std::vector<sf::Event> get_events_this_frame() 
{
	sf::Event event;
	std::vector<sf::Event> events_this_frame;
	while (window_handle.pollEvent(event))
	{
		events_this_frame.push_back(event);
	}
	return events_this_frame;
}

void read_input()
{
	for (const auto& event : get_events_this_frame())
	{
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			should_quit_game = true;
		}

		handle_event_for_game(event);
	}
}

void render_filled_bar(int margin_screen,
	int margin_bars,
	int border_width,
	const sf::Vector2f bar_size,
	int bar_idx,
	const sf::Color& background_color,
	const sf::Color& fill_color,
	float fill_value
	)
{
	sf::Vector2f bar_size_fx(bar_size.x + 2 * border_width, bar_size.y + 2 * border_width);
	int bar_pos_y = window_resolution.height - margin_screen - (bar_idx + 1) * bar_size.y - bar_idx * margin_bars;
	sf::Vector2f pos(margin_screen, bar_pos_y);

	sf::RectangleShape shape(bar_size_fx);
	shape.setPosition(pos);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(-border_width);
	shape.setFillColor(background_color);

	auto border_width_vec = sf::Vector2f(border_width, border_width);
	bar_size_fx = bar_size_fx - border_width_vec * 2.0f;
	bar_size_fx.x *= fill_value;
	sf::RectangleShape shape_fill(bar_size_fx);
	shape_fill.setPosition(pos + border_width_vec);
	shape_fill.setOutlineColor(sf::Color::Transparent);
	shape_fill.setOutlineThickness(0);
	shape_fill.setFillColor(fill_color);

	window_handle.draw(shape);
	window_handle.draw(shape_fill);
}

void update_game(PlayerCharacter& player)
{
	player.update_state();
}

void render_game_interface(const PlayerCharacterState& player_state)
{
	// Render stamina bar of the player in bottom left corner, first stamina then life
	const int margin_screen = 25;
	const int margin_bars = 10;
	const int border_width = 1;
	const sf::Vector2f bar_size(window_resolution.width * 0.15f, window_resolution.height * 0.015f);

	sf::Color stamina_bg = sf::Color(0, 127, 0);
	render_filled_bar(margin_screen, margin_bars, border_width, bar_size, 0, stamina_bg, sf::Color::Green, player_state.stamina.get_fill());

	sf::Color life_bg = sf::Color::Red;
	life_bg.a = 0.95f;
	render_filled_bar(margin_screen, margin_bars, border_width, bar_size, 1, life_bg, sf::Color::Red, player_state.life.get_fill());
}

int main() {
	using clock = std::chrono::high_resolution_clock;

	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();
	bool quit_game = false;

	PlayerCharacterState current_state;
	current_state.position = sf::Vector2f(window_center_point.x, window_center_point.y);
	PlayerCharacter the_player(current_state);

	PlayerCharacterState previous_state;

	sf::Texture player_texture;
	if (!player_texture.loadFromFile("hero.png"))
	{
		return -1;
	}

	sf::IntRect player_on_tex_rect(0, 0, 31, 31);
	PlayerCharacterRenderer the_player_renderer(player_texture, player_on_tex_rect);

	while (!quit_game) {
		auto delta_time = clock::now() - time_start;
		time_start = clock::now();
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		// process input
		read_input();
		the_player.process_controller_update();

		// update game logic as lag permits
		while (lag >= timestep) {
			lag -= timestep;

			previous_state = current_state;
			//update(&current_state); // update at a fixed rate each time
			update_game(the_player);
			current_state = the_player.get_state();
		}

		// calculate how close or far we are from the next timestep
		auto alpha = (float)lag.count() / timestep.count();
		auto interpolated_state = PlayerCharacterState::interpolate(previous_state, current_state, alpha);
		auto extrapolated_state = current_state;
		extrapolated_state.update(timestep_f * alpha);

		//render(interpolated_state);
		window_handle.clear(sf::Color::Cyan);
		the_player_renderer.render(interpolated_state);
		//the_player_renderer.render(extrapolated_state);
		//the_player_renderer.render(current_state);

		render_game_interface(interpolated_state);

		window_handle.display();
	}
}