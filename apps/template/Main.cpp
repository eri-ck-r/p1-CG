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
#include "MainWindow.h"

#include <cmath>
#include <fstream>

#include "Scene.h"
#include "Actor.h"
#include "Sphere.h"
#include "Plane.h"
#include "Raycaster.h"

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
	std::ofstream out{"imagem.ppm"};

	Raycaster rc(1024, 16.0f/9.0f, out);
	rc.writeHeader();

	auto redMaterial = createMaterial(1.0f, 0.0f, 0.0f);
	auto greenMaterial = createMaterial(0.0f, 1.0f, 0.0f);
	auto blueMaterial = createMaterial(0.0f, 0.0f, 1.0f);
	auto pinkMaterial = createMaterial(0.788f, 0.2f, 0.753f);
	auto yellowMaterial = createMaterial(1.0f, 1.0f, 0.0f);
	auto purpleMaterial = createMaterial(0.435f, 0.0f, 1.0f);
	auto greyMaterial = createMaterial(0.5f, 0.5f, 0.5f);

	rc.createSphereActor({ 0.1f, 2.3f, 0.1f }, 2.0f, blueMaterial, {1.0f, 1.0f, 1.0f});
	rc.createSphereActor({ 0.0f, -100.0f, 0.0f }, 100.0f, pinkMaterial, {1.0f, 1.0f, 1.0f});
	//rc.createPlaneActor({ 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, pinkMaterial, { 100.0f, 100.0f });
	rc.createSphereActor({ 4.0f, 4.0f, 0.0f }, 2.0f, redMaterial, { 1.0f, 1.0f, 1.0f});

	//rc.createPlaneActor({ 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, redMaterial, {1.0f, 1.0f});

	rc.createLight({ 4.0f, 8.0f, 3.0f }, Color{ 1.0f, 1.0f, 1.0f }); // branca
	//rc.createAxis(redMaterial, blueMaterial, greenMaterial, greyMaterial, false);
	rc.camera()->setPosition({ 5.0f, 4.0f, -10.0f });
	rc.camera()->setDirectionOfProjection(vec3f::null() - rc.camera()->position());
	rc.render();
	out.close();

	return 0;
}
