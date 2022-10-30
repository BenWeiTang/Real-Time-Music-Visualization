#include "NotePool.hpp"

NotePool::Iterator::Iterator(Note* ptr, Note* initialStart, Note* initialEnd) : m_ptr(ptr), m_InitialStart(initialStart), m_InitialEnd(initialEnd) {}

NotePool::Iterator NotePool::Iterator::operator++()
{
	++m_ptr;
	if (m_ptr == m_InitialEnd)
		m_ptr = m_InitialStart;
	return *this;
}

bool NotePool::Iterator::operator!=(const NotePool::Iterator& other) const { return m_ptr != other.m_ptr; }

const Note& NotePool::Iterator::operator*() const { return *m_ptr; }

Note& NotePool::Iterator::operator*() { return *m_ptr; }

NotePool::NotePool(size_t poolSize) :
	m_MaxSize(poolSize),
	m_Size(0),
	m_EndIndex(0),
	m_IsReady(false)
{
	// No default constructor for Note type so juse use malloc
	m_Notes = (Note*)malloc(sizeof(Note) * poolSize);
}

NotePool::~NotePool() { free(m_Notes); }

NotePool::Iterator NotePool::begin() const { return Iterator(m_Notes + ((m_EndIndex+1) % m_Size), m_Notes, m_Notes + m_MaxSize); }

NotePool::Iterator NotePool::end() const { return Iterator(m_Notes + (m_EndIndex % m_Size), m_Notes, m_Notes + m_MaxSize); }

void NotePool::EmplaceBack(Note&& note)
{
	if (!m_IsReady)
		m_IsReady = true;

	m_Notes[m_EndIndex] = std::move(note);
	m_EndIndex++;
	m_EndIndex %= m_MaxSize;
	m_Size = std::min(m_Size + 1, m_MaxSize);
}

unsigned int NotePool::GetMaxSize() const { return m_MaxSize; }

unsigned int NotePool::GetEndIndex() const { return m_EndIndex; }

bool NotePool::IsReady () const { return m_IsReady; }
