#include "NotePool.hpp"

NotePool::NotePool(size_t poolSize) :
	m_MaxSize(poolSize),
	m_Size(0),
	m_EndIndex(0)
{
	// No default constructor for Note type so juse use malloc
	m_Notes = (Note*)malloc(sizeof(Note) * poolSize);
}

NotePool::~NotePool() { free(m_Notes); }

void NotePool::EmplaceBack(Note&& note)
{
	m_Notes[m_EndIndex] = note;
	m_EndIndex++;
	m_EndIndex %= m_MaxSize;
	m_Size = std::min(m_Size + 1, m_MaxSize); //TODO: can be removed later
}

unsigned int NotePool::GetMaxSize()
{
	return m_MaxSize;
}

//TODO: can be removed later
unsigned int NotePool::GetSize()
{
	return m_Size;
}

unsigned int NotePool::GetEndIndex()
{
	return m_EndIndex;
}
