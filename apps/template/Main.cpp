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

#include <cmath>
#include <fstream>

#include "Scene.h"
#include "Actor.h"
#include "Sphere.h"
#include "Plane.h"

using namespace cg;

struct Settings
{
	int m{};
	int n{};
	float W{};
	float H{};
	Settings(int m, float aspect) :
		m(m), n(int(m / aspect)), W(m), H(n)
	{
		//do nothing
	}
};

inline static
void writeHeader(std::ostream& out, Settings& settings)
{
	out << "P3\n" << settings.m << ' ' << settings.n << "\n255\n";
}

inline static
void writeColor(std::ostream& out, const cg::Color& c)
{
	int iR = (int)(255.999 * c.x);
	int iG = (int)(255.999 * c.y);
	int iB = (int)(255.999 * c.z);

	out << iR << ' ' << iG << ' ' << iB << '\n';
}

/**
* @brief Creates a Sphere; No rotation required.
*
* @param center -- Center coordinates
* @param radius -- Sphere radius
* @param scale -- vec3f scale in x, y and z axis
*/
inline static
Reference<Sphere> createSphere(const vec3f& center, const float& radius, const vec3f& scale = { 1.0f, 1.0f, 1.0f })
{
	Reference<Sphere> sphere = Sphere::makeUse(new Sphere({ {0.0f, 0.0f, 0.0f}, 1.0f }));
	sphere->setTransform(center, quatf::identity(), radius * scale);
	return sphere;
}

/**
*  @brief Creates a Plane.
*
*  @param P -- Point which the plane passes through
*  @param normal -- Plane normal
*  @param scale -- Vec2f scale, only in vertical and horizontal (x and y)
*/
inline static
Reference<Plane> createPlane(const vec3f& P, const vec3f& normal, const vec2f& scale = { 1.0f, 1.0f })
{
	Reference<Plane> plane = Plane::makeUse(new Plane(P, { 0.0f, 1.0f, 0.0f }));
	vec3f scale3 = { scale.x, 1.0f, scale.y };
	vec3f newAxis = vec3f{ 0.0f, 1.0f, 0.0f }.cross(normal).versor();
	quatf rotationQuat(cg::math::toDegrees(acos(newAxis.dot(normal))), newAxis);
	plane->setTransform(P, rotationQuat, scale3);
	return plane;
}

/**
* @brief Creates and insert an actor into the scene
*
* @param scene -- Scene to add the actor
* @param Material -- Actor's material
*/
inline static
void
createActor(Scene* scene, Shape3* shape, Material* material)
{
	Reference<Actor> actor = Actor::makeUse(new Actor(*shape));
	actor->setMaterial(*material);
	scene->actors.add(actor);
}

