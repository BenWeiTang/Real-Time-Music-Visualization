#include <cmath>
#include "Note.hpp"

Note::Note(const double& freq, const sf::Vector2f& startPosition)
	: m_Pitch(IdentityPitch(freq)), m_Seed(time(0)), m_Perlin({ m_Seed }), m_Position(startPosition), m_CircleShape(100.f, 30u) //TODO: Delete this CircleShape
{
	double initialY = m_Perlin.noise1D(m_Position.x);
	initialY *= 0.5; // Down scale by half
	initialY += 0.5 * (initialY > 0) - 0.5 * (initialY < 0); // Shift right by 0.5 if positive; shift left by 0.5 if negative; 0.5 < abs(initialY) < 1.0
	m_Velocity = sf::Vector2f(0.f, (float)initialY); // Randomly go up or down, no lateral movement yet.
	m_CircleShape.setPosition(m_Position);
}

// Move constructor
Note::Note(Note&& other) noexcept { *this = std::move(other); }

// Move assign operator
Note& Note::operator=(Note&& other) noexcept
{
	if (this != &other)
	{
		// Currently no memories to release before assignment

		m_Pitch = other.m_Pitch;
		m_Seed = other.m_Seed;
		m_Perlin = other.m_Perlin;
		m_Position = other.m_Position;
		m_Velocity = other.m_Velocity;
		m_CircleShape = other.m_CircleShape;

		// Currently no dangling pointers to wrap up
	}
	return *this;
}

bool Note::operator==(const Note& rhs) const
{
	// Perfect unisons at the same spot is considered as same
	return m_Pitch == rhs.m_Pitch && m_Position == rhs.m_Position;
}

void Note::Update()
{
	// Reference: https://github.com/Reputeless/PerlinNoise
	m_Velocity.x += 0.05f * m_Perlin.noise1D(m_Position.y);
	m_Position += m_Velocity;

	m_CircleShape.setPosition(m_Position); //TODO: delete later
}

const Pitch::Pitch& Note::GetPitch() const
{
	return m_Pitch;
}

const sf::Vector2f& Note::GetPosition() const
{
	return m_Position;
}

const sf::CircleShape& Note::GetCircleShape() const
{
	return m_CircleShape;
}

const Pitch::Pitch Note::IdentityPitch(double freq)
{
	//FIXME: freq lower than 440 may give negative numbers
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
