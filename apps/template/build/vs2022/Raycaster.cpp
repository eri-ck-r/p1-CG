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
*  @param angles -- euler angles for z, x and y axis respectively
*  @param scale -- Vec2f scale, only in vertical and horizontal (x and y)
*/
Reference<Plane> Raycaster::createPlane(const vec3f& P, const vec3f& angles, const vec2f& scale = { 1.0f, 1.0f })
{
	Reference<Plane> plane = Plane::makeUse(new Plane(P, { 0.0f, 1.0f, 0.0f }));
	plane->setTransform(P, quatf::eulerAngles(angles), { scale.x, 1.0f, scale.y });
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

inline
Pixel Raycaster::colorToPixel(Color c)
{

	auto iR = (uint8_t)(255 * c.x);
	auto iG = (uint8_t)(255 * c.y);
	auto iB = (uint8_t)(255 * c.z);

	return Pixel( iR, iG, iB );
}

void Raycaster::createAxis(Material* material1,
Material* material2,
Material* material3,
Material* material4,
bool flag = false)
{
	auto xAxis = createSphere(vec3f::null(), 1.0f, { 1000.0f, 0.1f, 0.1f });
	auto yAxis = createSphere(vec3f::null(), 1.0f, { 0.1f, 1000.0f, 0.1f });
	auto zAxis = createSphere(vec3f::null(), 1.0f, { 0.1f, 0.1f, 1000.0f });

	createActor(xAxis, material1);
	createActor(yAxis, material2);
	createActor(zAxis, material3);

	if (flag)
	{
		for (int i = 0; i < 20; i++)
		{
			auto xSphere = createSphere({ (float)i, 0.0f, 0.0f }, 0.12f, { 1.0f, 3.0f, 1.0f });
			createActor(xSphere, material4);
			auto ySphere = createSphere({ 0.0f, (float)i, 0.0f }, 0.12f, { 3.0f, 1.0f, 1.0f });
			createActor(ySphere, material4);
			auto zSphere = createSphere({ 0.0f, 0.0f, (float)i }, 0.12f, { 1.0f, 3.0f, 1.0f });
			createActor(zSphere, material4);
		}
	}
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

void Raycaster::createSphereActor(const vec3f& center,
	const float& radius,
	Material* material,
	const vec3f& scale = { 1.0f, 1.0f, 1.0f })
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

void Raycaster::createPlaneActor(const vec3f& P,
	const vec3f& angles,
	Material* material,
	const vec2f& scale = { 1.0f, 1.0f })
{
	createActor(createPlane(P, angles, scale), material);
}

Color Raycaster::shoot(ray3f& pixelRay)
{
	float minDistance = std::numeric_limits<float>::max();
	Color c = _scene->backgroundColor;

	for (auto actor : _scene->actors)
	{
		float t;

		if (actor->shape()->intersect(pixelRay, t))
		{
			if (t < minDistance)
			{
				minDistance = t;
				c = actor->material()->ambient;

				// iluminaçao = Cd*Cl*(-N*Ll)
				// onde Cd = cor do material difuso, Cl = cor da luz(tem que calcular o falloff, N = normal
				// e Ll a direção do raio de luz ( é o lightray)
				vec3f interPoint = pixelRay(minDistance);
				bool flag = false;
				for (auto light : _scene->lights)
				{
					vec3f lightDirection = light->position() - interPoint;
					ray3f lightRay{ interPoint, lightDirection.versor()};
					Color lightColor = light->lightColor(lightDirection.length());

					float minDistance2 = std::numeric_limits<float>::max();
					for (auto shadowActor : _scene->actors)
					{
						float shadowInterPoint;
						if (shadowActor->shape()->intersect(lightRay, shadowInterPoint) && shadowActor != actor)
						{
							if(shadowInterPoint < minDistance2)
							{
								minDistance2 = shadowInterPoint;
								flag = true;
							}
						}
					}

					if (flag == false)
					{
						vec3f shapeNormal = actor->shape()->normalAt(interPoint);
						// I =  Od * Il * (N*Ll)
						c += actor->material()->diffuse * lightColor * (shapeNormal.dot(lightRay.direction));
						vec3f reflectionDirection = ((-lightDirection).versor() - 2.0f * (shapeNormal.dot((-lightDirection).versor()) * shapeNormal)).versor();
						c += actor->material()->spot * lightColor * pow(-(reflectionDirection.dot(pixelRay.direction)), 64);
					}
				}
			}
		}
	}
	return c;
}

void Raycaster::render()
{
	const auto& m = _camera->cameraToWorldMatrix();

	vec3f camU, camV, camN;
	camU = m[0];
	camV = m[1];
	camN = m[2];

	ray3f pixelRay{ _camera->position(), -camN };
	Color rayColor;

	float Zp = _camera->nearPlane();
	for (int j = 0; j < _n; ++j)
	{
		std::clog << "\rScanlines remaining: " << (_n - j) << ' ' << std::flush;
		float Yp = (_H / 2.0f) - ((_H / (float)_n) * (j + 0.5f));
		for (int i = 0; i < _m; i++)
		{
			float Xp = (((_W * (i + 0.5f)) / (float)_m)) - (_W / 2.0f);

			vec3f p = (Xp * camU + Yp * camV - Zp * camN).versor();
			pixelRay.direction = p;

			rayColor = shoot(pixelRay);

			// _bmp[i + j * _m] = colorToPixel(rayColor);
			writeColor(rayColor);
		}
	}

	// _bmp.save(_imageName);
}

