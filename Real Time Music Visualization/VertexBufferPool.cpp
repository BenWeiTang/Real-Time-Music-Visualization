#include "VertexBufferPool.hpp"

VertexBufferPool::VertexBufferPool(unsigned int subPoolSize) :
	c_Usage(sf::VertexBuffer::Usage::Static),
	c_Type(sf::PrimitiveType::Lines),
	c_SubPoolSize(subPoolSize)
{
	m_SubPools.reserve(48);
}

VertexBufferPool::~VertexBufferPool()
{
	for (int i = 0; i < m_SubPools.size(); i++)
	{
		// Deallocate Vertex Buffer memory
		m_SubPools[i]->create(0);
		delete(m_SubPools[i]);
	}
}

std::vector<sf::VertexBuffer*> VertexBufferPool::GetSubPools() const { return m_SubPools; }

void VertexBufferPool::AddSubPool(const sf::VertexArray& vertexArray)
{
	// Heap allocation
	sf::VertexBuffer* buffer = new sf::VertexBuffer(c_Type, c_Usage);

	//TODO: maybe this is not needed?
	buffer->create(c_SubPoolSize);

	// Underlying data structure of VertexArray is std::vector which should be contiguous; fingers crossed
	buffer->update(&vertexArray[0], c_SubPoolSize, 0); 

	m_SubPools.emplace_back(std::move(buffer));
}
