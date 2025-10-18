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
#include "Bitmap.h"

using namespace cg;

class Raycaster
{
private:
	Reference<Scene> _scene;
	Reference<Camera> _camera;
	int _m;
	int _n;
	float _W;
	float _H;

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

	static
	Pixel colorToPixel(Color c);

public:
	Raycaster(int width, float aspectRatio, std::ostream& out) :
		_m(width), _n((int)(width / aspectRatio)),
		_scene(Scene::makeUse(Scene::makeUse(new Scene()))),
		_camera(Camera::makeUse(new Camera(aspectRatio))),
		_out(&out)
	{
		_camera->setDefaultView(aspectRatio);
		_camera->setDirectionOfProjection(vec3f::null() - _camera->position());
		//_camera->setClippingPlanes(300.0f, 600.0f);

		_scene->backgroundColor = Color{ 0.678f, 0.848f, 0.90f }; //Light Blue
		_scene->ambientLight = Color{ 0.412f, 0.412f, 0.412f }; // Light Gray

		_H = _camera->windowHeight();
		_W = _H * aspectRatio;
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

	void createLight(const vec3f& position, const Color& color);

	void createSphereActor(const vec3f& center,
		const float& radius,
		Material* material,
		const vec3f& scale);

	void createPlaneActor(const vec3f& P,
		const vec3f& angles,
		Material* material,
		const vec2f& scale);

	Color shoot(ray3f& ray);

	void render();

};





#endif // __Raycaster_h