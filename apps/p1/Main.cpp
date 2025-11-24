//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2022 Paulo Pagliosa.                              |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: Main.cpp
// ========
// Main function for cg template.
//
// Author: Paulo Pagliosa
// Last revision: 07/11/2022

#include "graphics/Application.h"
#include "include/MainWindow.h"

#include <cmath>
#include <fstream>

#include "include/Scene.h"
#include "include/Actor.h"
#include "include/Sphere.h"
#include "include/Plane.h"
#include "include/Raycaster.h"

using namespace cg;

inline static
void writeColor(std::ostream& out, const cg::Color& c)
{
	int iR = (int)(255.999 * c.x);
	int iG = (int)(255.999 * c.y);
	int iB = (int)(255.999 * c.z);

	out << iR << ' ' << iG << ' ' << iB << '\n';
}

/**
* @brief Creates a material
*
* @param r -- Red channel
* @param g -- Green channel
* @param b -- Blue channel
* @param alpha -- Optional, opacity value
*/
inline static
Reference<Material> createMaterial(const float& r, const float& g, const float& b, const float& alpha = 1)
{
	return Material::makeUse(new Material(Color{ r, g, b, alpha }));
}

int
main(int argc, char** argv)
{
	cg::Application{ new MainWindow(1600, 900) }.run(argc, argv);

	return 0;
}
