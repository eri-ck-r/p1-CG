#include "../include/MyRenderer.h"

namespace cg
{ // begin namespace cg

#define STRINGIFY(A) "#version 400\n"#A


/////////////////////////////////////////////////////////////////////
//
// MyRenderer implementation
// ==============

	inline auto lightPosition(const Light& light, const Camera& camera)
	{
		if (light.flags.isSet(Light::LightBits::Camera))
			return light.position();
		return camera.worldToCameraMatrix().transform3x4(light.position());
	}

	void MyRenderer::setMaterial(const Material& material, void* texture)
	{
		_program.renderMaterial(material);
	}

	inline void MyRenderer::GLProgram::initUniformLightLocations(int i)
	{
		lightLocs[i].type = uniformLightLocation(i, "type");
		lightLocs[i].color = uniformLightLocation(i, "color");
		lightLocs[i].position = uniformLightLocation(i, "position");
		lightLocs[i].direction = uniformLightLocation(i, "direction");
		lightLocs[i].falloff = uniformLightLocation(i, "falloff");
		lightLocs[i].range = uniformLightLocation(i, "range");
		lightLocs[i].angle = uniformLightLocation(i, "angle");
	}

	inline void MyRenderer::GLProgram::initUniformLocations()
	{
		mvMatrixLoc = uniformLocation("mvMatrix");
		normalMatrixLoc = uniformLocation("normalMatrix");
		mvpMatrixLoc = uniformLocation("mvpMatrix");
		projectionTypeLoc = uniformLocation("projectionType");
		ambientLightLoc = uniformLocation("ambientLight");
		lightCountLoc = uniformLocation("lightCount");
		for (auto i = 0; i < maxLights; ++i)
			initUniformLightLocations(i);
		OaLoc = uniformLocation("material.Oa");
		OdLoc = uniformLocation("material.Od");
		OsLoc = uniformLocation("material.Os");
		nsLoc = uniformLocation("material.shine");
	}


	inline void MyRenderer::GLProgram::initProgram()
	{
		loadShaders("vertexShader.vert", "fragmentShader.frag").use();
		initUniformLocations();
	}

	MyRenderer::GLProgram::GLProgram() :
		GLSL::Program{ "Mesh Renderer" }
	{
		auto cp = GLSL::Program::current();

		initProgram();
		setUniformVec4(ambientLightLoc, Color::darkGray);
		renderMaterial(*Material::defaultMaterial());
		GLSL::Program::setCurrent(cp);
	}

	inline auto lightDirection(const Light& light, const Camera& camera)
	{
		if (light.flags.isSet(Light::LightBits::Camera))
			return light.direction();
		return camera.worldToCameraMatrix().transformVector(light.direction());
	}


	void MyRenderer::GLProgram::renderMaterial(const Material& material)
	{
		setUniformVec4(OaLoc, material.ambient);
		setUniformVec4(OdLoc, material.diffuse);
		setUniformVec4(OsLoc, material.spot);
		setUniform(nsLoc, material.shine);
	}

	void MyRenderer::GLProgram::renderLight(int i,
		const Light& light,
		const Camera& camera)
	{
		setUniform(lightLocs[i].type, (int)light.type());
		setUniformVec4(lightLocs[i].color, light.color);
		setUniformVec3(lightLocs[i].position, lightPosition(light, camera));
		setUniformVec3(lightLocs[i].direction, lightDirection(light, camera));
		setUniform(lightLocs[i].falloff, (int)light.falloff);
		setUniform(lightLocs[i].range, light.range());
		setUniform(lightLocs[i].angle, light.spotAngle());
	}

	inline void MyRenderer::GLProgram::renderDefaultLights()
	{
		setUniform(lightLocs[0].type, 1); // POINT
		setUniformVec4(lightLocs[0].color, vec4f{ 1, 1, 1, 0 });
		setUniformVec3(lightLocs[0].position, vec3f{ 0, 0, 0 });
		setUniform(lightLocs[0].range, 0.0f);
		setUniform(lightCountLoc, 1);
	}

	bool MyRenderer::setLight(int i, const Light& light)
	{
		return light.isTurnedOn() ?
			void(_program.renderLight(i, light, *camera())), true : false;
	}


	inline void MyRenderer::updateView()
	{
		camera()->update();

		GLint v[4];

		glGetIntegerv(GL_VIEWPORT, v);

		float w2 = v[2] / 2.0f;
		float h2 = v[3] / 2.0f;

		_viewportMatrix[0].set(w2, 0, 0, 0);
		_viewportMatrix[1].set(0, h2, 0, 0);
		_viewportMatrix[2].set(0, 0, 1, 0);
		_viewportMatrix[3].set(v[0] + w2, v[1] + h2, 0, 0);
	}

	void MyRenderer::begin()
	{
		if (auto cp = GLSL::Program::current(); &_program != cp)
		{
			_lastState.program = cp;
			_lastState.depthTest = glIsEnabled(GL_DEPTH_TEST);
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &_lastState.vao);
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &_lastState.texture);
			updateView();
			_program.use();
			_program.setUniformMat4(_program.viewportMatrixLoc, _viewportMatrix);
			glPolygonMode(GL_FRONT_AND_BACK, (renderMode != Wireframe) + GL_LINE);
			glEnable(GL_DEPTH_TEST);
		}
	}

	void MyRenderer::end()
	{
		if (auto cp = GLSL::Program::current(); &_program == cp)
		{
			GLSL::Program::setCurrent(_lastState.program);
			if (!_lastState.depthTest)
				glDisable(GL_DEPTH_TEST);
			glBindVertexArray(_lastState.vao);
			glBindTexture(GL_TEXTURE_2D, _lastState.texture);
		}
	}


	namespace
	{ // begin namespace

		inline mat4f
			mvMatrix(const mat4f& t, const Camera* c)
		{
			return c->worldToCameraMatrix() * t;
		}

		inline mat4f
			mvpMatrix(const mat4f& mvm, const Camera* c)
		{
			return c->projectionMatrix() * mvm;
		}

		inline auto
			normalMatrix(const mat3f& n, const Camera* c)
		{
			return mat3f{ c->worldToCameraMatrix() } *n;
		}

	} // end namespace

	void MyRenderer::render(TriangleMesh& mesh, const mat4f& t, const mat3f& n)
	{
		if (_lightCount == 0)
			_program.renderDefaultLights();

		auto camera = this->camera();
		auto mv = mvMatrix(t, camera);

		_program.setUniformMat4(_program.mvMatrixLoc, mv);
		_program.setUniformMat4(_program.mvpMatrixLoc, mvpMatrix(mv, camera));
		_program.setUniformMat3(_program.normalMatrixLoc, normalMatrix(n, camera));

		auto m = glMesh(&mesh);

		m->bind();
		glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);
	}

	void MyRenderer::render(TriangleMesh& mesh,
		const vec3f& p,
		const mat3f& r,
		const vec3f& s)
	{
		render(mesh, TRS(p, r, s), normalMatrix(r, s));
	}

	void MyRenderer::updateShaders()
	{
		_program.disuse();
		_program.loadShaders("vertexShader.vert", "fragmentShader.frag").use();
	}

} // end namespace cg
