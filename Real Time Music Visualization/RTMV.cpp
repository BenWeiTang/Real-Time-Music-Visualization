#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include <vector>
#include "RTMV.hpp"
#include "Note.hpp"
#include "tinyfiledialogs.hpp"

//#define BUFFER_SIZE 32768
#define BUFFER_SIZE 16384
//#define HOP_SIZE 4096
#define HOP_SIZE 8192
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define POOL_SIZE 512

RTMV::RTMV() :
	m_BufferSize(BUFFER_SIZE),
	m_HalfBufferSize(BUFFER_SIZE>>1),
	m_SampleRate(0),
	m_Samples(nullptr),
	m_SampleCount(0),
	m_HopSize(1),
	m_FileName(0),
	m_Window(
		sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
		"Realtime Music Visualization",
		sf::Style::Close
	),
	m_Hann(BUFFER_SIZE),
	m_CurrentOffset(0),
	m_WindowedSamples(BUFFER_SIZE),
	m_CoefScaleFactor(0),
	m_Notes(POOL_SIZE),
	m_Lines(sf::VertexArray(sf::Lines))
{
	m_Window.setFramerateLimit(60);
}

RTMV::~RTMV()
{
	delete[] m_Samples;
}

void RTMV::Begin()
{
	InitHannTable();

	if (!FindAudioFile())
		return;

	if (!LoadBuffer())
		return;

	PlayMusic();

	while (m_Window.isOpen())
	{
		HandleEvents();
		HandleHopping();
	}
}

// Initialize the lookup table of Hann window function
void RTMV::InitHannTable()
{
	// OG value in cos() is (2 * M_PI * i) / m_BufferSize
	// Cache into a factor to avoid repeated division operations
	double factor = 2 * M_PI / m_BufferSize;
	for (unsigned int i = 0; i < m_BufferSize; i++)
	{
		m_Hann[i] = 0.5 * (1 - cos(factor * i));
	}
}

// Return true if a wav file is selected, else false
bool RTMV::FindAudioFile()
{
	const char* pattern[1] = { "*.wav" };
	m_FileName = tinyfd_openFileDialog("Select an Audio File", NULL, 1, pattern, "wav file", 0);
	return m_FileName != NULL;
}

// Allocate memory for m_Samples to store samples
// Load m_SoundBuffer
bool RTMV::LoadBuffer()
{
	// Get a reference to the target file
	sf::InputSoundFile file;
	if (!file.openFromFile(m_FileName))
	{
		std::cout << "Unable to open file " << m_FileName << std::endl;
		return false;
	}
	m_SampleCount = file.getSampleCount();
	m_SampleRate = file.getSampleRate() * file.getChannelCount();
	m_CoefScaleFactor = (double)m_SampleRate / m_BufferSize;

	// Heap allocate an array of 16-bit int to store the samples
	// Keep reading from the target file until all samples are read
	m_Samples = new sf::Int16[m_SampleCount];
	sf::Uint64 count = 0;
	sf::Uint64 mark = 0;
	do
	{
		count = file.read((m_Samples + mark), 1024);
		mark += count;
	} while (count > 0);

	// Input samples into sound buffer
	m_SoundBuffer.loadFromSamples(m_Samples, m_SampleCount, file.getChannelCount(), file.getSampleRate());
	std::cout << "Successuflly loaded sound file " << m_FileName << std::endl;
	std::cout << "Duration: " << file.getDuration().asSeconds() << " s (" << m_SampleCount << " samples in total)" << std::endl;
	std::cout << "Sample Rate: " << file.getSampleRate() << std::endl;
	std::cout << "Channel(s): " << file.getChannelCount() << std::endl;
	return true;
}

// Play music using m_SoundBuffer
void RTMV::PlayMusic()
{
	m_Sound.setBuffer(m_SoundBuffer);
	m_Sound.play();
}

// Handle window events, e.g., closing window
void RTMV::HandleEvents()
{
	sf::Event event;

	while (m_Window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_Window.close();
			break;
		default:
			break;
		}
	}
}

// Regulate when hopping happens
// How often it hops is determined by m_HopSize
// When it's time to hop, call STFT() and then Draw()
void RTMV::HandleHopping()
{
	sf::Int32 currentPosition = m_Sound.getPlayingOffset().asSeconds() * m_SampleRate;
	if (currentPosition > m_CurrentOffset + m_HopSize)
	{
		m_CurrentOffset = currentPosition;
		STFT();
		AddNote();
		UpdateNotes();
		CaptureIntervals();
		Draw();
	}
}

// Perform Short Time Fourier Transform
void RTMV::STFT()
{
	// Copy desired range from m_Samples and apply Hann window function
	// Mag => real part; 0 => imaginary part
	if (m_CurrentOffset + m_HalfBufferSize < m_SampleCount && m_CurrentOffset > m_HalfBufferSize)
	{
		for (int i = 0; i < m_BufferSize; i++)
		{
			m_WindowedSamples[i] = Complex(m_Samples[i+m_CurrentOffset-m_HalfBufferSize]*m_Hann[i], 0);
		}
		
	}
	// Perform FFT on the samples
	m_Coefficients = CArray(m_WindowedSamples.data(), m_BufferSize);
	FFT(m_Coefficients);
}

// Reference: https://rosettacode.org/wiki/Fast_Fourier_transform#C++
// Perform Fast Fourier Transform
void RTMV::FFT(CArray& x)
{
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
}

void RTMV::AddNote()
{
	float xPosition = SCREEN_WIDTH * 0.85 * (m_Sound.getPlayingOffset().asSeconds() / m_SoundBuffer.getDuration().asSeconds());
	float yPosition = SCREEN_HEIGHT * 0.5;
	m_Notes.EmplaceBack(Note(MaxFreq(), sf::Vector2f(xPosition, yPosition)));
}

// Returns the frequency with the largest amplitude
double RTMV::MaxFreq()
{
	int maxIndex = -1;
	double maxAmp = -1.0;
	static const int lowBound = 20 * m_BufferSize / m_SampleRate;
	static const int highBound = m_BufferSize * std::min(20000u, m_HalfBufferSize) / m_SampleRate;
	for (int i = lowBound; i < highBound; i++)
	{
		double currentAmp = std::abs(m_Coefficients[i]);
		if (currentAmp > maxAmp)
		{
			maxAmp = currentAmp;
			maxIndex = i;
		}
	}
	double maxFreq = maxIndex * m_CoefScaleFactor;
	return maxFreq;
}

void RTMV::UpdateNotes()
{
	for (auto& note : m_Notes)
		note.Update();
}

void RTMV::CaptureIntervals()
{
	for (const auto& note : m_Notes)
	{
		for (const auto& other : m_Notes)
		{
			float noteX = note.GetPosition().x;
			float noteY = note.GetPosition().y;
			float otherX = other.GetPosition().x;
			float otherY = other.GetPosition().y;
			if (note == other || std::abs(noteX - otherX) > 50.f || std::abs(noteY - otherY) > 50.f)
				continue;
			m_Lines.append(sf::Vertex(note.GetPosition(), sf::Color::White));
			m_Lines.append(sf::Vertex(other.GetPosition(), sf::Color::White));
		}
	}
}

void RTMV::Draw()
{
	m_Window.clear();
	m_Window.draw(m_Lines);
	m_Window.display();
	m_Lines.clear();
}
