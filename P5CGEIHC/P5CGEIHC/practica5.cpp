/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model Goddard_M;
Model Goddard_Cuerpo;
Model Goddard_Mandibula;
Model Goddard_Pata1;
Model Goddard_Pata2;
Model Goddard_Pata3;
Model Goddard_Pata4;

Model Auto_Cuerpo;
Model Auto_Llanta1;
Model Auto_Llanta2;
Model Auto_Llanta3;
Model Auto_Llanta4;
Model Auto_Cofre;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	Goddard_M = Model();
	Goddard_M.LoadModel("Models/goddard_base.obj");
	Goddard_Cuerpo = Model();
	Goddard_Mandibula = Model();
	Goddard_Pata1 = Model();
	Goddard_Pata2 = Model();
	Goddard_Pata3 = Model();
	Goddard_Pata4 = Model();

	Goddard_Cuerpo.LoadModel("Models/P5-6.obj");
	Goddard_Mandibula.LoadModel("Models/P5-1.obj");
	Goddard_Pata1.LoadModel("Models/P5-2.obj");
	Goddard_Pata2.LoadModel("Models/P5-3.obj");
	Goddard_Pata3.LoadModel("Models/P5-4.obj");
	Goddard_Pata4.LoadModel("Models/P5-5.obj");

	Auto_Cuerpo = Model();
	Auto_Llanta1 = Model();
	Auto_Llanta2 = Model();
	Auto_Llanta3 = Model();
	Auto_Llanta4 = Model();
	Auto_Cofre = Model();

	Auto_Cuerpo.LoadModel("Models/P5-7.obj");
	Auto_Llanta1.LoadModel("Models/P5-8.obj");
	Auto_Llanta2.LoadModel("Models/P5-9.obj");
	Auto_Llanta3.LoadModel("Models/P5-10.obj");
	Auto_Llanta4.LoadModel("Models/P5-11.obj");
	Auto_Cofre.LoadModel("Models/P5-12.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		////------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		////Goddard
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro
		//
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, -1.5f));
		////modelaux = model;
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Goddard_M.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		////En sesión se separara una parte del modelo de Goddard y se unirá por jeraquía al cuerpo
		//modelaux = model;
		////Cola


		////Siguientes modelos
		///* Ejercicio:
		//1.- Separar las 4 patas de Goddard del modelo del cuerpo, unir por medio de jerarquía cada pata al cuerpo de Goddard
		//2.- Hacer que al presionar una tecla cada pata pueda rotar un máximo de 45° "hacia adelante y hacia atrás"
		//*/

		////pata delantera derecha
		//modelaux = model;

		//// pata delantera izquierda
		//modelaux = model;
		////pata trasera derecha
		//modelaux = model;

		////pata trasera izquierda
		//modelaux = model;


		//------------* INICIA DIBUJO DE GODDARD JERÁRQUICO *-------------------*

		color = glm::vec3(0.0f, 0.0f, 0.0f); // modelo negro

		// =====================
		// CUERPO PRINCIPAL
		// =====================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // ajusta si hace falta
		modelaux = model;

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Cuerpo.RenderModel();


		// =====================
		// MANDÍBULA INFERIOR
		// =====================
		glm::mat4 mandibulaBase = modelaux;

		// mover al pivote aproximado de la mandíbula
		mandibulaBase = glm::translate(mandibulaBase, glm::vec3(-2.0f, 3.0f, -0.5f));
		mandibulaBase = glm::rotate(mandibulaBase,
			glm::radians(mainWindow.getmandibula()),
			glm::vec3(0.0f, 0.0f, 1.0f));

		// regresar el modelo a su posición original respecto al pivote
		glm::mat4 mandibulaModel = mandibulaBase;
		mandibulaModel = glm::translate(mandibulaModel, glm::vec3(0.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mandibulaModel));
		Goddard_Mandibula.RenderModel();


		// =====================
		// PATA 1 - DELANTERA DERECHA
		// =====================
		glm::mat4 pata1Base = modelaux;
		pata1Base = glm::translate(pata1Base, glm::vec3(-1.0f, 2.2f, -1.0f));
		pata1Base = glm::rotate(pata1Base,
			glm::radians(mainWindow.getpata1()),
			glm::vec3(0.0f, 0.0f, 1.0f));

		// regresar al origen local de la malla
		glm::mat4 pata1Model = pata1Base;
		pata1Model = glm::translate(pata1Model, glm::vec3(-0.2f, -2.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(pata1Model));
		Goddard_Pata1.RenderModel();


		// =====================
		// PATA 2 - DELANTERA IZQUIERDA
		// =====================
		glm::mat4 pata2Base = modelaux;
		pata2Base = glm::translate(pata2Base, glm::vec3(-1.0f, 2.2f, 0.4f));
		pata2Base = glm::rotate(pata2Base,
			glm::radians(mainWindow.getpata2()),
			glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 pata2Model = pata2Base;
		pata2Model = glm::translate(pata2Model, glm::vec3(-0.2f, -2.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(pata2Model));
		Goddard_Pata2.RenderModel();


		// =====================
		// PATA 3 - TRASERA DERECHA
		// =====================
		glm::mat4 pata3Base = modelaux;
		pata3Base = glm::translate(pata3Base, glm::vec3(0.95f, 1.50f, -1.0f));
		pata3Base = glm::rotate(pata3Base,
			glm::radians(mainWindow.getpata3()),
			glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 pata3Model = pata3Base;
		pata3Model = glm::translate(pata3Model, glm::vec3(-0.1f, -1.5f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(pata3Model));
		Goddard_Pata3.RenderModel();


		// =====================
		// PATA 4 - TRASERA IZQUIERDA
		// =====================
		glm::mat4 pata4Base = modelaux;
		pata4Base = glm::translate(pata4Base, glm::vec3(0.95f, 1.5f, 0.4f));
		pata4Base = glm::rotate(pata4Base,
			glm::radians(mainWindow.getpata4()),
			glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 pata4Model = pata4Base;
		pata4Model = glm::translate(pata4Model, glm::vec3(-0.1f, -1.5f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(pata4Model));
		Goddard_Pata4.RenderModel();


		// =======================================================
		// EJERCICIO DE REPORTE - AUTO JERÁRQUICO
		// =======================================================
		color = glm::vec3(0.1f, 0.1f, 0.1f);

		// =====================
		// CUERPO DEL AUTO
		// =====================
		glm::mat4 autoBase = glm::mat4(1.0f);
		autoBase = glm::rotate(autoBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		autoBase = glm::translate(autoBase, glm::vec3(12.0f, -1.6f, mainWindow.getautoX()));
		autoBase = glm::scale(autoBase, glm::vec3(2.0f, 2.0f, 2.0f));
		color = glm::vec3(0.53f, 0.81f, 0.98f); // azul cielo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(autoBase));
		Auto_Cuerpo.RenderModel();


		// =====================
		// COFRE
		// =====================
		glm::mat4 cofreBase = autoBase;

		// mover al pivote del cofre
		cofreBase = glm::translate(cofreBase, glm::vec3(0.0f, 1.0f, 1.0f));
		cofreBase = glm::rotate(cofreBase,
			glm::radians(mainWindow.getcofre()),
			glm::vec3(1.0f, 0.0f, 0.0f));

		// regresar modelo a su posición
		glm::mat4 cofreModel = cofreBase;
		cofreModel = glm::translate(cofreModel, glm::vec3(0.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cofreModel));
		Auto_Cofre.RenderModel();


		// =====================
		// LLANTAS
		// =====================
		float giroLlanta = mainWindow.getrotLlantasAuto();

		// Llanta 1 - enfrente derecha
		glm::mat4 llanta1Base = autoBase;
		llanta1Base = glm::translate(llanta1Base, glm::vec3(-0.8f, 0.2f, 1.2f));
		llanta1Base = glm::rotate(llanta1Base, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 llanta1Model = llanta1Base;
		llanta1Model = glm::translate(llanta1Model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(llanta1Model));
		Auto_Llanta1.RenderModel();

		// Llanta 2 - enfrente izquierda
		glm::mat4 llanta2Base = autoBase;
		llanta2Base = glm::translate(llanta2Base, glm::vec3(0.8f, 0.2f, 1.2f));
		llanta2Base = glm::rotate(llanta2Base, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 llanta2Model = llanta2Base;
		llanta2Model = glm::translate(llanta2Model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(llanta2Model));
		Auto_Llanta2.RenderModel();

		// Llanta 3 - atrás derecha
		glm::mat4 llanta3Base = autoBase;
		llanta3Base = glm::translate(llanta3Base, glm::vec3(-0.8f, 0.2f, -1.2f));
		llanta3Base = glm::rotate(llanta3Base, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 llanta3Model = llanta3Base;
		llanta3Model = glm::translate(llanta3Model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(llanta3Model));
		Auto_Llanta3.RenderModel();

		// Llanta 4 - atrás izquierda
		glm::mat4 llanta4Base = autoBase;
		llanta4Base = glm::translate(llanta4Base, glm::vec3(0.8f, 0.2f, -1.2f));
		llanta4Base = glm::rotate(llanta4Base, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 llanta4Model = llanta4Base;
		llanta4Model = glm::translate(llanta4Model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(llanta4Model));
		Auto_Llanta4.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
