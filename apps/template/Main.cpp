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

struct Settings
{
    int m = 1024;
    int n = 768;
    int W = m;
    int H = n;
};

void writeHeader(std::ostream& out, Settings& settings)
{
    out << "P3\n" << settings.m << ' ' << settings.n << "\n255\n";
}

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

    std::ofstream of{ "image.ppm" };

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
    Settings settings;

    Reference<Material> newMaterial = Material::makeUse(new Material(Color{ 1.0f,0.0f,0.0f }));
    Reference<Material> blueMaterial = Material::makeUse(new Material(Color{ 0.0f,0.0f,1.0f }));

    Reference<Sphere> sphere = Sphere::makeUse(new Sphere({ 0.0f, 0.0f, 0.0f }, 8.0f));

    Reference<Plane> plane = Plane::makeUse(new Plane({ 0,0,0 }, { 0, 1, 0 }));
    Reference<Plane> plane2 = Plane::makeUse(new Plane{ {-2, 0, 0}, {1, 0, 0} });
    Reference<Plane> plane3 = Plane::makeUse(new Plane{ {0, 0, -2}, {0, 0, 1} });

    Reference<Actor> sphereActor = Actor::makeUse(new Actor{ *sphere });
    sphereActor->setTransform(mat4f::identity());
    sphereActor->setMaterial(*blueMaterial);

    Reference<Actor> planeActor = Actor::makeUse(new Actor{ *plane });
    planeActor->setTransform(mat4f::identity());

    Reference<Actor> planeActor2 = Actor::makeUse(new Actor{ *plane2 });
    planeActor2->setTransform(mat4f::identity());
    planeActor2->setMaterial(*newMaterial);

    Reference<Actor> planeActor3 = Actor::makeUse(new Actor{ *plane3 });
    planeActor3->setTransform(mat4f::identity());
    planeActor3->setMaterial(*blueMaterial);

    scene->backgroundColor = Color{ 0.678f, 0.848f, 0.90f };
    scene->ambientLight = Color{ 0.412f, 0.412f, 0.412f };

    scene->actors.add(sphereActor);
    scene->actors.add(planeActor);
    // scene->actors.add(planeActor2);

    camera->setDefaultView();
    camera->setPosition({ 0, 1, 10 });
    // camera->setDirectionOfProjection((vec3f{ 0,0,0 } - camera->position()).versor());
    // camera->pitch(30);
    camera->print();
    // GLImage image{ settings.m, settings.n };

    const auto& m = camera->cameraToWorldMatrix();

    vec3f camU, camV, camN;
    camU = m[0];
    camV = m[1];
    camN = m[2];

    writeHeader(of, settings);

    ray3f ray;

    /*
    float F, B;
    camera->clippingPlanes(F, B);
    auto z = B / F * 0.5f;
    B = vec3f{ settings.W * z, settings.H * z, B }.length();
    ray.tMin = F;
    ray.tMax = B;
    */

    ray.set(camera->position(), -camN);

    for (int j = 0; j < settings.n; ++j)
    {
        // std::clog << "\rScanlines remaining: " << (settings.n - j) << ' ' << std::flush;
        for (int i = 0; i < settings.m; i++)
        {
            //determinar o Xp e o Yp
            float Xp = (((settings.W * (i + 0.5f)) / (float)settings.m)) - (settings.W / 2.0f);
            float Yp = (settings.H / 2.0f) - ((settings.H / (float)settings.n) * (j + 0.5f));
            float Zp = camera->nearPlane() + 100.0f;

            vec3f p = (Xp * camU + Yp * camV - Zp * camN).versor();
            ray.direction = p;

            float minDistance = std::numeric_limits<float>::max();
            Color c = scene->backgroundColor;

            if (ray.direction.equals({ 0, 0, -1 }, 1e-2))
            {
                vec3f zero{};
            }

            for (auto actor : scene->actors)
            {
                float t = std::numeric_limits<float>::max();
                if (actor->shape()->intersect(ray, t))
                {
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
            // writeColor(std::cout, c);
            writeColor(of, c);
        }
        std::clog.precision(5);
        std::clog << "\nd: " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z;
    }

    of.close();

    std::cin.get();

    return 1;
}
