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
			return sf::Color::Color(255, 255, 255, 64); // White
		case 30:
			return sf::Color::Color(0, 255, 128, 64); // Spring Green
		case 60:
			return sf::Color::Color(0, 255, 255, 64); // Cyan
		case 90:
			return sf::Color::Color(0, 128, 255, 64); // Azure
		case 120:
			return sf::Color::Color(0, 0, 255, 64); // Blue
		case 150:
			return sf::Color::Color(128, 0, 255, 64); // Violet
		case 180 || -180:
			return sf::Color::Color(255, 0, 255, 64); // Magenta
		case -30:
			return sf::Color::Color(128, 255, 0, 64); // Chartreuse
		case -60:
			return sf::Color::Color(255, 255, 0, 64); // Yellow
		case -90:
			return sf::Color::Color(255, 128, 0, 64); // Orange
		case -120:
			return sf::Color::Color(255, 0, 0, 64); // Red
		case -150:
			return sf::Color::Color(255, 0, 128, 64); // Rose
		default:
			break;
		}
	}
}
