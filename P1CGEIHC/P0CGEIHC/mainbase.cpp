#include <stdio.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;

int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Perfil y uso de OpenGL para codigo legacy y moderno
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL); //Pixeles de ancho, alto y titulo de la ventana, ademas ventana a pantalla completa y compartir recursos con otra ventana

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight); //Se obtiene el tamaño del buffer de la ventana para poder asignar el viewport y que se vea correctamente en la ventana, esto es necesario porque el tamaño del buffer puede ser diferente al tamaño de la ventana dependiendo de la resolución del monitor

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow); //Se asigna el contexto de la ventana para que se puedan usar las funciones de OpenGL en esta ventana

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE; // Glew es una libreria que permite usar las funciones de OpenGL, al asignar glewExperimental a GL_TRUE se permite usar las nuevas extensiones de OpenGL, esto es necesario para poder usar las funciones de OpenGL 4.3 que se asignaron en las líneas anteriores

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}
	//Aqui empieza el codigo de OpenGl
	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight); //Se pasan los pixeles almacenados en memoria
	printf("Version de Opengl: %s \n",glGetString(GL_VERSION));
	printf("Marca: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("Shaders: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents(); // Espera por eventos de entrada, como el cierre de la ventana o el movimiento del mouse, y los procesa. Esto es necesario para que la ventana responda a las acciones del usuario.

		//Limpiar la ventana
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f); //Asigna el color de la ventana, en este caso verde, con valores de rojo, verde, azul y alfa (transparencia) entre 0.0f y 1.0f
		glClear(GL_COLOR_BUFFER_BIT); //Limpia el buffer de color, lo que significa que borra el contenido anterior de la ventana y la llena con el color asignado por glClearColor. Esto es necesario para que se vea el nuevo contenido en la ventana y no se superponga con el contenido anterior.

		glfwSwapBuffers(mainWindow); //Maneja un buffer doble, lo que significa que mientras se muestra un buffer en la pantalla, el otro buffer se está dibujando en segundo plano. Cuando se llama a glfwSwapBuffers, los buffers se intercambian, mostrando el nuevo contenido en la pantalla. Esto ayuda a evitar parpadeos y proporciona una experiencia visual más suave.

	}


	return 0;
}