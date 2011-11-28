// /////////////////////////////////////////////////////////////////
// @file Rectangle.cpp
// @author PJ O Halloran
// @date 12/12/2009
//
// File contains the implementation of the Rectangle class.
//
// /////////////////////////////////////////////////////////////////

// External headers
#include <cmath>

// Project headers
#include "Rectangle.h"

// Namespace Declarations

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Rectangle::Rectangle() : m_tl(0, 1), m_br(1, 0)
	{
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Rectangle::Rectangle(const Point &topLeft, const Point &bottomRight) : m_tl(topLeft), m_br(bottomRight)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Rectangle::Rectangle(const I32 top, const I32 bottom, const I32 left, const I32 right) : m_tl(left, top), m_br(right, bottom)
	{
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Rectangle::Rectangle(const Rectangle &recRef)
	{
		m_tl = recRef.m_tl;
		m_br = recRef.m_br;
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Rectangle::~Rectangle()
	{
		try
		{
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Rectangle::SetTopLeft(const Point &tlRef)
	{
		m_tl = tlRef;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const Point Rectangle::GetTopLeft() const
	{
		return (m_tl);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Rectangle::SetBottomRight(const Point &brRef)
	{
		m_br = brRef;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const Point Rectangle::GetBottomRight() const
	{
		return (m_br);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const I32 Rectangle::GetTop() const
	{
		return (m_tl.GetY());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const I32 Rectangle::GetBottom() const
	{
		return (m_br.GetY());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const I32 Rectangle::GetLeft() const
	{
		return (m_tl.GetX());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const I32 Rectangle::GetRight() const
	{
		return (m_br.GetX());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const U32 Rectangle::GetWidth() const
	{
		return (static_cast<U32>(std::abs(F32(m_br.GetX() - m_tl.GetX()))));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const U32 Rectangle::GetHeight() const
	{
		return (static_cast<U32>(std::abs(F32(m_tl.GetY() - m_br.GetY()))));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const U32 Rectangle::GetArea() const
	{
		return (GetWidth() * GetHeight());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Rectangle::IsSquare() const
	{
		return (GetWidth() == GetHeight());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Rectangle::operator=(const Rectangle &recRef)
	{
		m_tl = recRef.m_tl;
		m_br = recRef.m_br;
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Rectangle::operator==(const Rectangle &recRef)
	{
		if((m_tl == recRef.m_tl) && (m_br == recRef.m_br))
		{
			return (true);
		}

		return (false);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Rectangle::operator<(const Rectangle &recRef)
	{
		return (GetArea() < recRef.GetArea());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Rectangle::IsPointInside(const Point &pointRef) const
	{
		if(((pointRef.GetX() >= GetLeft())\
			&& (pointRef.GetX() <= GetRight()))\
			&& ((pointRef.GetY() >= GetTop())\
			&& (pointRef.GetY() <= GetBottom())))
		{
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const Point Rectangle::GetCentre() const
	{
		// Try to minize F32ing point error by rounding up.
		//  We round up as its either going to be X.0 in the case the Width/Height is even
		//		or X.5 in the case that Width/Height is odd. When a value is 0.5 or larger
		//			its better to round up than to truncate the value as C casting does.
		//				(Even this solution is not entirely accurate).
		I32 xC = m_tl.GetX() + static_cast<I32>(ceil(static_cast<F32>(GetWidth()) / 2.0f));
		I32 yC = m_br.GetY() + static_cast<I32>(ceil(static_cast<F32>(GetHeight()) / 2.0f));

		return (Point(xC, yC));
	}

}
