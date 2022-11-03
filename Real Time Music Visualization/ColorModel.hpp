#pragma once

#include <SFML/Graphics.hpp>

struct HSV
{
	float h, s, v;
	HSV(float h, float s, float v)
		: h(h), s(s), v(v) {}
};

HSV RGB_to_HSV(const sf::Color& rgb);

sf::Color HSV_to_RGB(const HSV& hsv); 