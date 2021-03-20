#include "BBE/Shape2.h"

bool bbe::Shape2::projectionsIntersect(const ProjectionResult& pr1, const ProjectionResult& pr2)
{
	const float pr1StartLengthSq = pr1.start.getLengthSqSigned();
	const float pr2StartLengthSq = pr2.start.getLengthSqSigned();

	if (pr1StartLengthSq < pr2StartLengthSq)
	{
		const float pr1StopLengthSq = pr1.stop.getLengthSqSigned();
		return pr2StartLengthSq <= pr1StopLengthSq;
	}
	else if (pr2StartLengthSq < pr1StartLengthSq)
	{
		const float pr2StopLengthSq = pr2.stop.getLengthSqSigned();
		return pr1StartLengthSq <= pr2StopLengthSq;
	}
	else
	{
		return true;
	}

	return false;
}

bbe::List<bbe::Vector2> bbe::Shape2::getVertices() const
{
	bbe::List<bbe::Vector2> retVal;
	getVertices(retVal);
	return retVal;
}

bbe::List<bbe::Vector2> bbe::Shape2::getNormals() const
{
	auto vertices = getVertices();
	bbe::List<bbe::Vector2> retVal;
	retVal.resizeCapacityAndLength(vertices.getLength());

	for (size_t i = 0; i < vertices.getLength() - 1; i++)
	{
		retVal[i] = vertices[i + 1] - vertices[i];
	}

	retVal[retVal.getLength() - 1] = vertices[0] - vertices[vertices.getLength() - 1];

	for (size_t i = 0; i < retVal.getLength(); i++)
	{
		retVal[i] = retVal[i].rotate90Clockwise();
		retVal[i] = retVal[i].normalize();
	}

	return retVal;
}

bbe::Shape2::ProjectionResult bbe::Shape2::project(const bbe::Vector2& projection) const
{
	auto vertices = getVertices();
	auto projections = bbe::Math::project(vertices, projection);

	float minLengthSq = projections[0].getLengthSqSigned();
	float maxLengthSq = minLengthSq;
	ProjectionResult retVal{ projections[0], projections[0] };

	for (size_t i = 1; i < projections.getLength(); i++)
	{
		const float lengthSq = projections[i].getLengthSqSigned();
		if (lengthSq > maxLengthSq)
		{
			maxLengthSq = lengthSq;
			retVal.stop = projections[i];
		}
		if (lengthSq < minLengthSq)
		{
			minLengthSq = lengthSq;
			retVal.start = projections[i];
		}
	}

	return retVal;
}

bool bbe::Shape2::intersects(const Shape2& other) const
{
	auto normalsThis = getNormals();
	auto normalsOther = other.getNormals();

	for (const bbe::Vector2& normal : normalsThis)
	{
		auto p1 = project(normal);
		auto p2 = other.project(normal);
		if (!projectionsIntersect(p1, p2)) return false;
	}

	for (const bbe::Vector2& normal : normalsOther)
	{
		auto p1 = project(normal);
		auto p2 = other.project(normal);
		if (!projectionsIntersect(p1, p2)) return false;
	}

	return true;
}
