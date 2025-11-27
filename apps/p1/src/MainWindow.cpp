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
// OVERVIEW: MainWindow.cpp
// ========
// Source file for cg template window.
//
// Author: Paulo Pagliosa
// Last revision: 07/11/2022

#include "../include/MainWindow.h"
#include "../include/Raycaster.h"

/////////////////////////////////////////////////////////////////////
//
// MainWindow implementation
// ==========
MainWindow::MainWindow(int width, int height) :
	Base{ "P2/T1 CG", width, height }
{
	// Put your code here. Example:
	_lineColor = cg::Color::red;
	_meshColor = cg::Color::blue;
	_radius = 1;
	_speed = 0.01f;
}

void MainWindow::initializeMaterialList()
{
	Reference<Material> materials[5];
	materials[0] = Material::makeUse(new Material{Color(0.54f, 0.31f, 0.15f)});
	materials[1] = Material::makeUse(new Material{Color(0.83f, 0.84f, 0.86f)});
	materials[2] = Material::makeUse(new Material{Color(0.75f, 0.75f, 0.75f)});
	materials[3] = Material::makeUse(new Material{Color(0.53f, 0.52f, 0.50f)});
	materials[4] = Material::makeUse(new Material{Color(1.00f, 0.84f, 0.00f)});

    materials[0]->setName("Copper");
    materials[1]->setName("Aluminum");
    materials[2]->setName("Silver");
    materials[3]->setName("Titanium");
    materials[4]->setName("Gold");
	
	for (int i = 0; i < 5; i++)
		materialList.add(materials[i]);
}

void
MainWindow::initialize()
{
	// Put your OpenGL initialization code here. Example:
	Base::initialize();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);

	_scene =  Scene::makeUse(new Scene());
	_scene->backgroundColor = _backgroundColor;
	_scene->ambientLight = Color::gray * 0.5f;

	_renderer = MyRenderer::makeUse(new MyRenderer());
	_renderer->setCamera(camera());
	_renderer->begin();

	rc = Raycaster(width(), camera()->aspectRatio());
	rc._scene = _scene;
	rc._camera = camera();

	createSphere(vec3f::null(), 1.0f);
	_currentActor = *_scene->actors.begin();
} 

void
MainWindow::update()
{
	// Put your scene update code here. Example:
	static float time{};

	if (_animate)
		_radius = 1 + cosf(_speed * (time += deltaTime()) * 0.5f);
}

void
MainWindow::renderScene()
{
	// Put your scene rendering code here. Example:
	using namespace cg;

	auto g3 = this->g3();
	if (_openGLMode)
	{
		_renderer->setLights(_scene->lights.begin(), _scene->lights.end());

		for (auto actor : _scene->actors)
		{
			_renderer->setMaterial(*actor->material(), actor);

			auto& trs = actor->shape()->localToWorldMatrix();
			mat3f n(trs);
			_renderer->render(*actor->mesh(),
				trs,
				n);
		}
		if (_showGround)
			g3->drawXZPlane(8, 1);
	}
	else
	{
		rc.image()->draw(0, 0);
		ImGui::SetNextWindowSize({ 180, 80 });
		ImGui::Begin("OpenGL Mode");
		if (ImGui::Button("Go back to OpenGL mode"))
			_openGLMode = true;
		ImGui::End();
	}
}

