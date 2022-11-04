#include <cmath>
#include "Note.hpp"

#define X_MIN_VELOCITY_PERC 0.2
#define Y_MIN_VELOCITY_PERC 0.2

Note::Note(const double& freq, const sf::Vector2f& startPosition)
	: m_Pitch(IdentityPitch(freq)), m_Seed(time(0)), m_Perlin({ m_Seed }), m_Position(startPosition), m_XVelocityScaleFactor(5.f)
{
	double initialY = m_Perlin.noise1D(m_Position.x);
	initialY *= 1 - Y_MIN_VELOCITY_PERC; // Down scale by half
	initialY += Y_MIN_VELOCITY_PERC * (initialY > 0) - Y_MIN_VELOCITY_PERC * (initialY < 0); // Shift right by 0.5 if positive; shift left by 0.5 if negative; 0.5 < abs(initialY) < 1.0
	double initialX = m_Perlin.noise1D(10*time(0));
	initialX *= 1 - X_MIN_VELOCITY_PERC;
	initialX += X_MIN_VELOCITY_PERC * (initialX > 0) - X_MIN_VELOCITY_PERC * (initialX < 0);
	m_Velocity = sf::Vector2f((float)initialX, (float)initialY); // Randomly go up or down, no lateral movement yet.
	m_Position.x += m_Perlin.noise1D(10 * time(0));
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
	m_Velocity.x = m_Perlin.noise1D(m_Position.y * 0.01f);
	m_Position += m_Velocity;
}

const Pitch::Pitch& Note::GetPitch() const
{
	return m_Pitch;
}

const sf::Vector2f& Note::GetPosition() const
{
	return m_Position;
}

const Pitch::Pitch Note::IdentityPitch(double freq)
{
	int keyNum = std::round(12*std::log2(freq/440.0)+49); // A will be 49, A# will be 50, ... 
	keyNum %= 12;
	switch (keyNum)
	{
	case 1:
		return Pitch::Pitch::A;
	case 2:
		return Pitch::Pitch::b;
	case 3:
		return Pitch::Pitch::B;
	case 4:
		return Pitch::Pitch::C;
	case 5:
		return Pitch::Pitch::d;
	case 6:
		return Pitch::Pitch::D;
	case 7:
		return Pitch::Pitch::e;
	case 8:
		return Pitch::Pitch::E;
	case 9:
		return Pitch::Pitch::F;
	case 10:
		return Pitch::Pitch::g;
	case 11:
		return Pitch::Pitch::G;
	case 0:
		return Pitch::Pitch::a;
	default:
		break;
	}
}
