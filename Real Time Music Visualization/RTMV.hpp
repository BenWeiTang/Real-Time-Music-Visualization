#pragma once

#include <complex>
#include <valarray>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

class RTMV
{
private:
	unsigned int m_BufferSize;
	unsigned int m_HalfBufferSize;
	unsigned int m_SampleRate;
	sf::Int16* m_Samples;
	sf::Uint64 m_SampleCount;
	unsigned int m_HopSize;
	char* m_FileName;
	sf::RenderWindow m_Window;
	std::vector<double> m_Hann;
	sf::Sound m_Sound;
	sf::SoundBuffer m_SoundBuffer;
	sf::Uint64 m_CurrentOffset;
	std::vector<Complex> m_WindowedSamples;
	CArray m_Coefficients;
	double m_CoefScaleFactor;
	unsigned int m_maxFreqLowBound;
	unsigned int m_maxFreqHighBound;

	sf::VertexArray VA2;

public:
	RTMV();
	~RTMV();
	void Begin();

private:
	void InitHannTable();
	void HandleEvents();
	bool FindAudioFile();
	bool LoadBuffer();
	void PlayMusic();
	void HandleHopping();
	void STFT();
	void FFT(CArray& x);
	double MaxFreq();
	void Draw();
};
