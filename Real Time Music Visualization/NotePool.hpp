#pragma once
#include <vector>
#include "Note.hpp"

class NotePool
{
public:
	NotePool(size_t n);
	void EmplaceBack(const Note& note);
	unsigned int GetMaxSize();
	unsigned int GetSize();
	unsigned int GetEndIndex();
private:
	std::vector<Note> m_Notes;
	unsigned int m_MaxSize;
	unsigned int m_Size; //TODO: can be removed later
	unsigned int m_EndIndex; // Exclusive
};
