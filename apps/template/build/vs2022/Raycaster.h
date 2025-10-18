#ifndef __Raycaster_h
#define __Raycaster_h

#include "graphics/Application.h"
#include "graphics/GLImage.h"
#include "graphics/Camera.h"

#include <cmath>
#include <fstream>

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
	std::ofstream _of;
	int _m;
	int _n;
	int _W;
	int _H;

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

	void generateMaterials()
	{

	}

public:
	Raycaster(int width, float aspectRatio, std::string imageName) :
		_m(width), _n((int)(width / aspectRatio)),
		_scene(Scene::makeUse(Scene::makeUse(new Scene()))),
		_camera(Camera::makeUse(new Camera(aspectRatio))),
		_of(imageName)

	{
		_camera->setDefaultView(aspectRatio);
		_camera->setDirectionOfProjection(vec3f::null() - _camera->position());
		_camera->setClippingPlanes(300.0f, 600.0f);

		_scene->backgroundColor = Color{ 0.678f, 0.848f, 0.90f }; //Light Blue
		_scene->ambientLight = Color{ 0.412f, 0.412f, 0.412f }; // Light Gray

		_H = _camera->windowHeight();
		_W = _H * aspectRatio;
	}

	auto& scene() { return _scene; }

	auto& camera() { return _camera; }

	void createAxis(bool);

	void createLight(const vec3f& position, const Color& color);

	void createSphereActor(const vec3f& center,
		const float& radius,
		const vec3f& scale,
		Material* material);

	void createPlaneActor(const vec3f&, const vec3f&, const vec2f&, Material* material);

	void render();

};





#endif // __Raycaster_h