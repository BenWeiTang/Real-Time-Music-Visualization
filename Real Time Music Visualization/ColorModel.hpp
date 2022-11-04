#pragma once

#include <SFML/Graphics.hpp>

struct HSV
{
	double h, s, v;
	HSV(double h, double s, double v)
		: h(h), s(s), v(v) {}
};

HSV RGB_to_HSV(const sf::Color& rgb);

sf::Color HSV_to_RGB(const HSV& hsv); 