#pragma once
#include <cmath>
#define PI 3.1415926
template<class T> class Vec2
{
public:
	/** the x, y coordinates */
	T x, y;
	/** default constructor - sets x,y to 0,0 */
	Vec2() : x(0), y(0) { }

	/** initializes the Vec2 */
	Vec2(T x, T y) : x(x), y(y) { }

	/** @return true if the given x, y coordinate mathes this Vec2's data */
	bool is(T x, T y) const;

	/** @return true if the given x, y coordinate mathes this Vec2's data */
	bool is(Vec2 xy) const;

	/** @return true if the Vec2 is within the given rectangular boundary */
	bool isWithin(T minx, T miny, T maxx, T maxy);

	/** re-initialize */
	void init(T x, T y);

	/** @return a random Vec2 within the specified boundary */
	static Vec2 random(Vec2 min, Vec2 max);

	/** @return pythagorean distance from the origin */
	T magnitude();

	Vec2 operator+(Vec2 const & v) const;
	Vec2 operator-(Vec2 const & v) const;
	Vec2 operator*(T value) const;
	Vec2 operator/(T value) const;
	Vec2 & operator+=(Vec2& v);
	Vec2 & operator-=(Vec2& v);
	Vec2 & operator*=(T value);
	Vec2 & operator/=(T value);
	bool operator==(Vec2 const & v) const { return is(v); }
	bool operator!=(Vec2 const & v) const { return !is(v); }

	/**
	* @param A,B line 1
	* @param C,D line 2
	* @param point __OUT to the intersection of line AB and CD
	* @param dist __OUT the distance along line AB to the intersection
	* @return true if intersection occurs between the lines
	*/
	static bool lineIntersection(const Vec2 & A, const Vec2 & B,
		const Vec2 & C, const Vec2 & D,
		T & dist, Vec2 & point);

	/**
	* @return positive if v2 is clockwise of this vector
	* (assume Y points down, X to right)
	*/
	T sign(const Vec2 & v) const;

	/** @return true if this point is inside the given triangle */
	bool isInsideTriangle(Vec2 const & a, Vec2 const & b,
		Vec2 const & c) const;

	void limitMagnitude(T max);
};
/////////////////////////// Implementation ////////////////////////////
/** @return true if the given x, y coordinate mathes this Vec2's data */
template <class T> bool Vec2<T>::is(T x, T y) const
{
	return this->x == x && this->y == y;
}

/** @return true if the given x, y coordinate mathes this Vec2's data */
template <class T> bool Vec2<T>::is(Vec2<T> xy) const
{
	return xy.x == x && xy.y == y;
}

/** @return true if the Vec2 is within the given rectangular boundary */
template <class T> bool Vec2<T>::isWithin(T minx, T miny, T maxx, T maxy)
{
	return x >= minx && y >= miny && x < maxx && y < maxy;
}

/** re-initialize */
template <class T>
void Vec2<T>::init(T a_x, T a_y)
{
	x = a_x;
	y = a_y;
}

/** @return pythagorean distance from the origin */
template <class T> T Vec2<T>::magnitude()
{
	return (T)sqrt(x*x + y*y);
}

template <class T> Vec2<T> Vec2<T>::operator+(Vec2<T> const & v) const { return Vec2<T>(x + v.x, y + v.y); }
template <class T> Vec2<T> Vec2<T>::operator-(Vec2<T> const & v) const { return Vec2<T>(x - v.x, y - v.y); }
template <class T> Vec2<T> Vec2<T>::operator*(T value) const { return Vec2<T>(x*value, y*value); }
template <class T> Vec2<T> Vec2<T>::operator/(T value) const { return Vec2<T>(x / value, y / value); }
template <class T> Vec2<T> & Vec2<T>::operator+=(Vec2<T>& v) { x += v.x;   y += v.y;   return *this; }
template <class T> Vec2<T> & Vec2<T>::operator-=(Vec2<T>& v) { x -= v.x;   y -= v.y;   return *this; }
template <class T> Vec2<T> & Vec2<T>::operator*=(T value) { x *= value; y *= value; return *this; }
template <class T> Vec2<T> & Vec2<T>::operator/=(T value) { x /= value; y /= value; return *this; }

template <class T> inline T randInUnitInterval()
{
	const int RANDMAX = 32768;
	return (rand() % RANDMAX) / ((T)RANDMAX);
}

/** @return a random Vec2 within the specified boundary */
template <class T> Vec2<T> Vec2<T>::random(Vec2<T> min, Vec2<T> max)
{
	T w = max.x - min.x, h = max.y - min.y;
	return Vec2(
		randInUnitInterval<T>() * w + min.x,
		randInUnitInterval<T>() * h + min.y);
}

/**
* @param A,B line 1
* @param C,D line 2
* @param point __OUT to the intersection of line AB and CD
* @param dist __OUT the distance along line AB to the intersection
* @return true if intersection occurs between the lines
*/
template <class T> bool Vec2<T>::lineIntersection(const Vec2<T> & A, const Vec2<T> & B,
	const Vec2<T> & C, const Vec2<T> & D,
	T & dist, Vec2 & point)
{
	T rTop = (A.y - C.y)*(D.x - C.x) - (A.x - C.x)*(D.y - C.y);
	T rBot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);
	T sTop = (A.y - C.y)*(B.x - A.x) - (A.x - C.x)*(B.y - A.y);
	T sBot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);
	if ((rBot == 0) || (sBot == 0))
	{
		//lines are parallel
		return false;
	}
	T r = rTop / rBot;
	T s = sTop / sBot;
	Vec2 delta = B - A;
	dist = delta.magnitude() * r;
	point = A + (delta * r);
	return ((r > 0) && (r < 1) && (s > 0) && (s < 1));
}

/**
* @return positive if v2 is clockwise of this vector
* (assume Y points down, X to right)
*/
template <class T> T Vec2<T>::sign(const Vec2<T> & v) const
{
	return (x*v.y) - (y*v.x);
}

/** @return true if this point is inside the given triangle */
template <class T> bool Vec2<T>::isInsideTriangle(Vec2<T> const & a,
	Vec2<T> const & b,
	Vec2<T> const & c) const
{
	T signab = (*this - a).sign(b - a),
		signbc = (*this - b).sign(c - b),
		signac = (*this - c).sign(a - c);
	return(((signab >= 0) == (signbc >= 0)) && ((signbc >= 0) == (signac >= 0)))
		|| (((signab <= 0) == (signbc <= 0)) && ((signbc <= 0) == (signac <= 0)));
}

template <class T> void Vec2<T>::limitMagnitude(T max)
{
	T m = magnitude();
	if (m > max)
	{
		x = x * max / m;
		y = y * max / m;
	}
}
