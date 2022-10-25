#pragma once

#include <SFML/Graphics.hpp>
#include "Pitch.hpp"
#include "PerlinNoise.hpp"

class Note
{
public:
	Note(const double& freq, const sf::Vector2f& startPosition);
	bool operator==(const Note& rhs) const;
	void Update();
	const Pitch::Pitch& GetPitch() const;
	const sf::CircleShape& GetCircleShape() const;  //TODO: Remove later

private:
	const Pitch::Pitch c_Pitch;
	const siv::PerlinNoise::seed_type c_Seed;
	const siv::PerlinNoise c_Perlin;
	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
	sf::CircleShape m_CircleShape; //TODO: Remove later

	static const Pitch::Pitch IdentityPitch(double freq);
};
