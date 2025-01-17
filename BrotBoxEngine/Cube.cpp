#include "BBE/Cube.h"
#include "BBE/VertexWithNormal.h"
#include "BBE/List.h"
#include "BBE/Math.h"
#include "string.h"


bbe::INTERNAL::vulkan::VulkanBuffer bbe::Cube::s_indexBuffer;
bbe::INTERNAL::vulkan::VulkanBuffer bbe::Cube::s_vertexBuffer;
size_t bbe::Cube::amountOfIndices = 0;

void bbe::Cube::s_init(VkDevice device, VkPhysicalDevice physicalDevice, INTERNAL::vulkan::VulkanCommandPool & commandPool, VkQueue queue)
{
	s_initVertexBuffer(device, physicalDevice, commandPool, queue);
	s_initIndexBuffer(device, physicalDevice, commandPool, queue);
}

void bbe::Cube::s_initIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, INTERNAL::vulkan::VulkanCommandPool & commandPool, VkQueue queue)
{
	bbe::List<uint32_t> indices = {
		 0,  1,  3,	//Bottom
		 1,  2,  3,
		 5,  4,  7,	//Top
		 6,  5,  7,
		 9,  8, 11,	//Left
		10,  9, 11,
		12, 13, 15,	//Right
		13, 14, 15,
		16, 17, 19,	//Front
		17, 18, 19,
		21, 20, 23,	//Back
		22, 21, 23,
	};

	amountOfIndices = indices.getLength();

	s_indexBuffer.create(device, physicalDevice, sizeof(uint32_t) * indices.getLength(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

	void* dataBuf = s_indexBuffer.map();
	memcpy(dataBuf, indices.getRaw(), sizeof(uint32_t) * indices.getLength());
	s_indexBuffer.unmap();

	s_indexBuffer.upload(commandPool, queue);
}

void bbe::Cube::s_initVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, INTERNAL::vulkan::VulkanCommandPool & commandPool, VkQueue queue)
{
	bbe::List<INTERNAL::VertexWithNormal> vertices = {
		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5, -0.5), Vector3(0, 0, -1)),
		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5, -0.5), Vector3(0, 0, -1)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5, -0.5), Vector3(0, 0, -1)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5, -0.5), Vector3(0, 0, -1)),

		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5,  0.5), Vector3(0, 0,  1)),
		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5,  0.5), Vector3(0, 0,  1)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5,  0.5), Vector3(0, 0,  1)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5,  0.5), Vector3(0, 0,  1)),

		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5, -0.5), Vector3(0, -1, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5,  0.5), Vector3(0, -1, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5,  0.5), Vector3(0, -1, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5, -0.5), Vector3(0, -1, 0)),

		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5, -0.5), Vector3(0,  1, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5,  0.5), Vector3(0,  1, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5,  0.5), Vector3(0,  1, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5, -0.5), Vector3(0,  1, 0)),

		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5, -0.5), Vector3(-1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5,  0.5), Vector3(-1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5,  0.5), Vector3(-1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5, -0.5), Vector3(-1, 0, 0)),

		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5, -0.5), Vector3( 1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5,  0.5), Vector3( 1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5,  0.5), Vector3( 1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5, -0.5), Vector3( 1, 0, 0)),
	};

	for (size_t i = 0; i < vertices.getLength(); i++)
	{
		vertices[i].m_normal = bbe::Math::interpolateLinear(vertices[i].m_pos, vertices[i].m_normal, 0.5f);

		vertices[i].m_normal = vertices[i].m_normal.normalize();
	}

	s_vertexBuffer.create(device, physicalDevice, sizeof(INTERNAL::VertexWithNormal) * vertices.getLength(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

	void* dataBuf = s_vertexBuffer.map();
	memcpy(dataBuf, vertices.getRaw(), sizeof(INTERNAL::VertexWithNormal) * vertices.getLength());
	s_vertexBuffer.unmap();

	s_vertexBuffer.upload(commandPool, queue);
}

void bbe::Cube::s_destroy()
{
	s_indexBuffer.destroy();
	s_vertexBuffer.destroy();
}

bbe::Cube::Cube()
{
	//UNTESTED
}

bbe::Cube::Cube(const Vector3 & pos, const Vector3 & scale, const Vector3 & rotationVector, float radians)
{
	//UNTESTED
	set(pos, scale, rotationVector, radians);
}

bbe::Cube::Cube(const Matrix4& matTranslation, const Matrix4& matScale, const Matrix4& matRotation)
{
	set(matTranslation, matScale, matRotation);
}

bbe::Cube::Cube(const Vector3& pos, const Matrix4& matScale, const Matrix4& matRotation)
{
	set(pos, matScale, matRotation);
}

bbe::Cube::Cube(const Matrix4 & transform)
	: m_transform(transform)
{
	//UNTESTED
}


void bbe::Cube::set(const Vector3 & pos, const Vector3 & scale, const Vector3 & rotationVector, float radians)
{
	//UNTESTED
	const Matrix4 matTranslation = Matrix4::createTranslationMatrix(pos);
	const Matrix4 matScale = Matrix4::createScaleMatrix(scale);
	const Matrix4 matRotation = Matrix4::createRotationMatrix(radians, rotationVector);
	set(matTranslation, matScale, matRotation);
}

void bbe::Cube::set(const Matrix4& matTranslation, const Matrix4& matScale, const Matrix4& matRotation)
{
	m_transform = matTranslation * matRotation * matScale;
}

void bbe::Cube::set(const Vector3& pos, const Matrix4& matScale, const Matrix4& matRotation)
{
	set(Matrix4::createTranslationMatrix(pos), matScale, matRotation);
}

void bbe::Cube::setRotation(const Vector3 & rotationVector, float radians)
{
	const Matrix4 matTranslation = Matrix4::createTranslationMatrix(m_transform.extractTranslation());
	const Matrix4 matScale = Matrix4::createScaleMatrix(m_transform.extractScale());
	const Matrix4 matRotation = Matrix4::createRotationMatrix(radians, rotationVector);

	m_transform = matTranslation * matRotation * matScale;
}

void bbe::Cube::setPosition(const Vector3& pos)
{
	const Matrix4 matTranslation = Matrix4::createTranslationMatrix(pos);
	const Matrix4 matScale = Matrix4::createScaleMatrix(m_transform.extractScale());
	const Matrix4 matRotation = m_transform.extractRotation();
	set(matTranslation, matScale, matRotation);
}

void bbe::Cube::translate(const bbe::Vector3& translation)
{
	const Matrix4 matScale = Matrix4::createScaleMatrix(m_transform.extractScale());
	const Matrix4 matRotation = m_transform.extractRotation();
	set(getPos() + translation, matScale, matRotation);
}

bbe::Vector3 bbe::Cube::getPos() const
{
	//UNTESTED
	return m_transform.extractTranslation();
}

float bbe::Cube::getX() const
{
	//UNTESTED
	return getPos().x;
}

float bbe::Cube::getY() const
{
	//UNTESTED
	return getPos().y;
}

float bbe::Cube::getZ() const
{
	//UNTESTED
	return getPos().z;
}

bbe::Vector3 bbe::Cube::getCenter() const
{
	return getPos();
}

bbe::Vector3 bbe::Cube::getScale() const
{
	//UNTESTED
	return m_transform.extractScale();
}

float bbe::Cube::getWidth() const
{
	//UNTESTED
	return getScale().x;
}

float bbe::Cube::getHeight() const
{
	//UNTESTED
	return getScale().z;
}

float bbe::Cube::getDepth() const
{
	//UNTESTED
	return getScale().y;
}

bbe::Matrix4 bbe::Cube::getTransform() const
{
	//UNTESTED
	return m_transform;
}

bbe::List<bbe::Vector3> bbe::Cube::getNormals() const
{
	bbe::List<bbe::Vector3> retVal;
	retVal.resizeCapacityAndLength(6);

	const bbe::Matrix4 rotationMatrix = m_transform.extractRotation();
	retVal[0] = rotationMatrix * bbe::Vector3(1, 0, 0);
	retVal[1] = -retVal[0];
	retVal[2] = rotationMatrix * bbe::Vector3(0, 1, 0);
	retVal[3] = -retVal[2];
	retVal[4] = rotationMatrix * bbe::Vector3(0, 0, 1);
	retVal[5] = -retVal[4];

	return retVal;
}

void bbe::Cube::getVertices(bbe::List<bbe::Vector3>& outVertices) const
{
	outVertices.clear();

	outVertices.add(m_transform * bbe::Vector3(+0.5f, +0.5f, +0.5f));
	outVertices.add(m_transform * bbe::Vector3(+0.5f, +0.5f, -0.5f));
	outVertices.add(m_transform * bbe::Vector3(+0.5f, -0.5f, +0.5f));
	outVertices.add(m_transform * bbe::Vector3(+0.5f, -0.5f, -0.5f));
	outVertices.add(m_transform * bbe::Vector3(-0.5f, +0.5f, +0.5f));
	outVertices.add(m_transform * bbe::Vector3(-0.5f, +0.5f, -0.5f));
	outVertices.add(m_transform * bbe::Vector3(-0.5f, -0.5f, +0.5f));
	outVertices.add(m_transform * bbe::Vector3(-0.5f, -0.5f, -0.5f));
}

bbe::Vector3 bbe::Cube::approach(const bbe::Cube& other, const bbe::Vector3& approachVector) const
{
	//TODO This implementation is very, very naive. It can be implemented in a much faster
	// 	   and robust way.
	//     See: https://www.codeproject.com/Articles/15573/2D-Polygon-Collision-Detection

	// If we already intersect then we can't approach any further.
	if (intersects(other)) return bbe::Vector3(0, 0, 0);

	bbe::Vector3 pos = getPos();
	bbe::Matrix4 scale = bbe::Matrix4::createScaleMatrix(getScale());
	bbe::Matrix4 rotation = m_transform.extractRotation();

	// If we don't collide after the approach then we return the full approachVector.
	// TODO: This is sensitive for bullet through paper!
	if (!other.intersects(bbe::Cube(pos + approachVector, scale, rotation))) return approachVector;

	float upperBound = 1;
	float lowerBound = 0;
	// 40 iterations are probably enough...
	// In tests the 40 iterations were never reached (halfWay break happened)
	for (int i = 0; i < 40; i++)
	{
		float halfWay = (upperBound + lowerBound) * 0.5f;
		if (halfWay == upperBound || halfWay == lowerBound) break;
		if (other.intersects(bbe::Cube(pos + approachVector * halfWay, scale, rotation)))
		{
			upperBound = halfWay;
		}
		else
		{
			lowerBound = halfWay;
		}
	}

	return approachVector * lowerBound;
}
