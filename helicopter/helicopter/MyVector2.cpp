// author Peter Lowe
#include "MyVector2.h"

/// <summary>
/// get length of vector using sqrt of the sum of the squares
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>length</returns>
float vectorLength(const sf::Vector2f t_vector)
{
	float sumOfSquares = (t_vector.x * t_vector.x ) + (t_vector.y * t_vector.y);
	const float length = sqrt(sumOfSquares);
	return length;
}

/// <summary>
/// return length squared may be smaller than length if
/// less than one
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>length squared</returns>
float vectorLengthSquared(const sf::Vector2f t_vector)
{
	const float sumOfSquares = (t_vector.x * t_vector.x) + (t_vector.y * t_vector.y);	
	return sumOfSquares;
}


float vectorCorssProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_vectorB)
{
	const float result = t_vectorA.x * t_vectorB.y - t_vectorA.y * t_vectorB.x;
	return result;	
}

float vectorDotProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_vectorB)
{
	const float result = (t_vectorA.x * t_vectorB.x) + (t_vectorA.y * t_vectorB.y);
	return result;
}

float vectorAngleBetween(const sf::Vector2f t_vectorA, const sf::Vector2f t_VectorB)
{
	float cosine = vectorDotProduct(vectorUnitVector(t_vectorA), vectorUnitVector(t_VectorB));
	if (cosine > 1.0f)
	{
		cosine = 1.0f;
	}
	if (cosine < -1.0f)
	{
		cosine = -1.0f;
	}
	const float angleInRadians = std::acos(cosine);
	const float  angleInDegrees = angleInRadians * 180.0f / PI;
	return angleInDegrees;
}

sf::Vector2f vectorRotateBy(const sf::Vector2f t_vector, const float t_angleRadians)
{
	const float cos = std::cos(t_angleRadians); //calculate once use twice
	const float sin = std::sin(t_angleRadians);
	const float xComponent = (t_vector.x * cos) - (t_vector.y * sin);
	const float yComponent = (t_vector.x * sin) + (t_vector.y * cos);
	const sf::Vector2f result{ xComponent, yComponent };
	return result;
}


sf::Vector2f vectorProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	float scale = vectorDotProduct( t_vector, t_onto) / vectorLengthSquared(t_onto);
	const sf::Vector2f result = t_onto * scale;
	return result;
}

sf::Vector2f vectorRejection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	const sf::Vector2f result = t_vector - vectorProjection(t_vector, t_onto);

	return result;
}

float vectorScalarProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	float result = vectorDotProduct(t_vector, t_onto) / vectorLength(t_onto);
	return result;  
}

sf::Vector2f vectorUnitVector(sf::Vector2f t_vector)
{
	sf::Vector2f result{ 0.0f,0.0f };
	const float lenght = vectorLength(t_vector);
	if (lenght != 0.0f) // never divide by zero unless the zombies are overrunning the base
	{
		result = sf::Vector2f{ t_vector.x / lenght, t_vector.y / lenght };
	}
	return result;
}
