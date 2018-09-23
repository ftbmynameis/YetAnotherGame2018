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

void render_game_interface(const PlayerCharacterState& player_state)
{
	// Render stamina bar of the player in bottom left corner
	sf::Vector2f bar_size(window_resolution.width * 0.1, window_resolution.height * 0.05);
	const std::size_t margin = 40; // pixel
	const std::size_t border_width = 3; // pixel

	sf::Vector2f pos(margin, window_resolution.height - margin - bar_size.y);
	//sf::Vector2f pos_fill(pos.x + border_width, pos.y + border_width);

	sf::RectangleShape shape(bar_size);
	shape.setPosition(pos);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(border_width);
	sf::Color background_fill = sf::Color::Green;
	background_fill.a = 255 * 0.3f;
	shape.setFillColor(background_fill);

	bar_size.x *= player_state.stamina.get_fill();
	sf::RectangleShape shape_fill(bar_size);
	shape_fill.setPosition(pos);
	shape_fill.setOutlineColor(sf::Color::Transparent);
	shape_fill.setOutlineThickness(0);
	shape_fill.setFillColor(sf::Color::Green);

	window_handle.draw(shape);
	window_handle.draw(shape_fill);
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
			the_player.update_state();
			current_state = the_player.get_state();
		}

		// calculate how close or far we are from the next timestep
		auto alpha = (float)lag.count() / timestep.count();
		auto interpolated_state = previous_state.interpolate(current_state, alpha);
		auto extrapolated_state = current_state;
		extrapolated_state.extrapolate(timestep_f * alpha);

		//render(interpolated_state);
		window_handle.clear(sf::Color::Cyan);
		the_player_renderer.render(interpolated_state);
		//the_player_renderer.render(extrapolated_state);
		//the_player_renderer.render(current_state);

		render_game_interface(interpolated_state);

		window_handle.display();
	}
}