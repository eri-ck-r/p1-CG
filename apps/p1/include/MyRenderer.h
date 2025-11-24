#ifndef __MyRenderer_h
#define __MyRenderer_h

/*
* TODO: TENHO QUE BOTAR TUDO OS SHADERS AQUI.....
*
* TENHO QUE DAR OVERRIDE NO METODO DE DRAWMESH PRA EU PODER MUDAR
* O MATERIAL DOS OBJETOS... ISSO AQUI NAO E MUITO DIFICIL MAS
* VAI SER CHATO PQ NAO QUERO MEXER NO CODIGO ORIGINAL DELE
*
* TENHO QUE DAR UM JEITO DE MANDAR VÁRIAS LUZES PARA A CENA
* NO CASO TENHO QUE TER UMA LISTA DE LIGHTS E MANDAR ISSO PRA OPENGL
* DE ALGUM JEITO...
*
* TENHO QUE SEPARAR O CODIGO DA MAINWINDOW, O GUI ETC
*
* TENHO QUE MANDAR A POSIÇÃO DA CAMERA TAMBEM PRA PODER CALCULAR O PHONG
*
* AÍ AMANHA EU PERGUNTO PRO POVO DA SALA COMO QUE TA O BGL E PAH
*
* DE RESTO EU NAO VOU TER MUITOS PROBLEMAS PARA IMPLEMENTAR NO C++
* NA REAL VAI FICAR BEM MAIS FACIL DE TESTAR NE KK
*
* AH É TENHO QUE VER COMO USA GLIMAGE... PARECE TRANQUILO
*
* ah e tambem adicionar um selecionador de cor pra cena de fundo
* tenho que ver como q o openGL funciona pra usar cor de fundo
* sepa é só um glBackground() kkkk sla
*
* vou ver como usa CMake tambem... nao quero ser refém de windows
* nessa porra.
*
* quando for fazer o código c++ do raycaster bora segregar tudo em funçao.... ta feiao mane
*
*
*
*/


#include "graphics/CameraHolder.h"
#include "graphics/GLGraphics3.h"
#include "graphics/Light.h"
#include "graphics/Material.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// MyRenderer: OpenGL mesh renderer class
// ==============
    class MyRenderer : public CameraHolder
    {
    public:
        constexpr static auto maxLights = 8;

        enum class RenderMode
        {
            Wireframe = 1,
            HiddenLines = 2,
            Flat = 4,
            Smooth = 0
        };

        enum class RenderBits
        {
            UseLights = 1,
            UseVertexColors = 2
        };

        using enum RenderMode;
        using enum RenderBits;
        using RenderFlags = Flags<RenderBits>;

        RenderMode renderMode{ Smooth };
        RenderFlags flags{ UseLights };

        MyRenderer(Camera* camera = nullptr) :
            CameraHolder{ camera }
        {
            // do nothing
        }

        auto useVertexColors() const
        {
            return flags.isSet(UseVertexColors);
        }

        void begin();

        void setAmbientLight(const Color& color)
        {
            _program.setUniformVec4(_program.ambientLightLoc, color);
        }

        template <typename LightIterator>
        void setLights(LightIterator, LightIterator);


        void setMaterial(const Material&, void* = nullptr);

        void render(TriangleMesh&, const mat4f&, const mat3f&);
        void render(TriangleMesh&, const vec3f&, const mat3f&, const vec3f&);

        void updateShaders();

        void render(TriangleMesh& mesh)
        {
            render(mesh, mat4f::identity(), mat3f::identity());
        }

        void end();

    private:
        struct GLState
        {
            GLSL::Program* program;
            bool depthTest;
            int texture;
            int vao;
        };

        struct GLProgram : public GLSL::Program
        {
            struct LightPropLoc
            {
                GLint type;
                GLint color;
                GLint position;
                GLint direction;
                GLint falloff;
                GLint range;
                GLint angle;
            };

            GLint mvMatrixLoc;
            GLint normalMatrixLoc;
            GLint mvpMatrixLoc;
            GLint viewportMatrixLoc;
            GLint projectionTypeLoc;
            GLint ambientLightLoc;
            GLint lightCountLoc;
            LightPropLoc lightLocs[maxLights];
            GLint OaLoc;
            GLint OdLoc;
            GLint OsLoc;
            GLint nsLoc;

            GLProgram();

            void renderMaterial(const Material&);
            void renderLight(int, const Light&, const Camera&);
            void renderDefaultLights();

        private:
            GLint uniformLightLocation(int i, const char* field)
            {
                constexpr auto maxName = 32;
                char name[maxName];

                snprintf(name, maxName, "lights[%d].%s", i, field);
                return uniformLocation(name);
            }

            void initProgram();
            void initUniformLocations();
            void initSubroutineIndices();
            void initUniformLightLocations(int);

        }; // GLProgram

        GLProgram _program;
        GLState _lastState;
        mat4f _viewportMatrix;
        int _lightCount{};
        GLuint _texture{};

        bool setLight(int, const Light&);
        void updateView();

    }; // MyRenderer

    template <typename LightIterator>
    void MyRenderer::setLights(LightIterator begin, LightIterator end)
    {
        _lightCount = 0;
        while (begin != end)
            if (setLight(_lightCount, **begin++))
                if (++_lightCount == maxLights)
                    break;
        _program.setUniform(_program.lightCountLoc, _lightCount);
    }

} // end namespace cg

#endif // __MyRenderer_h