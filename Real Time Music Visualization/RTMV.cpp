#define _USE_MATH_DEFINES
#include <cmath> // PI
#include <iostream>
#include <string>
#include "RTMV.hpp"
#include "ColorModel.hpp" // Capture Interval
#include "tinyfiledialogs.hpp" // File selection

#define BUFFER_SIZE 16384 // Number of samples to taker per FFT operation.
#define HOP_SIZE 4096 // Size of the hopping window. The larger the hop size, the less frequenty notes will be added.
#define FRAME_RATE_LIMIT 60 // Maximum FPS. The higer the maximum, the faster the Notes move and the more often the Intervals are captured.
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define POOL_SIZE 128 // Maximum size of NotePool.
#define PICKUP_THRESHOLD 1.5e+06 // Notes with amplitude lower than it will not be added to the NotePool.
#define CAPTURE_PERIOD 8 // The period of the capturing cycle, the larger the number, the less often the notes are captured. Be wary of low values as this can make the program memory expensive
#define MIN_CONNECT_DISTANCE 10.f // The interval of two Notes will not be captured if the distance between them is shorter than this.
#define MAX_CONNECT_DISTANCE 40.f // The interval of two Notes will not be captured if the distance between them is longer than this.
#define VERTEX_BUFFER_POOL_SIZE 4096 // The size of each sub pool in the VertexBufferPool. The larger the size, the more to copy each time but less often.

RTMV::RTMV() :
	m_BufferSize(BUFFER_SIZE),
	m_HalfBufferSize(BUFFER_SIZE >> 1),
	m_SampleRate(0),
	m_Samples(nullptr),
	m_SampleCount(0),
	m_HopSize(HOP_SIZE),
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
	m_CurrentLines(sf::VertexArray(sf::Lines)),
	m_PastLineCache(sf::VertexArray(sf::Lines, VERTEX_BUFFER_POOL_SIZE)),
	m_LineHistory(VERTEX_BUFFER_POOL_SIZE)
{
	m_Window.setFramerateLimit(FRAME_RATE_LIMIT);
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
	m_Window.clear();
	m_Window.display();

	while (m_Window.isOpen())
	{
		// Stops the program when the music is finished playing
		if (m_CurrentOffset + m_HalfBufferSize >= m_SampleCount)
			break;

		HandleEvents();
		HandleHopping();
	}

	// Only take a screenshot when the music is finished playing; not when the user closes the window
	if (m_CurrentOffset + m_HalfBufferSize >= m_SampleCount)
		SaveScreenshot();
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
	}

	if (!m_Notes.IsReady())
		return;

	UpdateNotes();
	CaptureIntervals();
	Draw();
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
			m_WindowedSamples[i] = Complex(m_Samples[i + m_CurrentOffset - m_HalfBufferSize] * m_Hann[i], 0);
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
	float xPosition = SCREEN_WIDTH * 0.8 * (m_Sound.getPlayingOffset().asSeconds() / m_SoundBuffer.getDuration().asSeconds()) + SCREEN_WIDTH * 0.1;
	float yPosition = SCREEN_HEIGHT * 0.5;
	double maxFreq = MaxFreq();
	if (maxFreq < 0)
		return;
	m_Notes.EmplaceBack(Note(MaxFreq(), sf::Vector2f(xPosition, yPosition)));
}

// Returns the frequency with the largest amplitude
// Returns -1.0 if the max amplitude at the moment is less than PICKUP_THRESHOLD
double RTMV::MaxFreq()
{
	int maxIndex = -1;
	double maxAmp = -1.0;
	static const int lowBound = 28 * m_BufferSize / m_SampleRate; // Lowest note on a piano is 27.5 Hz
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
	return maxAmp > PICKUP_THRESHOLD ? maxFreq : -1.0;
}

void RTMV::UpdateNotes()
{
	for (auto& note : m_Notes)
		note.Update();
}

void RTMV::CaptureIntervals()
{
	static int tick = 0;
	for (const auto& note : m_Notes)
	{
		for (const auto& other : m_Notes)
		{
			const auto& notePosition = note.GetPosition();
			const auto& otherPosition = other.GetPosition();
			float noteX = notePosition.x, noteY = notePosition.y;
			float otherX = otherPosition.x, otherY = otherPosition.y;
			float distance = std::sqrt(std::pow(noteX - otherX, 2) + std::pow(noteY - otherY, 2));
			const auto midPoint = sf::Vector2f(0.5 * (noteX + otherX), 0.5 * (noteY + otherY));
			if (note == other || distance > MAX_CONNECT_DISTANCE || distance < MIN_CONNECT_DISTANCE)
				continue;

			const auto interval = Pitch::Interval(note.GetPitch(), other.GetPitch());
			const auto color = Pitch::GetIntervalColor(interval);
			m_CurrentLines.append(sf::Vertex(notePosition, color));
			m_CurrentLines.append(sf::Vertex(midPoint, color));

			if (tick > CAPTURE_PERIOD)
			{
				m_PastLineCache.append(sf::Vertex(notePosition, color));
				m_PastLineCache.append(sf::Vertex(midPoint, color));

				if (m_PastLineCache.getVertexCount() > VERTEX_BUFFER_POOL_SIZE-1)
				{
					m_LineHistory.AddSubPool(m_PastLineCache);
					m_PastLineCache.clear();
				}
			}
		}
	}
	if (tick++ > CAPTURE_PERIOD)
		tick = 0;
}

void RTMV::Draw()
{
	m_Window.clear();
	m_Window.draw(m_CurrentLines);
	m_Window.draw(m_PastLineCache);
	for (int i = 0; i < m_LineHistory.GetSubPoolCount(); i++)
	{
		m_Window.draw(*m_LineHistory.GetSubPools()[i]);
	}
	m_Window.display();
	m_CurrentLines.clear();
}

void RTMV::SaveScreenshot()
{
	// Create an Image object
	const sf::Vector2u windowSize = m_Window.getSize();
	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(m_Window);
	const sf::Image image = texture.copyToImage();

	// Format default file name (full path)
	std::string defaultFileName(m_FileName);
	defaultFileName.resize(defaultFileName.size() - std::strlen(".wav")); // Get rid of ".wav" at the end

	// Format destination file name (full path)
	const char* pattern[1] = { "*.png" };
	const char* saveFileName = tinyfd_saveFileDialog("Save Current Screenshot", defaultFileName.c_str(), 1, pattern, "PNG file");
	
	// If confirmed to save, save the file to destination
	if (saveFileName != NULL)
	{
		image.saveToFile(saveFileName);
		std::cout << "Saved to file: " << saveFileName << std::endl;
	}
	else
	{
		std::cout << "No path was specified for the screenshot. Exit program." << std::endl;
	}
}
