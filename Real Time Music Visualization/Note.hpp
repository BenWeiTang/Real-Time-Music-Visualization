#pragma once

#include <SFML/Graphics.hpp>
#include "Pitch.hpp"
#include "PerlinNoise.hpp"

class Note
{
public:
	Note(const Pitch::Pitch& pitch,
		const sf::Vector2f& startPosition,
		const sf::Vector2f& velocity);
	void Update();

private:
	const Pitch::Pitch c_Pitch;
	const siv::PerlinNoise::seed_type c_Seed;
	const siv::PerlinNoise c_Perlin;
	sf::Vector2f m_Position;
	sf::Vector2f m_velocity;
};