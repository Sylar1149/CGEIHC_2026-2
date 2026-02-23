//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);


	GLfloat vertices_trianguloazul[] = {
		//X      Y      Z      R    G    B
		-1.0f, -1.0f, 0.5f,   0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.5f,   0.0f, 0.0f, 1.0f,
		 0.0f,  1.0f, 0.5f,   0.0f, 0.0f, 1.0f,
	};
	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	GLfloat vertices_trianguloverde[] = {
		//X      Y      Z      R        G        B
		-1.0f, -1.0f, 0.5f,   0.0039f, 0.4980f, 0.0078f,
		 1.0f, -1.0f, 0.5f,   0.0039f, 0.4980f, 0.0078f,
		 0.0f,  1.0f, 0.5f,   0.0039f, 0.4980f, 0.0078f,
	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_cuadradorojo[] = {
		//X      Y      Z      R    G    B
		-0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
	};
	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);


	GLfloat vertices_cuadradocafe[] = {
		//X      Y      Z      R       G       B
		-0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,

		-0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		-0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);


}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//
		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		////
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[0]->RenderMeshColor();

//
//		// =====================
//// FIGURAS EJERCICIO 1
//// =====================
//		shaderList[1].useShader();
//		uniformModel = shaderList[1].getModelLocation();
//		uniformProjection = shaderList[1].getProjectLocation();
//
//		// IMPORTANTE: manda la proyección UNA vez aquí
//		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
//
//		// Para que no se vean gigantes: escala general para triángulos
//		glm::vec3 scaleTri(0.35f, 0.35f, 1.0f);
//		// Escala general para cuadrados (ya son más chicos, pero igual)
//		glm::vec3 scaleSq(0.60f, 0.60f, 1.0f);
//
//		// ---- Fila inferior: triángulos (rojo, azul, verde) ----
//
//		// Triángulo rojo (meshColorList[1])
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(-2.2f, -1.2f, -7.0f));
//		model = glm::scale(model, scaleTri);
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshColorList[1]->RenderMeshColor();
//
//		// Triángulo azul (meshColorList[2])
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, -1.2f, -7.0f));
//		model = glm::scale(model, scaleTri);
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshColorList[2]->RenderMeshColor();
//
//		// Triángulo verde (meshColorList[3])
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(2.2f, -1.2f, -7.0f));
//		model = glm::scale(model, scaleTri);
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshColorList[3]->RenderMeshColor();
//
//		// ---- Fila superior: cuadrados (verde, rojo, café) ----
//
//		// Cuadrado verde (meshColorList[4])
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(-2.2f, 1.2f, -7.0f));
//		model = glm::scale(model, scaleSq);
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshColorList[4]->RenderMeshColor();
//
//		// Cuadrado rojo (meshColorList[5])
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, 1.2f, -7.0f));
//		model = glm::scale(model, scaleSq);
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshColorList[5]->RenderMeshColor();
//
//		// Cuadrado café (meshColorList[6])
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(2.2f, 1.2f, -7.0f));
//		model = glm::scale(model, scaleSq);
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshColorList[6]->RenderMeshColor();
//


		////Agregar cuadrado verde y triángulo rojo usando el mismo shader de las letras, pero con diferente matriz de modelo para que se vean en diferentes posiciones

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -3.0f, -7.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//meshColorList[4]->RenderMeshColor();


		////Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		//shaderList[0].useShader(); 
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();
		//angulo += 0.01;
		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		////model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[0]->RenderMesh();


		////Agregar cubo
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, -5.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//meshList[1]->RenderMesh();
	
		
		// =====================
		// EJERCICIO 2 - CASITA
		// =====================
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Valores generales
		float z = -5.0f;                 
		glm::vec3 sq(1.0f, 1.0f, 1.0f);  
		glm::vec3 tri(0.6f, 0.6f, 1.0f); 
		
		//Cuerpo casa CUADRADO ROJO
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, -10.0f));
		model = glm::scale(model, glm::vec3(5.6f, 5.5, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();


		//Techo casa TRIANGUKO AZUL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.6f, -10.0f));
		model = glm::scale(model, glm::vec3(3.5f, 1.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

		//Puerta CUADRADO VERDE
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.6f, -9.9f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();

		//Ventanas CUADRADO VERDE
		// Izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.6f, -0.8f, -9.9f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();

		// Derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.6f, -0.8f, -9.9f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();

		//Arbol izquierdo
		// Tronco
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.0f, -4.0f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[6]->RenderMeshColor();

		// Hojas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.0f, -2.1f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();

		//Arbol derecho
		// Tronco
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -4.0f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[6]->RenderMeshColor();

		// Hojas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -2.1f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/