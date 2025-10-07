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
    puts("Ds template by Paulo Pagliosa (ppagliosa@gmail.com)\n");
    puts("Camera controls keys:\n"
        "(w) move forward  (s) move backward\n"
        "(a) move left     (d) move right\n"
        "(q) move up       (z) move down\n");
    puts("Mouse controls:\n"
        "(scroll wheel)    zoom\n"
        "(middle-click)    pan\n"
        "(Alt+right-click) rotate");

    using namespace cg;

    Reference<Scene> scene;
    Reference<Camera> camera;
    struct Settings
    {
        int m = 1024;
        int n = 768;
    } settings;

    camera->setPosition({0, 0, -1});
    GLImage image{settings.m, settings.n};

    for (int j = 0; j < settings.n; ++j)
    {
        for (int i = 0; i < settings.m; i++)
        {
            vec3f p = {0,0,0};
            ray3f ray{camera->position(), p};
	        float minDistance = std::numeric_limits<float>::max();
            Color c = scene->backgroundColor;

            for (auto actor : scene->actors)
            {
                float t;

                if(actor->shape()->intersect(ray, t))
                {
                    if (t < minDistance)
                    {
                        minDistance = t;
                        c = actor->material()->diffuse;
                    }
                }
                
            }

        }
    }
  
    return cg::Application{new MainWindow{1280, 720}}.run(argc, argv);
}
