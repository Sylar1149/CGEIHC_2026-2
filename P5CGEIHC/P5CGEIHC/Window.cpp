#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;

	muevex = 2.0f;

	mandibula = 0.0f;
	pata1 = 0.0f;
	pata2 = 0.0f;
	pata3 = 0.0f;
	pata4 = 0.0f;
	cola = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;

	muevex = 2.0f;

	mandibula = 0.0f;
	pata1 = 0.0f;
	pata2 = 0.0f;
	pata3 = 0.0f;
	pata4 = 0.0f;
	cola = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
}

int Window::Initialise()
{
	// Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	// Variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Crear ventana
	mainWindow = glfwCreateWindow(width, height, "Practica XX: Nombre de la practica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	// Obtener tamaño de buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Asignar contexto
	glfwMakeContextCurrent(mainWindow);

	// Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);

	// Manejar teclado y mouse
	createCallbacks();

	// Permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Habilitar profundidad
	glEnable(GL_DEPTH_TEST);

	// Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Movimiento extra en X (si todavía lo usas)
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		theWindow->muevex += 1.0f;
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		theWindow->muevex -= 1.0f;
	}

	// =========================
	// ARTICULACIONES DE GODDARD
	// límite de -45° a +45°
	// =========================

	// Mandíbula inferior
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (theWindow->mandibula < 45.0f)
			theWindow->mandibula += 5.0f;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		if (theWindow->mandibula > 0.0f)
			theWindow->mandibula -= 5.0f;
	}

	// Pata 1 - delantera derecha
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		if (theWindow->pata1 < 45.0f)
			theWindow->pata1 += 5.0f;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		if (theWindow->pata1 > -45.0f)
			theWindow->pata1 -= 5.0f;
	}

	// Pata 2 - delantera izquierda
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		if (theWindow->pata2 < 45.0f)
			theWindow->pata2 += 5.0f;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (theWindow->pata2 > -45.0f)
			theWindow->pata2 -= 5.0f;
	}

	// Pata 3 - trasera derecha
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		if (theWindow->pata3 < 45.0f)
			theWindow->pata3 += 5.0f;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		if (theWindow->pata3 > -45.0f)
			theWindow->pata3 -= 5.0f;
	}

	// Pata 4 - trasera izquierda
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (theWindow->pata4 < 45.0f)
			theWindow->pata4 += 5.0f;
	}
	if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
	{
		if (theWindow->pata4 > -45.0f)
			theWindow->pata4 -= 5.0f;
	}

	// Guardar estado de teclas
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = (GLfloat)xPos;
		theWindow->lastY = (GLfloat)yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = (GLfloat)xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - (GLfloat)yPos;

	theWindow->lastX = (GLfloat)xPos;
	theWindow->lastY = (GLfloat)yPos;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}