#ifndef __Raycaster_h
#define __Raycaster_h

#include "graphics/Application.h"
#include "graphics/Camera.h"

#include <cmath>
#include <fstream>
#include <iostream>

#include "Scene.h"
#include "Actor.h"
#include "Sphere.h"
#include "Plane.h"

using namespace cg;

class Raycaster
{
private:
	Reference<Scene> _scene;
	Reference<Camera> _camera;
	int _m;
	int _n;
	float aspectRatio;
	std::ostream* _out;

	Reference<Sphere> createSphere(const vec3f&, const float, const vec3f&);

	Reference<Plane> createPlane(const vec3f&, const vec3f&, const vec2f&);

	void createActor(Shape3*, Material*);

	/**
	* @brief Creates a material
	*
	* @param r -- Red channel
	* @param g -- Green channel
	* @param b -- Blue channel
	* @param alpha -- Optional, opacity value
	*/
	static
		Reference<Material> createMaterial(const float& r, const float& g, const float& b, const float& alpha = 1)
	{
		return Material::makeUse(new Material(Color{ r, g, b, alpha }));
	}

	Color shoot(ray3f& ray);

public:
	Raycaster() = delete;
	/*
	* @brief Raycaster constructor
	*
	* @param width -- Image's width
	*/
	explicit Raycaster(int width, float aspectRatio, std::ostream& out) :
		_m(width), _n((int)(width / aspectRatio)),
		_scene(Scene::makeUse(Scene::makeUse(new Scene()))),
		_camera(Camera::makeUse(new Camera(aspectRatio))),
		_out(&out),
		aspectRatio(aspectRatio)
	{
		_camera->setDefaultView(aspectRatio);
		_camera->setDirectionOfProjection(vec3f::null() - _camera->position());
		//_camera->setClippingPlanes(300.0f, 600.0f);

		_scene->backgroundColor = Color{ 0.678f, 0.848f, 0.90f }; //Light Blue
		_scene->ambientLight = Color{ 0.412f, 0.412f, 0.412f }; // Light Gray

	}

	void writeHeader()
	{
		*_out << "P3\n" << _m << ' ' << _n << "\n255\n";
	}

	void writeColor(const cg::Color& c)
	{
		int iR = (int)(255.999 * c.x);
		int iG = (int)(255.999 * c.y);
		int iB = (int)(255.999 * c.z);

		*_out << iR << ' ' << iG << ' ' << iB << '\n';
	}

	auto& scene() { return _scene; }

	auto& camera() { return _camera; }

	void createAxis(Material*, Material*, Material*, Material*, bool);

	/**
	* @brief Creates a light.
	*
	* @param position -- Light's position
	* @param color -- Light's color
	*/
	void createLight(const vec3f& position, const Color& color);

	/**
	* @brief Creates a Sphere Actor; No rotation required.
	*
	* @param center -- Center coordinates
	* @param radius -- Sphere radius
	* @param scale -- vec3f scale in x, y and z axis
	* @param material -- Material to be used
	*/
	void createSphereActor(const vec3f& center,
		const float& radius,
		Material* material,
		const vec3f& scale = { 1.0f, 1.0f, 1.0f });

	/**
	* @brief Creates a Square plane Actor;
	*
	* @param P -- Point which plane passes through
	* @param angles -- Euler angles in radians in z, x and y order
	* @param scale -- Square's x and y scale;
	*/
	void createPlaneActor(const vec3f& P,
		const vec3f& angles,
		Material* material,
		const vec2f& scale = { 1.0f, 1.0f });


	void render();

};





#endif // __Raycaster_h