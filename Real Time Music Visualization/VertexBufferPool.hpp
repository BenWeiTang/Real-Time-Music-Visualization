#pragma once

#include <vector> // SubPools
#include <SFML/Graphics.hpp> // VertexArray, VertexBuffer

class VertexBufferPool
{
public:
	VertexBufferPool(unsigned int subPoolSize);
	~VertexBufferPool();
	std::vector<sf::VertexBuffer*> GetSubPools() const;
	void AddSubPool(const sf::VertexArray& vertexArray);
private:
	const sf::VertexBuffer::Usage c_Usage;
	const sf::PrimitiveType c_Type;
	const unsigned int c_SubPoolSize;
	std::vector<sf::VertexBuffer*> m_SubPools;
};
