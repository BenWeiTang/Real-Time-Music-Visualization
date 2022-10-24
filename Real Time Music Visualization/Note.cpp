#include <cmath>
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

const Pitch::Pitch& Note::GetPitch() const
{
	return c_Pitch;
}

const sf::CircleShape& Note::GetCircleShape() const
{
	return m_CircleShape;
}

const Pitch::Pitch Note::IdentityPitch(double freq)
{
	int keyNum = std::round(12*std::log2(freq/440.0)); // A will be 0, A# will be 1, ... 
	keyNum %= 12;
	switch (keyNum)
	{
	case 0:
		return Pitch::Pitch::A;
	case 1:
		return Pitch::Pitch::b;
	case 2:
		return Pitch::Pitch::B;
	case 3:
		return Pitch::Pitch::C;
	case 4:
		return Pitch::Pitch::d;
	case 5:
		return Pitch::Pitch::D;
	case 6:
		return Pitch::Pitch::e;
	case 7:
		return Pitch::Pitch::E;
	case 8:
		return Pitch::Pitch::F;
	case 9:
		return Pitch::Pitch::g;
	case 10:
		return Pitch::Pitch::G;
	case 11:
		return Pitch::Pitch::a;
	default:
		break;
	}
}
