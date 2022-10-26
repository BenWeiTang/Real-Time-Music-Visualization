#include "NotePool.hpp"

NotePool::NotePool(size_t poolSize) :
	m_MaxSize(poolSize),
	m_Size(0),
	m_EndIndex(0)
{
	m_Notes.reserve(poolSize + 1);
}

void NotePool::EmplaceBack(const Note& note)
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