bool
MainWindow::keyInputEvent(int key, int action, int mods)
{
	// Put your key event handler code here. Example:
	if (action != GLFW_RELEASE && mods == GLFW_MOD_ALT)
		switch (key)
		{
		case GLFW_KEY_P:
			_animate ^= true;
			return true;
		case GLFW_KEY_U:
			createSphere(camera()->position(), 1.0f);
			std::cout << "aa";
			return true;
		case GLFW_KEY_S:
			std::cout << "Shaders reloaded\n";
			//_renderer->updateShaders();
		}

	if (ImGui::GetIO().WantCaptureKeyboard || action == GLFW_RELEASE)
		return false;

	const auto delta = camera()->distance() * 0.05f;
	auto d = vec3f::null();

	switch (key)
	{
	case GLFW_KEY_W:
		d.z -= delta;
		break;
	case GLFW_KEY_S:
		d.z += delta;
		break;
	case GLFW_KEY_A:
		d.x -= delta;
		break;
	case GLFW_KEY_D:
		d.x += delta;
		break;
	case GLFW_KEY_Q:
		d.y += delta;
		break;
	case GLFW_KEY_Z:
		d.y -= delta;
		break;
	default:
		return false;
	}
	camera()->translate(d*_cameraSpeed);
	return true;
}

bool
MainWindow::mouseButtonInputEvent(int button, int action, int mods) 
{
	if (ImGui::GetIO().WantCaptureMouse)
		return false;

	auto active = action == GLFW_PRESS;

	if (action != GLFW_RELEASE && mods == GLFW_MOD_ALT)
	{
		int x, y;
		cursorPosition(x, y);
		auto mouseRay = rc.makeRay(x, y);

		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			createSphere(mouseRay(_objectCreationDistance), 1);
			return true;
		case GLFW_MOUSE_BUTTON_RIGHT:
			createLight(mouseRay(_objectCreationDistance));
			return true;
		default:
			return false;
		}
	}
	return Base::mouseButtonInputEvent(button, action, mods);
}

