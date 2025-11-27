#include "../include/Raycaster.h"

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
	Reference<Plane> plane = Plane::makeUse(new Plane());
	plane->setTransform(P, quatf::eulerAngles(angles), { scale.x, 1.0f, scale.y });
	return plane;
}

/**
* @brief Creates and insert an actor into the scene
*
* @param shape -- Actor3's shape
* @param material -- Actor3's material
*/
void Raycaster::createActor(Shape3* shape, Material* material)
{
	//Reference<Actor3> actor = Actor3::makeUse(new Actor3(*shape));
	//actor->setMaterial(*material);
	//_scene->actors.add(actor);
}


/**
* @brief Creates x y and z axes for better spacial orientation
*
* @param Material1 -- X axis's material
* @param Material2 -- Y axis's material
* @param Material3 -- Z axis's material
* @param Material4 -- Point's material
* @param flag -- True for generating points, increasing actors count by 30; False by default
*/
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
		for (int i = 0; i < 10; i++)
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
* @brief Creates a Sphere Actor3; No rotation required.
*
* @param center -- Center coordinates
* @param radius -- Sphere radius
* @param scale -- vec3f scale in x, y and z axis; 1 by default
* @param material -- Material to be used
*/
void Raycaster::createSphereActor(const vec3f& center,
	const float& radius,
	Material* material,
	const vec3f& scale)
{
	createActor(createSphere(center, radius, scale), material);
}

/**
* @brief Creates a Square plane Actor3;
*
* @param P -- Point which plane passes through
* @param angles -- Euler angles in radians in z, x and y order
* @param scale -- Square's x and y scale;
*/
void Raycaster::createPlaneActor(const vec3f& P,
	const vec3f& angles,
	Material* material,
	const vec2f& scale)
{
	createActor(createPlane(P, angles, scale), material);
}

void Raycaster::render()
{
	Color rayColor;

	/*
	_bvh = nullptr;

	std::vector<Reference<Shape3>> shapeVector;
	for (auto actor : _scene->actors)
	{

		assert(actor != nullptr);
		shapeVector.push_back(actor->shape());
	}
	// _bvh = BVH<Shape3>::makeUse(new BVH<Shape3>{ std::move(shapeVector) });
	_bvh = new BVH<Shape3>{ std::move(shapeVector) };
	*/

	for (int j = 0; j < _n; ++j)
	{
		std::clog << "\rScanlines remaining: " << (_n - j) << ' ' << std::flush;
		for (int i = 0; i < _m; i++)
		{
			auto pixelRay = makeRay(i, j);
			rayColor = shade(pixelRay);
			_imageBuffer(i, -j + _n - 1) = rayColor;
		}
		_image->setData(_imageBuffer);
	}
	_image->draw(0, 0);
}



Color Raycaster::shade(ray3f& pixelRay)
{
	using namespace cg::math;

	Color c = _scene->backgroundColor;
	
	Intersection inter;

	if (shoot(pixelRay, inter))
	{
		Actor3* actor = (Actor3*)inter.object;
		auto m = actor->material();
		c = m->ambient * _scene->ambientLight;
		vec3f interPoint = pixelRay(inter.distance);
		vec3f shapeNormal = actor->shape()->normalAt(interPoint);
		Color interpolatedDiffuse = (m->diffuse + (Color::black - m->diffuse) * actor->metalFactor);
		Color diffuseBRDF =  interpolatedDiffuse * math::inverse(math::pi<float>);

		for (auto light : _scene->lights)
		{
			bool isOccluded = false;

			vec3f lightDirection = light->position() - interPoint;
			float lightDistance = lightDirection.length();
			lightDirection *= inverse(lightDistance);
			ray3f lightRay{ interPoint + (shapeNormal * 1e-3f), lightDirection };


			for (auto shadowActor : _scene->actors)
			{
				float shadowInterPoint;
				if (shadowActor->shape()->intersect(lightRay, shadowInterPoint) &&
					math::isNegative(shadowInterPoint - lightDistance))
				{
					isOccluded = true;
					break;
				}
			}

			if (!isOccluded)
			{ 

				Color lightColor = light->lightColor(lightDistance);
				vec3f halfWay = (lightDirection - pixelRay.direction).versor();

				Color interpolatedSpecular = Color(0.04f, 0.04f, 0.04f) * (1 - actor->metalFactor) + m->specular * actor->metalFactor;
				Color fresnel = interpolatedSpecular + (Color::white - interpolatedSpecular) * powf(1.0f - max(lightDirection.dot(halfWay), 0.01f), 5);

				float nDotL =  max(shapeNormal.dot(lightDirection), 0.01f);
				float nDotV = max(-shapeNormal.dot(pixelRay.direction), 0.01f);
				float k = sqr(actor->rugosity + 1) / 8; // compilador vai otimizar pois é divisão por potencia de 2
				float g1 = nDotL / ( (nDotL * (1 - k)) + k);
				float g2 = nDotV / ( (nDotV * (1 - k)) + k);
				float microfacetNDF = powf(actor->rugosity, 2) / ( pi<float> * sqr(sqr( max(shapeNormal.dot(halfWay), 0.1f)) * ( powf(actor->rugosity, 4) - 1) + 1));

				Color specularBRDF = fresnel * (g1 * g2 * microfacetNDF / (4 * nDotL * nDotV));

				c += lightColor * (diffuseBRDF + specularBRDF) * nDotL;
			}
			c *= pi<float>;
		}
		
	}
	if (c.r > 1.0f)
		c.r = 1.0f;
	if (c.g > 1.0f)
		c.g = 1.0f;
	if (c.b > 1.0f)
		c.b = 1.0f;
	return c;
}

ray3f Raycaster::makeRay(int i, int j)
{
	float H = _camera->windowHeight();
	float W = H * aspectRatio;

	const auto& m = _camera->cameraToWorldMatrix();

	vec3f camU, camV, camN;
	camU = m[0];
	camV = m[1];
	camN = m[2];

	float Xp = (((W * (i + 0.5f)) / (float)_m)) - (W * 0.5f);
	float Yp = (H * 0.5f) - ((H / (float)_n) * (j + 0.5f));
	float Zp = _camera->nearPlane();

	vec3f p = (Xp * camU + Yp * camV - Zp * camN).versor();

	return ray3f{ _camera->position(), p };
}

bool Raycaster::shoot(ray3f ray, Intersection& inter)
{
	if (_scene->actors.empty())
	{
		std::cout << "lista vazia tá vazia" << '\n';
		return false;
	}
	float minDistance = std::numeric_limits<float>::max();
	Reference<Actor3> closestActor = *(_scene->actors.begin());
	for (auto actor : _scene->actors)
	{
		float t;

		if (actor->shape()->intersect(ray, t) && t < minDistance)
		{
			minDistance = t;
			closestActor = actor;
		}
	}
	
	if (minDistance != std::numeric_limits<float>::max())
	{
		inter.object = closestActor;
		inter.distance = minDistance;
		return true;
	}

	return false;
}