/*
* @brief Creates 3 lines for the x, y and z axis
*
* @param scene -- Scene to add the axes
* @param material1, 2 and 3 -- materials for x, y and z respectively
* @param material4 -- material for axis mapping
*/
inline static
void createAxis(Scene* scene, Material* material1, Material* material2, Material* material3, Material* material4)
{
	auto xAxis = createSphere(vec3f::null(), 1.0f, { 1000.0f, 0.1f, 0.1f });
	auto yAxis = createSphere(vec3f::null(), 1.0f, { 0.1f, 1000.0f, 0.1f });
	auto zAxis = createSphere(vec3f::null(), 1.0f, { 0.1f, 0.1f, 1000.0f });

	createActor(scene, xAxis, material1);
	createActor(scene, yAxis, material2);
	createActor(scene, zAxis, material3);

	/*for (int i = 0; i < 20; i++)
	{
		auto axisSphere = createSphere({ (float)i, 0.0f, 0.0f }, 0.12f, { 1.0f, 3.0f, 1.0f });
		createActor(scene, axisSphere, material4);
	}

	for (int i = 0; i < 20; i++)
	{
		auto axisSphere = createSphere({ 0.0f, (float)i, 0.0f }, 0.12f, { 3.0f, 1.0f, 1.0f });
		createActor(scene, axisSphere, material4);
	}

	for (int i = 0; i < 20; i++)
	{
		auto axisSphere = createSphere({ 0.0f, 0.0f, (float)i }, 0.12f, { 1.0f, 3.0f, 1.0f });
		createActor(scene, axisSphere, material4);
	}*/
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



/**
* @brief Creates a light and adds it to the scene
*
* @param scene -- The scene to which add the light
* @param position -- Vec3f Light position
* @param color -- Light's Color
*/
inline static
void createLight(Scene* scene, const vec3f& position, const Color& color)
{
	Reference<Light> light = Light::makeUse(new Light());
	light->setPosition(position);
	light->color = color;
	light->setType(Light::Type::Point);
	light->falloff = Light::Falloff::Constant;
	scene->lights.add(light);
}

inline static
void createSphereActor(Scene* scene,
	const vec3f& center,
	const float& radius,
	Material* material,
	const vec3f& scale)
{
	createActor(scene, createSphere(center, radius, scale), material);
}
int
main(int argc, char** argv)
{
	std::ofstream of{ "image.ppm" };

	Reference<Scene> scene = Scene::makeUse(new Scene());
	Reference<Camera> camera = Camera::makeUse(new Camera());
	Settings settings(1920, 16.0f / 9.0f);

	auto redMaterial = createMaterial(1.0f, 0.0f, 0.0f);
	auto greenMaterial = createMaterial(0.0f, 1.0f, 0.0f);
	auto blueMaterial = createMaterial(0.0f, 0.0f, 1.0f);
	auto pinkMaterial = createMaterial(0.788f, 0.2f, 0.753f);
	auto yellowMaterial = createMaterial(1.0f, 1.0f, 0.0f);
	auto purpleMaterial = createMaterial(0.435f, 0.0f, 1.0f);
	auto greyMaterial = createMaterial(0.5f, 0.5f, 0.5f);

	auto sphere1 = createSphere({ 0.0f, 2.0f, 0.0f }, 2.0f);
	auto sphere2 = createSphere({ 0.0f, -100.0f, 0.0f }, 100.0f);
	auto sphere3 = createSphere({ 4.0f, 4.0f, 0.0f }, 2.0f);
	auto sphere4 = createSphere({ -4.0f, 7.0f, 4.0f }, 2.0f, { 3.0f, 5.0f, 1.0f });

	createAxis(scene, yellowMaterial, purpleMaterial, greenMaterial, greyMaterial);

	auto plane1 = createPlane({ 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	auto plane2 = createPlane({ -3.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	auto plane3 = createPlane({ 3.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });

	createActor(scene, plane1, redMaterial);

	createActor(scene, sphere1, blueMaterial);
	createActor(scene, sphere2, pinkMaterial);
	createActor(scene, sphere3, redMaterial);
	//createActor(scene, sphere4, greenMaterial);

	scene->backgroundColor = Color{ 0.678f, 0.848f, 0.90f };
	scene->ambientLight = Color{ 0.412f, 0.412f, 0.412f };

	createLight(scene, { 4.0f, 2.0f, 3.0f }, Color{ 1.0f, 1.0f, 1.0f }); // branca 

	camera->setDefaultView(16.0f / 9.0f);
	//camera->setNearPlane(1.0f);
	camera->setPosition({ 15.0f, 3.0f, 10.0f });
	camera->setDirectionOfProjection((vec3f{ 0,0,1 } - camera->position()).versor());
	const auto& m = camera->cameraToWorldMatrix();

	vec3f camU, camV, camN;
	camU = m[0];
	camV = m[1];
	camN = m[2];

	writeHeader(of, settings);

	ray3f ray;

	//camera->setClippingPlanes(1.0f, 300.0f);
	settings.H = camera->windowHeight();
	settings.W = settings.H * (16.0f / 9.0f);

	camera->print();

	ray.set(camera->position(), -camN);

	float Zp = camera->nearPlane();
	for (int j = 0; j < settings.n; ++j)
	{
		std::clog << "\rScanlines remaining: " << (settings.n - j) << ' ' << std::flush;
		float Yp = (settings.H / 2.0f) - ((settings.H / (float)settings.n) * (j + 0.5f));
		for (int i = 0; i < settings.m; i++)
		{
			float Xp = (((settings.W * (i + 0.5f)) / (float)settings.m)) - (settings.W / 2.0f);

			vec3f p = (Xp * camU + Yp * camV - Zp * camN).versor();
			ray.direction = p;

			float minDistance = std::numeric_limits<float>::max();
			Color c = scene->backgroundColor;

			for (auto actor : scene->actors)
			{
				float t = std::numeric_limits<float>::max();

				if (actor->shape()->intersect(ray, t))
				{
					if (t < minDistance)
					{
						minDistance = t;
						c = actor->material()->ambient;

						// iluminaçao = Cd*Cl*(-N*Ll)
						// onde Cd = cor do material difuso, Cl = cor da luz(tem que calcular o falloff, N = normal
						// e Ll a direção do raio de luz ( é o lightray)
						vec3f interPoint = ray(minDistance);
						bool flag = false;
						for (auto light : scene->lights)
						{
							vec3f lightDirection = light->position() - interPoint;
							ray3f lightRay{ interPoint, lightDirection.versor() };
							Color lightColor = light->lightColor(lightDirection.length());

							for (auto shadowActor : scene->actors)
							{
								float shadowInterPoint;
								if (shadowActor->shape()->intersect(lightRay, shadowInterPoint) && shadowActor != actor
									&& math::isNegative(shadowInterPoint - lightDirection.length()))
								{
									flag = true;
									break;
								}
							}

							if (flag == false)
							{
								vec3f shapeNormal = actor->shape()->normalAt(interPoint);
								c += actor->material()->diffuse * lightColor * (shapeNormal.dot(lightRay.direction));
								vec3f reflectionDirection = ((-lightDirection).versor() - 2.0f * (shapeNormal.dot((-lightDirection).versor()) * shapeNormal)).versor();
								c += actor->material()->spot * lightColor * pow(-(reflectionDirection.dot(ray.direction)), 64);
							}
						}
					}
				}
			}
			writeColor(of, c);
		}
	}

	of.close();

	return 1;
}
