#pragma once
#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }

	GLfloat getXChange();
	GLfloat getYChange();

	GLfloat getmuevex() { return muevex; }

	// Articulaciones Goddard
	GLfloat getmandibula() { return mandibula; }
	GLfloat getpata1() { return pata1; }   // delantera derecha
	GLfloat getpata2() { return pata2; }   // delantera izquierda
	GLfloat getpata3() { return pata3; }   // trasera derecha
	GLfloat getpata4() { return pata4; }   // trasera izquierda
	GLfloat getcola() { return cola; }     // opcional, por si luego la agregas

	bool getShouldClose() {
		return glfwWindowShouldClose(mainWindow);
	}

	bool* getsKeys() { return keys; }

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;

	void createCallbacks();

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;

	// Articulaciones
	GLfloat mandibula;
	GLfloat pata1;
	GLfloat pata2;
	GLfloat pata3;
	GLfloat pata4;
	GLfloat cola;

	bool mouseFirstMoved;

	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};