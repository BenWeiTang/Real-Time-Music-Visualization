#pragma once

#include <algorithm> // std::clamp
#include <SFML/Graphics.hpp> // sf::Color

struct HSV
{
	double h, s, v, a;

	// h: [0, 360), s: [0, 1), v: [0, 1), a: [0, 1)
	HSV(double h, double s, double v, double a = 1.0)
		: h(std::clamp(h, 0.0, 360.0))
		, s(std::clamp(h, 0.0, 1.0))
		, v(std::clamp(v, 0.0, 1.0))
		, a(std::clamp(a, 0.0, 1.0))
	{}
};

HSV RGB_to_HSV(const sf::Color& rgb);

// r: [0, 255), g: [0, 255), b: [0, 255), a: [0, 255)
HSV RGB_to_HSV(const sf::Uint8& r, const sf::Uint8& g, const sf::Uint8& b, const sf::Uint8& a = 255u);

sf::Color HSV_to_RGB(const HSV& hsv); 

// h: [0, 360), s: [0, 1), v: [0, 1), a: [0, 1)
sf::Color HSV_to_RGB(const double& h, const double& s, const double& v, const double& a = 1.0); 