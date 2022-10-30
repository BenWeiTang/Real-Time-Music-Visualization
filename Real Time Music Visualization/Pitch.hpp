#pragma once

namespace Pitch
{
	enum Pitch : short 
	{
		C = 0,   // C
		d = 210, // C sharp or D flat
		D = 60,  // D
		e = 270, // D sharp or E flat
		E = 120, // E
		F = 330, // F
		g = 180, // F sharp or G flat
		G = 30,  // G
		a = 240, // G sharp or A flat
		A = 90,  // A
		b = 300, // A sharp or B flat
		B = 150  // B
	};

	static inline short Interval(const Pitch& a, const Pitch& b)
	{
		short aValue = (short)a;
		short bValue = (short)b;
		short distance = std::abs(aValue - bValue);
		distance = std::min(distance, (short)(360 - distance));
		return distance * (aValue > bValue) - distance * (bValue > aValue);
	}

	static inline sf::Color GetIntervalColor(const short& interval)
	{
		switch (interval)
		{
		case 0:
			return sf::Color::Color(255, 255, 255, 32); // White
		case 30:
			return sf::Color::Color(0, 255, 128, 32); // Spring Green
		case 60:
			return sf::Color::Color(0, 255, 255, 32); // Cyan
		case 90:
			return sf::Color::Color(0, 128, 255, 32); // Azure
		case 120:
			return sf::Color::Color(0, 0, 255, 32); // Blue
		case 150:
			return sf::Color::Color(128, 0, 255, 32); // Violet
		case 180 || -180:
			return sf::Color::Color(255, 0, 255, 32); // Magenta
		case -30:
			return sf::Color::Color(128, 255, 0, 32); // Chartreuse
		case -60:
			return sf::Color::Color(255, 255, 0, 32); // Yellow
		case -90:
			return sf::Color::Color(255, 128, 0, 32); // Orange
		case -120:
			return sf::Color::Color(255, 0, 0, 32); // Red
		case -150:
			return sf::Color::Color(255, 0, 128, 32); // Rose
		default:
			break;
		}
	}
}
