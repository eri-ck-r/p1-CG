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
#include "graphics/GLImage.h"
#include "MainWindow.h"

#include "Scene.h"
#include "Actor.h"
#include "Sphere.h"
#include "Plane.h"

void writeColor(std::ostream& out, const cg::Color& c)
{
	int iR = (int)(255.999 * c.x);
	int iG = (int)(255.999 * c.y);
	int iB = (int)(255.999 * c.z);

	out << iR << ' ' << iG << ' ' << iB << '\n';
}

int
main(int argc, char** argv)
{
	//puts("Ds template by Paulo Pagliosa (ppagliosa@gmail.com)\n");
	//puts("Camera controls keys:\n"
	//	"(w) move forward  (s) move backward\n"
	//	"(a) move left     (d) move right\n"
	//	"(q) move up       (z) move down\n");
	//puts("Mouse controls:\n"
	//	"(scroll wheel)    zoom\n"
	//	"(middle-click)    pan\n"
	//	"(Alt+right-click) rotate");

	using namespace cg;

	Reference<Scene> scene = Scene::makeUse(new Scene());
	Reference<Camera> camera = Camera::makeUse(new Camera());
	struct Settings
	{
		int m = 1024;
		int n = 768;
		int W = m;
		int H = n;
	} settings;

	Reference<Material> newMaterial = Material::makeUse(new Material(Color{1.0f,0.0f,0.0f}));

	Reference<Sphere> sphere = Sphere::makeUse(new Sphere({ 0.0f,0.0f ,0.0f }, 1));
	Reference<Plane> plane = Plane::makeUse(new Plane({ 0,0,0 }, { 0, 1, 0 }));

	Reference<Actor> sphereActor = Actor::makeUse(new Actor{ *sphere });
	sphereActor->setTransform(mat4f::identity());
	sphereActor->setMaterial(*newMaterial);

	Reference<Actor> planeActor = Actor::makeUse(new Actor{ *plane });
	planeActor->setTransform(mat4f::identity());

	scene->backgroundColor = Color{ 0.678f, 0.848f, 0.90f };
	scene->ambientLight = Color{ 0.412f, 0.412f, 0.412f };

	scene->actors.add(sphereActor);
	scene->actors.add(planeActor);

	camera->setPosition(vec3f{ 1.0f, 1.0f , 1.0f });
	camera->setDirectionOfProjection((vec3f{ 0,0,0 } - camera->position()).versor());
	camera->setViewUp({ 0.2f, 1.0f, 0.2f });
	camera->setNearPlane(0.5f);

	//GLImage image{ settings.m, settings.n };

	vec3f camU = camera->viewUp().cross(-camera->directionOfProjection()).versor();
	vec3f camN = -1.0f*(camera->directionOfProjection().versor());
	vec3f camV = camN.cross(camU);

	std::cout << "P3\n" << settings.m << ' ' << settings.n << "\n255\n";

	for (int j = 0; j < settings.n; ++j)
	{
		std::clog << "\rScanlines remaining: " << (settings.n - j) << ' ' << std::flush;
		for (int i = 0; i < settings.m; i++)
		{
			//determinar o Xp e o Yp
			float Xp = (settings.W / settings.m) * (i + 0.5f) - (settings.W / 2);
			float Yp = (settings.H / 2) - (settings.H / settings.n) * (j + 0.5f);
			float Zp = camera->nearPlane();

			vec3f p = (Xp * camU + Yp * camV - Zp * camN).versor();
			ray3f ray{ camera->position(), p };

			float minDistance = std::numeric_limits<float>::max();
			Color c = scene->backgroundColor;

			for (auto actor : scene->actors)
			{
				float t;
				if (actor->shape()->intersect(ray, t))
				{
					//std::clog << "\ro:" << ray.direction.x << "," << ray.direction.y << "," << ray.direction.z << std::flush;
					for (auto light : scene->lights)
					{

					}
					if (t < minDistance)
					{
						minDistance = t;
						c = actor->material()->diffuse;
					}
				}
			}
			//printar cor
			writeColor(std::cout, c);
		}
	}

	return cg::Application{ new MainWindow{1280, 720} }.run(argc, argv);
}
