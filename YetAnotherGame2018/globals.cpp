#include "globals.hpp"

float timestep_f = 1.0f / 60.0f;

sf::VideoMode window_resolution(800, 600);
sf::Vector2i window_center_point(window_resolution.width / 2, window_resolution.height / 2);
sf::RenderWindow window_handle(window_resolution, "YAG2018");