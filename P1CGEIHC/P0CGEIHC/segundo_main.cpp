#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
//GLuint VAO, VBO, shader; //Se crean tres variables de tipo entero
GLuint VAO_Rombo, VBO_Rombo;
GLuint VAO_Trap, VBO_Trap;
GLuint shader;


//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//El vertex shader se encarga de recibir las coordenadas de los vértices y asignarles un espacio en la pantalla, y el fragment shader se encarga de darle color a los fragmentos que se dibujan, en este caso el triángulo


//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.9f,0.8f,0.2f,1.0f);	 			\n\
}";
//El fragment shader se encarga de darle color a los fragmentos que se dibujan, en este caso el triángulo, y el vertex shader se encarga de recibir las coordenadas de los vértices y asignarles un espacio en la pantalla


void CrearRombo()
{
	// Rombo centrado a la izquierda (ajusta si quieres)
	GLfloat rombo[] = {
		// Triángulo 1 (arriba)
		-0.6f,  0.6f, 0.0f,   // arriba
		-0.8f,  0.2f, 0.0f,   // izquierda
		-0.4f,  0.2f, 0.0f,   // derecha

		// Triángulo 2 (abajo)
		-0.6f, -0.2f, 0.0f,   // abajo
		-0.8f,  0.2f, 0.0f,   // izquierda
		-0.4f,  0.2f, 0.0f    // derecha
	};

	glGenVertexArrays(1, &VAO_Rombo);
	glBindVertexArray(VAO_Rombo);

	glGenBuffers(1, &VBO_Rombo);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Rombo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rombo), rombo, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void CrearTrapecio()
{
	// Trapecio isósceles a la derecha
	// (base inferior más larga, superior más corta, centradas)
	GLfloat trap[] = {
		// Triángulo 1
		 0.2f,  0.2f, 0.0f,   // sup izq
		 0.6f,  0.2f, 0.0f,   // sup der
		 0.1f, -0.4f, 0.0f,   // inf izq

		 // Triángulo 2
		  0.6f,  0.2f, 0.0f,   // sup der
		  0.7f, -0.4f, 0.0f,   // inf der
		  0.1f, -0.4f, 0.0f    // inf izq
	};

	glGenVertexArrays(1, &VAO_Trap);
	glBindVertexArray(VAO_Trap);

	glGenBuffers(1, &VBO_Trap);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Trap);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trap), trap, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



/*void CrearTriangulo()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f,0.0f,
		1.0f,-1.0f, 0.0f,
		0.0f,1.0f,0.0f,

		-1.0f,-1.0f,0.0f,
		0.0f, 1.0f,0.0f,
		-1.0f,1.0f, 0.0f
	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		glEnableVertexAttribArray(0); //Envia los datos al vertex shader, el 0 es el location del vertex shader, el 3 es la cantidad de datos por vértice, GL_FLOAT es el tipo de dato, GL_FALSE es para normalizar los datos, el stride es para saltar cierta cantidad de datos en caso de haber datos de color por ejemplo, y el último parámetro es un puntero a los datos pero como ya se los pasamos a la tarjeta gráfica con glBufferData se pone 0
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}*/
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Llamada a las funciones creadas antes del main
	CrearRombo();
	CrearTrapecio();
	CompileShaders();

	//CrearTriangulo();
	CompileShaders();

	double lastChange = glfwGetTime();
	int state = 0; // 0=rojo, 1=verde, 2=azul

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		double now = glfwGetTime();
		double period = 0.5; // segundos

		if (now - lastChange >= period) {
			state = (state + 1) % 3;
			lastChange = now;
		}

		if (state == 0)      glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // rojo
		else if (state == 1) glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // verde
		else                 glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // azul


		//Limpiar la ventana
		//glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		// Dibujar rombo
		glBindVertexArray(VAO_Rombo);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// Dibujar trapecio
		glBindVertexArray(VAO_Trap);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);



		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES,0,6); //Comienza desde el primer dato hasta el tercero
		//glBindVertexArray(0);

		glUseProgram(0); //Dejar de usar el shader para que no afecte a otros objetos que se dibujen después, en este caso no hay pero es buena práctica

		glfwSwapBuffers(mainWindow); //Intercambiar los buffers, el back buffer es el que se dibuja y el front buffer es el que se muestra, al intercambiarlos se muestra lo que se dibujó en el back buffer, esto es para evitar parpadeos y que se vea un dibujo completo
		 
		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
	}


	return 0;
}