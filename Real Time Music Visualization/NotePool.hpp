#pragma once
#include <vector>
#include "Note.hpp"

class NotePool
{
public:
	class Iterator
	{
	public:
		Iterator(Note* ptr, Note* initialStart, Note* initialEnd);
		Iterator operator++();
		bool operator!=(const Iterator& other) const;
		const Note& operator*() const;
		Note& operator*();
	private:
		Note* m_ptr;
		Note* m_InitialStart; // Inclusive
		Note* m_InitialEnd; // Exclusive
	};

	NotePool(size_t n);
	~NotePool();
	Iterator begin() const;
	Iterator end() const;
	void EmplaceBack(Note&& note);
	unsigned int GetMaxSize();
	unsigned int GetSize();
	unsigned int GetEndIndex();
private:
	Note* m_Notes;
	unsigned int m_MaxSize;
	unsigned int m_Size; //TODO: can be removed later
	unsigned int m_EndIndex; // Exclusive
};
