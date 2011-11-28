// /////////////////////////////////////////////////////////////////
// @file Circle.h
// @author PJ O Halloran
// @date 26/12/2009
//
// File contains the header for the Circle class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __CIRCLE_H
#define __CIRCLE_H

// External Headers

// Project Headers
#include "GameTypes.h"
#include "Point.h"

namespace GameHalloran
{
	// /////////////////////////////////////////////////////////////////
	// @class Circle
	// @author PJ O Halloran
	//
	// This class represents a 2D circle.
	//
	// /////////////////////////////////////////////////////////////////
	class Circle
	{
		private:
		
			Point m_loc;			///< The centre point of the circle.
			I32 m_r;				///< The radius.
		
		public:
		
			// /////////////////////////////////////////////////////////////////
			// Default constructor.
			// 
			// The Circle located at (0, 0) and has a radius of 1.
			//
			// /////////////////////////////////////////////////////////////////
			Circle();
			
			// /////////////////////////////////////////////////////////////////
			// Custom constructor.
			// 
			// Initializes the Circle with the position and radius supplied.
			//
			// @param Point The centre point of the circle.
			// @param I32 The radius of the circle.
			//
			// /////////////////////////////////////////////////////////////////
			Circle(const Point &locRef, const I32 radius);
			
			// /////////////////////////////////////////////////////////////////
			// Copy constructor.
			// 
			// Initializes the Circle with the Circle supplied.
			//
			// @param Circle The Circle to copy.
			//
			// /////////////////////////////////////////////////////////////////
			Circle(const Circle &circleRef);
			
			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			virtual	~Circle();

			// /////////////////////////////////////////////////////////////////
			// Set the current radius of the circle.
			//
			// /////////////////////////////////////////////////////////////////
			void SetRadius(const I32 radius);

			// /////////////////////////////////////////////////////////////////
			// Get the radius.
			//
			// /////////////////////////////////////////////////////////////////
			I32 GetRadius() const;
			
			// /////////////////////////////////////////////////////////////////
			// Set the centre point of the circle.
			//
			// /////////////////////////////////////////////////////////////////
			void SetCentrePoint(const Point &locRef);

			// /////////////////////////////////////////////////////////////////
			// Get the centre point of the circle.
			//
			// /////////////////////////////////////////////////////////////////
			Point GetCentrePoint() const;
			
			// /////////////////////////////////////////////////////////////////
			// Set the circle equal to the circle supplied.
			//
			// /////////////////////////////////////////////////////////////////
			void operator=(const Circle &circleRef);
			
			// /////////////////////////////////////////////////////////////////
			// Check if this circle is equivalent to the circle supplied.
			//
			// @return True if the circle is equivalent and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			bool operator==(const Circle &circleRef);
			
			// /////////////////////////////////////////////////////////////////
			// Check if this circle is less than to the circle supplied.
			//
			// You cannot really mathematically say one circle is less than
			// another, however we need to implement this operator to properly
			// sort and store objects of type Circle in an STL container.
			//
			// For this reason, if the centre point and radius is less then the
			// other circle, then it is deemed as "less than".
			//
			// @return True if the circle is less than and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			bool operator<(const Circle &circleRef);

			// /////////////////////////////////////////////////////////////////
			// Check if a point is inside the Circle.
			//
			// @return True if the point is inside or on the circumference of the
			//         circle and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			bool IsPointInside(const Point &pointRef) const;
			
	};
}

#endif