bool
MainWindow::onMouseLeftPress(int i, int j)
{
	auto mouseRay = rc.makeRay(i, j);
	IntersectionInfo inter;
	if (rc.shoot(mouseRay, inter))
	{
		std::cout << "acertouj" << '\n';
		_currentActor = inter.actor;
		updateActorGUI();
		return true;
	}
	std::cout << " nn acertouj" << '\n';
	return false;
}
void
MainWindow::gui()
{
	// Put your gui code here. Example:
	ImGui::SetNextWindowSize({ 420, 240 });
	ImGui::Begin("P2/T1 GUI");
	{

		ImGui::ColorEdit3("Line Color", (float*)&_lineColor);
		ImGui::ColorEdit3("Mesh Color", (float*)&_meshColor);
		if (ImGui::ColorEdit3("Ambient Light", (float*)&_ambientLight))
		{
			_scene->ambientLight = _ambientLight;
			_renderer->setAmbientLight(_ambientLight);
		}
		if (ImGui::ColorEdit3("Background Color", (float*)&_backgroundColor))
			_scene->backgroundColor = _backgroundColor;
		ImGui::Separator();
		ImGui::Checkbox("Animate", &_animate);
		ImGui::SliderFloat("Speed", &_speed, 0.001f, 0.01f);
		ImGui::SliderFloat("Camera speed", &_cameraSpeed, 1.0f, 10.0f);
		ImGui::SliderFloat("Object Creation distance", &_objectCreationDistance, 1.0f, 50.0f);
		ImGui::Checkbox("Show Ground", &_showGround);
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			deltaTime(),
			ImGui::GetIO().Framerate);
	}
	ImGui::End();

	
	ImGui::SetNextWindowSize({ 360, 180 });
	ImGui::Begin("Object Inspector");
	{
		if (ImGui::InputFloat("Radius", &actorProps.objectRadius, 0.1f, 10.0f))
			updateActorShape();
		if (ImGui::SliderFloat3("Position", &actorProps.objectPosition.x, -20.0f, 20.0f))
			updateActorShape();
		if (ImGui::SliderFloat3("Scale", &actorProps.objectScale.x, -20.0f, 20.0f))
			updateActorShape();
		if (ImGui::Button("Delete"))
			removeActor();
		if (ImGui::BeginMenu("Mesh"))
		{
			if (ImGui::MenuItem("Sphere"))
				_currentActor->mesh() = GLGraphics3::sphere();
			if (ImGui::MenuItem("Box"))
				_currentActor->mesh() = GLGraphics3::box();
			ImGui::EndMenu();
		}
	}
	ImGui::End();

	ImGui::SetNextWindowSize({ 360, 240 });
	ImGui::Begin("Object Material");
	{
		ImGui::ColorEdit3("Ambient", (float*)&_currentActor->material()->ambient);
		ImGui::ColorEdit3("Diffuse", (float*)&_currentActor->material()->diffuse);
		ImGui::ColorEdit3("Spot", (float*)&_currentActor->material()->spot);
		ImGui::InputFloat("Shine", &_currentActor->material()->shine);
		ImGui::ColorEdit3("Specular", (float*)&_currentActor->material()->specular);
		ImGui::ColorEdit3("Transparency", (float*)&_currentActor->material()->transparency);
		ImGui::InputFloat("Index of Refraction", &_currentActor->material()->ior);
		ImGui::SliderFloat("Rugosity", &_currentActor->rugosity, 0.0f, 1.0f);
		ImGui::SliderFloat("Metal Factor", &_currentActor->metalFactor, 0.0f, 1.0f);
	}
	ImGui::End();

	ImGui::SetNextWindowSize({ 360,240 });
	const char* previewName = (_currentLight == nullptr) ? "Select Light" : _currentLight->name();
	ImGui::SetNextWindowSize({ 360,360 });
	ImGui::Begin("Light Inspector");
	{
		if(ImGui::BeginCombo("Scene Lights", previewName))
		{
			for (Reference<Light> light : _scene->lights)
			{
				bool isSelected = (_currentLight == light);
				ImGui::PushID(light);
				if (ImGui::Selectable(light->name(), isSelected))
				{
					_currentLight = light;
					_currentLightPosition = light->position();
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
				ImGui::PopID();
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();

		if (_currentLight != nullptr)
		{
			ImGui::Text(_currentLight->name());
			ImGui::ColorEdit3("Color", (float*)&_currentLight->color);
			if (ImGui::SliderFloat3("Position", &_currentLightPosition.x, -20.0f, 20.0f))
				_currentLight->setPosition(_currentLightPosition);
			
			ImGui::Separator();
			ImGui::Text("FallOff");
			if (ImGui::BeginMenu(falloffToString(_currentLight->falloff)))
			{
				if (ImGui::MenuItem("Constant"))
					_currentLight->falloff = Light::Falloff::Constant;
				if (ImGui::MenuItem("Linear"))
					_currentLight->falloff = Light::Falloff::Linear;
				if (ImGui::MenuItem("Quadratic"))
					_currentLight->falloff = Light::Falloff::Quadratic;
				ImGui::EndMenu();
			}
		}
	}
	if (ImGui::Button("Delete"))
		_scene->lights.remove(_currentLight);
	ImGui::End();

	ImGui::SetNextWindowSize({ 80,80 });
	ImGui::Begin("Raycast");
	if (ImGui::Button("Render"))
	{
		rc.render();
		_openGLMode = false;
	}
	ImGui::End();
}

void MainWindow::updateActorGUI()
{
	auto& trs = _currentActor->shape()->localToWorldMatrix();
	actorProps.objectPosition = vec3f(trs[3]);
	auto scaleX = trs[0].length();
	auto scaleY = trs[1].length();
	auto scaleZ = trs[2].length();
	actorProps.objectScale = vec3f{ scaleX, scaleY, scaleZ };

	actorProps.color = _currentActor->material()->ambient * 5.0f;

}

void MainWindow::updateActorShape()
{
	std::cout << "atualizei kkkk" << '\n';
	_currentActor->shape()->setTransform(actorProps.objectPosition, quatf::identity(), vec3f{ 1.0f, 1.0f, 1.0f } * actorProps.objectRadius);
	updateActorGUI();
}

inline void MainWindow::removeActor()
{
	_scene->actors.remove(_currentActor);
}

