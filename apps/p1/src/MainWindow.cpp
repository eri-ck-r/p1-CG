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
	Base{ "Ds template", width, height }
{

	// Put your code here. Example:
	_lineColor = cg::Color::red;
	_meshColor = cg::Color::blue;
	_radius = 1;
	_speed = 0.01f;
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

	rc = Raycaster(width(), camera()->aspectRatio());
	rc._scene = _scene;
	rc._camera = camera();
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

	g3->setLineColor(_lineColor);
	g3->drawArc({ -4, 0, 0 }, // center
		_radius, // radius
		{ 1, 0, 0 }, // first point direction
		{ 0, 0, 1 }, // normal
		180); // angle
	g3->setPolygonMode(GLGraphics3::LINE);
	g3->drawCircle({ 0, 0, 0 }, // center
		_radius, // radius
		{ 0, 0, 1 }); // normal
	g3->setPolygonMode(GLGraphics3::FILL);
	g3->setMeshColor(_meshColor);
	/* aqui vai ter um for que vai percorrer todos os atores da cena
	 entao as classes dos atores vão ter que ter a mesh deles tambem
	 e fornecer a trs é claro
	 agora a parte dos materiais eu já não sei
	 como q eu seleciono essa bosta de material porra
	 entao obviamente o raycaster tem que conhecer sua mainwindow e
	 vice versa

	 entao acho que nesse drawMesh eu passo a trs e a trasnfrom vector
	 */
	for (auto actor : _scene->actors)
	{
		auto& trs = actor->shape()->localToWorldMatrix();
		mat3f n(trs);
		g3->drawMesh(*actor->mesh(),
			trs,
			n);
	}
	g3->drawMesh(*g3->sphere(), // mesh
		{ 4, 0, 0 }, // position
		mat3f::identity(), // rotation
		vec3f{ 1, 2, 1 } *_radius); // scale
	if (_showGround)
		g3->drawXZPlane(8, 1);
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
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			int x, y;
			cursorPosition(x, y);
			auto mouseRay = rc.makeRay(x, y);
			createSphere(mouseRay(_objectCreationDistance), 1);

			return true;
		}
	}
	return Base::mouseButtonInputEvent(button, action, mods);
}

bool
MainWindow::onMouseLeftPress(int i, int j)
{
	auto ray = rc.makeRay(i, j);
	Intersection inter;
	if (rc.shoot(ray, inter))
	{
		std::cout << "acertouj" << '\n';
		currentSelectedActor = inter.actor;
		//TODO tem que ter um update object aqui pra poder mudar tudo pra aparecer na GUI
		// e ela nao resetar o objeto inteiro toda hora que ela aparece
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
	ImGui::Begin("Template GUI");
	ImGui::ColorEdit3("Line Color", (float*)&_lineColor);
	ImGui::ColorEdit3("Mesh Color", (float*)&_meshColor);
	ImGui::Separator();
	ImGui::Checkbox("Animate", &_animate);
	ImGui::SliderFloat("Speed", &_speed, 0.001f, 0.01f);
	ImGui::SliderFloat("Camera speed", &_cameraSpeed, 1.0f, 10.0f);
	ImGui::SliderFloat("Object Creation distace", &_objectCreationDistance, 1.0f, 50.0f);
	ImGui::Checkbox("Show Ground", &_showGround);
	ImGui::Separator();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		deltaTime(),
		ImGui::GetIO().Framerate);
	ImGui::End();

	
	ImGui::SetNextWindowSize({ 360, 180 });
	ImGui::Begin("Object Inspector");
	if (ImGui::InputFloat("Radius", &_objectRadius, 0.1f, 10.0f))
		updateActor();
	if (ImGui::SliderFloat3("Position", &_objectPosition.x, -20.0f, 20.0f))
		updateActor();
	if (ImGui::SliderFloat3("Scale", &_objectScale.x, -20.0f, 20.0f))
		updateActor();
	if (ImGui::Button("Modify"))
		updateActor();
	if (ImGui::Button("Delete"))
		removeActor();
	ImGui::End();

}

void MainWindow::updateActorGUI()
{
	auto& trs = currentSelectedActor->shape()->localToWorldMatrix();
	_objectPosition = vec3f(trs[3]);
	auto scaleX = trs[0].length();
	auto scaleY = trs[1].length();
	auto scaleZ = trs[2].length();
	_objectScale = vec3f{ scaleX, scaleY, scaleZ };
}

void MainWindow::updateActor()
{
	std::cout << "atualizei kkkk" << '\n';
	currentSelectedActor->shape()->setTransform(_objectPosition, quatf::identity(), vec3f{ 1.0f, 1.0f, 1.0f } * _objectRadius);
	updateActorGUI();
}

inline void MainWindow::removeActor()
{
	_scene->actors.remove(currentSelectedActor);
}
