#include "Note.hpp"

Note::Note(const double& freq, const sf::Vector2f& startPosition, const sf::Vector2f& velocity)
	: c_Pitch(IdentityPitch(freq)), c_Seed(time(0)), c_Perlin({ c_Seed }), m_Position(startPosition), m_velocity(velocity), m_CircleShape(100.f, 30u) //TODO: Delete this CircleShape
{
	m_CircleShape.setPosition(m_Position);
}

void Note::Update()
{
	// Reference: https://github.com/Reputeless/PerlinNoise
	m_velocity.x += c_Perlin.noise1D(m_Position.y);
	m_Position += m_velocity;

	m_CircleShape.setPosition(m_Position); //TODO: delete later
}

const sf::CircleShape& Note::GetCircleShape() const
{
	return m_CircleShape;
}
