#include "Note.hpp"

Note::Note(const Pitch::Pitch& pitch, const sf::Vector2f& startPosition, const sf::Vector2f& velocity)
	: c_Pitch(pitch), c_Seed(time(0)), c_Perlin({ c_Seed }), m_Position(startPosition), m_velocity(velocity)
{
}

void Note::Update()
{
	// Reference: https://github.com/Reputeless/PerlinNoise
	m_velocity.x += c_Perlin.noise1D(m_Position.y);
	m_Position += m_velocity;
}
