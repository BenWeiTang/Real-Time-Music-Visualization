#include "ColorModel.hpp"
#include <iostream>

HSV RGB_to_HSV(const sf::Color& rgb)
{
	double r = rgb.r / 255.0;
	double g = rgb.g / 255.0;
	double b = rgb.b / 255.0;

	double cmax = std::max(r, std::max(g, b));
	double cmin = std::min(r, std::min(g, b));
	double diff = cmax - cmin;
	double h = -1.0, s = -1.0;

	if (cmax == cmin)
		h = 0.f;
	else if (cmax == r)
		h = std::fmod(60 * ((g - b) / diff) + 360, 360);
	else if (cmax == g)
		h = std::fmod(60 * ((b - r) / diff) + 120, 360);
	else if (cmax == b)
		h = std::fmod(60 * ((r - g) / diff) + 240, 360);

	if (cmax == 0)
		s = 0.f;
	else
		s = (diff / cmax) * 100;

	float v = cmax * 100;

	return HSV(h, s, v);
}

sf::Color HSV_to_RGB(const HSV& hsv)
{
	double h = hsv.h, s = hsv.s * 0.01, v = hsv.v * 0.01;
	double c = v * s;
	double x = c * (1 - std::abs(std::fmod((h / 60.0), 2) - 1));
	double m = v - c;
	double r = 0, g = 0, b = 0;

	if (h >= 0 && h < 60)
	{
		r = c; g = x; b = 0;
	}
	else if (h >= 60 && h < 120)
	{
		r = x; g = c; b = 0;
	}
	else if (h >= 120 && h < 180)
	{
		r = 0; g = c; b = x;
	}
	else if (h >= 180 && h < 240)
	{
		r = 0; g = x; b = c;
	}
	else if (h >= 240 && h < 300)
	{
		r = x; g = 0; b = c;
	}
	else if (h >= 300 && h < 360)
	{
		r = c; g = 0; b = x;
	}

	sf::Uint8 R = (r + m) * 255;
	sf::Uint8 G = (g + m) * 255;
	sf::Uint8 B = (b + m) * 255;
	return sf::Color(R, G, B);
}
