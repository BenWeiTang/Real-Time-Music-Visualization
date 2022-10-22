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
		short absInterval = std::abs((short)a - (short)b);
		absInterval = std::min(absInterval, (short)(360 - absInterval));
		return absInterval;
	}
}
