#pragma once

#include <stdlib.h> // srand, rand
#include <time.h> // srand, rand
#include "ColorModel.hpp" // HSV_to_RGB

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
		std::srand(time(NULL));
		const static double hue = std::rand() % 360;
		const static double saturation = 0.125; // Saturation for perfect unisons
		const static double value = 1.0; // Value for perfect unisons
		const static double alpha = 0.125; // Alpha for all
		const static double hueStep = 180.0 / 6.0; // When interval is 180 deg apart (6 steps), rotate around the color wheel 180 deg
		const static double satStep = 1.0 / 12.0; // When interval is 180 deg apart (6 steps), saturation should increase by 0.5, thus divided by 12 (6 x 2)
		const static double valueStep = value / 12.0; // When interval is 180 deg apart (6 steps), value should be halved, thus divided by 12 (6 x 2)
		switch (interval)
		{
		case 0:
			return HSV_to_RGB(hue, saturation, value, alpha); // Perfect unison
		case 30:
			return HSV_to_RGB(std::fmod((hue + 1 * hueStep), 360), saturation + 1 * satStep, value - 1 * valueStep, alpha); // Up 1 perfect fifth
		case 60:
			return HSV_to_RGB(std::fmod((hue + 2 * hueStep), 360), saturation + 2 * satStep, value - 2 * valueStep, alpha); // Up 2 perfect fifth
		case 90:
			return HSV_to_RGB(std::fmod((hue + 3 * hueStep), 360), saturation + 3 * satStep, value - 3 * valueStep, alpha); // Up 3 perfect fifth
		case 120:
			return HSV_to_RGB(std::fmod((hue + 4 * hueStep), 360), saturation + 4 * satStep, value - 4 * valueStep, alpha); // Up 4 perfect fifth
		case 150:
			return HSV_to_RGB(std::fmod((hue + 5 * hueStep), 360), saturation + 5 * satStep, value - 5 * valueStep, alpha); // Up 5 perfect fifth
		case 180 || -180:
			return HSV_to_RGB(std::fmod((hue + 6 * hueStep), 360), saturation + 6 * satStep, value - 6 * valueStep, alpha); // Up 6 perfect fifth
		case -30:
			return HSV_to_RGB(std::fmod((hue - 1 * hueStep), 360), saturation + 1 * satStep, value - 1 * valueStep, alpha); // Down 1 perfect fifth
		case -60:
			return HSV_to_RGB(std::fmod((hue - 2 * hueStep), 360), saturation + 2 * satStep, value - 2 * valueStep, alpha); // Down 2 perfect fifth
		case -90:
			return HSV_to_RGB(std::fmod((hue - 3 * hueStep), 360), saturation + 3 * satStep, value - 3 * valueStep, alpha); // Down 3 perfect fifth
		case -120:
			return HSV_to_RGB(std::fmod((hue - 4 * hueStep), 360), saturation + 4 * satStep, value - 4 * valueStep, alpha); // Down 4 perfect fifth
		case -150:
			return HSV_to_RGB(std::fmod((hue - 5 * hueStep), 360), saturation + 5 * satStep, value - 5 * valueStep, alpha); // Down 5 perfect fifth
		default:
			break;
		}
	}
}
