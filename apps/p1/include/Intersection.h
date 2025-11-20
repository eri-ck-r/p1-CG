#ifndef __IntersectionInfo_h
#define __IntersectionInfo_h

#include "Actor.h"

using namespace cg;

/**
* @brief IntersectionInfo struct for ray casting
*
* @param Reference<Actor3> actor
* @param vec3f Interpoint;
*/
struct IntersectionInfo
{
	Reference<Actor3> actor;
	vec3f interPoint;	
};

#endif //__Intersection_h