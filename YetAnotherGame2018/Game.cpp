#include "Game.hpp"

using namespace std::chrono_literals;

Game* Game::_instance = nullptr;

Game::Game()
	: window_resolution(800, 600),
	window_center(window_resolution.width / 2.0f, window_resolution.height / 2.0f),
	window_handle(window_resolution, "YANG XX 18"),
	timestep_f(1.0f / 60.0f),
	should_quit_game(false),
	timestep(16ms)
{
	if (!player_texture.loadFromFile("hero.png"))
	{
		int foo = 0;
	}

	sf::IntRect player_on_tex_rect(0, 0, 31, 31);

	player_entity.reset(new PlayerEntity(new PlayerEntityRenderer(player_texture, player_on_tex_rect)));
}

Game* Game::instance()
{
	if (_instance == nullptr)
		_instance = new Game;
	return _instance;
}

void Game::run()
{
	using clock = std::chrono::high_resolution_clock;

	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();

	while (!should_quit_game) {
		auto delta_time = clock::now() - time_start;
		time_start = clock::now();
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		// process input
		read_input();

		// update game logic as lag permits
		while (lag >= timestep) {
			lag -= timestep;

			update(timestep_f);
		}

		// calculate how close or far we are from the next timestep
		auto alpha = (float)lag.count() / timestep.count();

		window_handle.clear(sf::Color::Cyan);
		render(alpha);
		window_handle.display();
	}
}

void Game::update(float dt)
{
	// Tell player what terrain he's on
	for (const auto& terrain_area : terrain_areas)
	{
		if (terrain_area.first.contains(player_entity->get_position()))
		{
			player_entity->set_ground_terrain(terrain_area.second);
		}
	}

	// At the end update the entities
	player_entity->update(dt);
}

void Game::render(float alpha)
{
	player_entity->render(window_handle, alpha);

	render_game_interface();
}

void Game::debug_number_in_title(float number)
{
	window_handle.setTitle(std::to_string(number));
}

std::vector<sf::Event> Game::get_events_this_frame()
{
	sf::Event event;
	std::vector<sf::Event> events_this_frame;
	while (window_handle.pollEvent(event))
	{
		events_this_frame.push_back(event);
	}
	return events_this_frame;
}

void Game::read_input()
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

PlayerEntity::MOVE_STATE Game::key_press_to_state(bool pressed, PlayerEntity::MOVE_STATE old_state, PlayerEntity::MOVE_STATE target_state)
{
	PlayerEntity::MOVE_STATE new_state = old_state;
	if (pressed)
	{
		new_state = target_state;
	}
	else if (new_state == target_state)
	{
		new_state = PlayerEntity::MOVE_STATE::NOTHING;
	}
	return new_state;
}

void Game::handle_event_for_game(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
	{
		bool pressed = event.type == sf::Event::KeyPressed;
		if (event.key.code == sf::Keyboard::W)
		{
			player_entity->set_roll_state(key_press_to_state(pressed, player_entity->get_roll_state(), PlayerEntity::MOVE_STATE::FORWARD));
		}
		if (event.key.code == sf::Keyboard::S)
		{
			player_entity->set_roll_state(key_press_to_state(pressed, player_entity->get_roll_state(), PlayerEntity::MOVE_STATE::BACKWARD));
		}
		if (event.key.code == sf::Keyboard::A)
		{
			player_entity->set_pitch_state(key_press_to_state(pressed, player_entity->get_pitch_state(), PlayerEntity::MOVE_STATE::FORWARD));
		}
		if (event.key.code == sf::Keyboard::D)
		{
			player_entity->set_pitch_state(key_press_to_state(pressed, player_entity->get_pitch_state(), PlayerEntity::MOVE_STATE::BACKWARD));
		}
		if (event.key.code == sf::Keyboard::LShift)
		{
			player_entity->set_wants_to_sprint(pressed);
		}
	}

	if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2i mouse_pos(event.mouseMove.x, event.mouseMove.y);
		float yaw = atan2f(mouse_pos.y - player_entity->get_position().y, mouse_pos.x - player_entity->get_position().x);
		player_entity->set_yaw(yaw);
	}
}

void Game::render_filled_bar(int margin_screen,
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

void Game::render_game_interface()
{
	// Render stamina bar of the player in bottom left corner, first stamina then life
	const int margin_screen = 25;
	const int margin_bars = 10;
	const int border_width = 1;
	const sf::Vector2f bar_size(window_resolution.width * 0.15f, window_resolution.height * 0.015f);

	sf::Color stamina_bg = sf::Color(0, 127, 0);
	render_filled_bar(margin_screen, margin_bars, border_width, bar_size, 0, stamina_bg, sf::Color::Green, player_entity->get_stamina_fill_percentage());
}