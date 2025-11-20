#ifndef __MyGraphics3_h
#define __MyGraphics3_h

#include "graphics/GLGraphics3.h"

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
class MyGraphics3 : public cg::GLGraphics3
{
public:
	void teste()
	{
		return;
	}



private:
	
};


#endif