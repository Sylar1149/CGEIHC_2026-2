#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getllanta1() { return llanta1; }
	GLfloat getllanta2() { return llanta2; }
	GLfloat getllanta3() { return llanta3; }
	GLfloat getllanta4() { return llanta4; }

	GLfloat getpata1a() { return pata1a; }
	GLfloat getpata1b() { return pata1b; }

	GLfloat getpata2a() { return pata2a; }
	GLfloat getpata2b() { return pata2b; }

	GLfloat getpata3a() { return pata3a; }
	GLfloat getpata3b() { return pata3b; }

	GLfloat getpata4a() { return pata4a; }
	GLfloat getpata4b() { return pata4b; }

	GLfloat getcola() { return cola; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6;
	GLfloat llanta1, llanta2, llanta3, llanta4;
	GLfloat pata1a, pata1b;
	GLfloat pata2a, pata2b;
	GLfloat pata3a, pata3b;
	GLfloat pata4a, pata4b;
	GLfloat cola;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

