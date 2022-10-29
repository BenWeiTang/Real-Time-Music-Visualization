#pragma once

#include <SFML/Graphics.hpp>
#include "Pitch.hpp"
#include "PerlinNoise.hpp"

class Note
{
public:
	Note(const double& freq, const sf::Vector2f& startPosition);
	Note(Note&& other) noexcept; // Move contructor
	Note& operator=(Note&& other) noexcept; // Move assignment operator
	bool operator==(const Note& rhs) const;
	void Update();
	const Pitch::Pitch& GetPitch() const;
	const sf::Vector2f& GetPosition() const;

private:
	Pitch::Pitch m_Pitch;
	siv::PerlinNoise::seed_type m_Seed;
	siv::PerlinNoise m_Perlin;
	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
	float m_XVelocityScaleFactor;

	static const Pitch::Pitch IdentityPitch(double freq);
};
