#include "Raycaster.h"


/**
* @brief Creates a Sphere; No rotation required.
*
* @param center -- Center coordinates
* @param radius -- Sphere radius
* @param scale -- vec3f scale in x, y and z axis
*/
Reference<Sphere> Raycaster::createSphere(const vec3f& center, const float radius, const vec3f& scale = { 1.0f, 1.0f, 1.0f })
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
Reference<Plane> Raycaster::createPlane(const vec3f& P, const vec3f& normal, const vec2f& scale = {1.0f, 1.0f})
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
* @param shape -- Actor's shape
* @param material -- Actor's material
*/
void Raycaster::createActor(Shape3* shape, Material* material)
{
	Reference<Actor> actor = Actor::makeUse(new Actor(*shape));
	actor->setMaterial(*material);
	_scene->actors.add(actor);
}

void Raycaster::createAxis(bool flag = false)
{
	auto xAxis = createSphere(vec3f::null(), 1.0f, { 1000.0f, 0.1f, 0.1f });
	auto yAxis = createSphere(vec3f::null(), 1.0f, { 0.1f, 1000.0f, 0.1f });
	auto zAxis = createSphere(vec3f::null(), 1.0f, { 0.1f, 0.1f, 1000.0f });

	//createActor(xAxis, Raycaster::Materials::redMaterial);
	//createActor(yAxis, yellowMaterial);
	//createActor(zAxis, blueMaterial);

	//if (flag)
	//{
	//	for (int i = 0; i < 20; i++)
	//	{
	//		auto xSphere = createSphere({ (float)i, 0.0f, 0.0f }, 0.12f, { 1.0f, 3.0f, 1.0f });
	//		createActor(xSphere, grayMaterial);
	//		auto ySphere = createSphere({ 0.0f, (float)i, 0.0f }, 0.12f, { 3.0f, 1.0f, 1.0f });
	//		createActor(ySphere, grayMaterial);
	//		auto zSphere = createSphere({ 0.0f, 0.0f, (float)i }, 0.12f, { 1.0f, 3.0f, 1.0f });
	//		createActor(zSphere, grayMaterial);
	//	}
	//}
}

/**
* @brief Creates a light.
* 
* @param position -- Light's position
* @param color -- Light's color
*/
void Raycaster::createLight(const vec3f& position, const Color& color)
{
	Reference<Light> light = Light::makeUse(new Light());
	light->setPosition(position);
	light->color = color;
	light->setType(Light::Type::Point);
	light->falloff = Light::Falloff::Constant;
	_scene->lights.add(light);
}

/**
* @brief Creates a Sphere Actor; No rotation required.
*
* @param center -- Center coordinates
* @param radius -- Sphere radius
* @param scale -- vec3f scale in x, y and z axis
* @param material -- Material to be used 
*/
inline
void Raycaster::createSphereActor(const vec3f& center, const float& radius, const vec3f& scale = {1.0f, 1.0f, 1.0f}, Material* material)
{
	createActor(createSphere(center, radius, scale), material);
}

/**
* @brief Creates a Square plane Actor;
* 
* @param P -- Point which plane passes through
* @param angles -- Euler angles in radians in z, x and y order
* @param scale -- Square's x and y scale;
*/
void Raycaster::createPlaneActor(const vec3f& P, const vec3f& angles, const vec2f& scale = {1.0f, 1.0f}, Material* material)
{

}
