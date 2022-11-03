#include "ColorModel.hpp"

HSV RGB_to_HSV(const sf::Color& rgb)
{
	float r = rgb.r / 255.0f;
	float g = rgb.g / 255.0f;
	float b = rgb.b / 255.0f;

	float cmax = std::max(r, std::max(g, b));
	float cmin = std::min(r, std::min(g, b));
	float diff = cmax - cmin;
	float h = -1.f, s = -1.f;

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
	float c = hsv.v * hsv.s;
	float x = c * (1 - std::abs(std::fmod((hsv.h / 60.f), 2) - 1));
	float m = hsv.v - c;
	float r = 0, g = 0, b = 0;

	if (hsv.h >= 0 && hsv.h <= 60)
	{
		r = c; g = x; b = 0;
	}
	else if (hsv.h >= 60 && hsv.h <= 120)
	{
		r = x; g = c; b = 0;
	}
	else if (hsv.h >= 120 && hsv.h <= 180)
	{
		r = 0; g = c; b = x;
	}
	else if (hsv.h >= 180 && hsv.h <= 240)
	{
		r = 0; g = x; b = c;
	}
	else if (hsv.h >= 240 && hsv.h <= 300)
	{
		r = x; g = 0; b = c;
	}
	else if (hsv.h >= 300 && hsv.h <= 360)
	{
		r = c; g = 0; b = x;
	}
	r = (r + m) * 255;
	g = (g + m) * 255;
	b = (b + m) * 255;

	return sf::Color(r, g, b);
}
