#ifndef __Intersection_h
#define __Intersection_h

#include "Actor.h"

using namespace cg;

/**
* @brief Intersection struct for ray casting
*
* @param Reference<Actor> actor
* @param vec3f Interpoint;
*/
struct Intersection
{
	Reference<Actor> actor;
	vec3f interPoint;	
};

#endif //__Intersection_h