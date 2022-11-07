#pragma once

#include <complex> // Complex array for FFT
#include <valarray> // Complex array for FFT
#include <vector> // Windowed samples
#include <SFML/Graphics.hpp> // VertexArray
#include <SFML/Audio.hpp> // Sound, SoundBuffer
#include "NotePool.hpp" // NotePool class
#include "VertexBufferPool.hpp" // Line Histories

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
	NotePool m_Notes;
	sf::VertexArray m_CurrentLines;
	sf::VertexArray m_PastLineCache;
	VertexBufferPool m_LineHistory;

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
	void AddNote();
	double MaxFreq();
	void UpdateNotes();
	void CaptureIntervals();
	void Draw();
	void SaveScreenshot();
};
