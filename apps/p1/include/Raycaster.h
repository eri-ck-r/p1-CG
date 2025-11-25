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
#include "Intersection.h"
#include "graphics/GLImage.h"
using namespace cg;

class Raycaster
{
private:
	Reference<Scene> _scene;
	Reference<Camera> _camera;
	int _m;
	int _n;
	float aspectRatio;
	ImageBuffer _imageBuffer;
	Reference<GLImage> _image;

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

	Color shade(ray3f& ray);

public:
	Raycaster() = default;
	/*
	* @brief Raycaster constructor
	*
	* @param width -- Image's width
	* @param aspectRatio - Image's Aspect Ratio
	* @param out  -- ostream object for outputting the colors
	*/
	Raycaster(int width, float aspectRatio) :
		_m(width), _n((int)(width / aspectRatio)),
		aspectRatio(aspectRatio),
		_image(new GLImage{ _m, _n }),
		_imageBuffer(_m, _n)
	{
		// do nothing
	}

	auto& scene() { return _scene; }

	auto& camera() { return _camera; }

	auto& image() { return _image; }

	void createAxis(Material*, Material*, Material*, Material*, bool);

	/**
	* @brief Creates a light.
	*
	* @param position -- Light's position
	* @param color -- Light's color
	*/
	void createLight(const vec3f& position, const Color& color);

	/**
	* @brief Creates a Sphere Actor3; No rotation required.
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
	* @brief Creates a Square plane Actor3;
	*
	* @param P -- Point which plane passes through
	* @param angles -- Euler angles in radians in z, x and y order
	* @param scale -- Square's x and y scale;
	*/
	void createPlaneActor(const vec3f& P,
		const vec3f& angles,
		Material* material,
		const vec2f& scale = { 1.0f, 1.0f });

	/**
	* @brief creates a ray from i,j screen coordinates
	* 
	* @param i -- i screen coordinate
	* @param j -- j screen coordinate
	*/
	ray3f makeRay(int i, int j);

	bool shoot(ray3f ray, IntersectionInfo& inter);

	void render();

	friend class MainWindow;
};





#endif // __Raycaster_h