/*
Práctica 7: Iluminación 1 
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture animalesTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Model Auto_Cuerpo;
Model Auto_Llanta1;
Model Auto_Llanta2;
Model Auto_Llanta3;
Model Auto_Llanta4;
Model Auto_Cofre;

Model Lamp_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


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

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};	GLfloat cubo_vertices[] = {
			// front
			//x		y		z		S		T			NX		NY		NZ
			-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
			0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
			0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
			-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
			// right
			//x		y		z		S		T
			0.5f, -0.5f,  0.5f,	    0.754f,  0.34f,		-1.0f,	0.0f,	0.0f,
			0.5f, -0.5f,  -0.5f,	0.99f,	0.34f,		-1.0f,	0.0f,	0.0f,
			0.5f,  0.5f,  -0.5f,	0.99f,	0.66f,		-1.0f,	0.0f,	0.0f,
			0.5f,  0.5f,  0.5f,	    0.754f,	0.66f,		-1.0f,	0.0f,	0.0f,
			// back
			-0.5f, -0.5f, -0.5f,	0.24f,  0.34f,		0.0f,	0.0f,	1.0f,
			0.5f, -0.5f, -0.5f,		0.01f,	0.34f,		0.0f,	0.0f,	1.0f,
			0.5f,  0.5f, -0.5f,		0.01f,	0.66f,		0.0f,	0.0f,	1.0f,
			-0.5f,  0.5f, -0.5f,	0.24f,	0.66f,		0.0f,	0.0f,	1.0f,

			// left
			//x		y		z		S		T
			-0.5f, -0.5f,  -0.5f,	0.51f,  0.34f,		1.0f,	0.0f,	0.0f,
			-0.5f, -0.5f,  0.5f,	0.74f,	0.34f,		1.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  0.5f,	0.74f,	0.66f,		1.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  -0.5f,	0.51f,	0.66f,		1.0f,	0.0f,	0.0f,

			// bottom
			//x		y		z		S		T
			-0.5f, -0.5f,  0.5f,	0.74f,  0.01f,		0.0f,	1.0f,	0.0f,
			0.5f,  -0.5f,  0.5f,	0.51f,	0.01f,		0.0f,	1.0f,	0.0f,
			 0.5f,  -0.5f,  -0.5f,	0.51f,	0.33f,		0.0f,	1.0f,	0.0f,
			-0.5f, -0.5f,  -0.5f,	0.74f,	0.33f,		0.0f,	1.0f,	0.0f,

			//UP
			 //x		y		z		S		T
			 -0.5f, 0.5f,  0.5f,	0.51f,  0.67f,		0.0f,	-1.0f,	0.0f,
			 0.5f,  0.5f,  0.5f,	0.74f,	0.67f,		0.0f,	-1.0f,	0.0f,
			  0.5f, 0.5f,  -0.5f,	0.74f,	0.99f,		0.0f,	-1.0f,	0.0f,
			 -0.5f, 0.5f,  -0.5f,	0.51f,	0.99f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	animalesTexture = Texture("Textures/dado_animales_modificado.tga");
	animalesTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	

	Auto_Cuerpo = Model();
	Auto_Llanta1 = Model();
	Auto_Llanta2 = Model();
	Auto_Llanta3 = Model();
	Auto_Llanta4 = Model();
	Auto_Cofre = Model();

	Auto_Cuerpo.LoadModel("Models/P6-7.obj");
	Auto_Llanta1.LoadModel("Models/P6-8.obj");
	Auto_Llanta2.LoadModel("Models/P6-9.obj");
	Auto_Llanta3.LoadModel("Models/P6-10.obj");
	Auto_Llanta4.LoadModel("Models/P6-11.obj");
	Auto_Cofre.LoadModel("Models/P6-12.obj");

	Lamp_M = Model();
	Lamp_M.LoadModel("Models/Lamp.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

    // Otra luz puntual para la lámpara: luz puntual blanca 
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,   // blanca
		4.0f, 3.0f,
		18.0f, -0.7f, 25.0f,
		1.0f, 0.09f, 0.032f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;


	
	// Spotlight del coche, se declara con dirección y posición iniciales, pero luego se actualizan en el loop para que sigan al coche
	spotLights[1] = SpotLight(0.0f, 0.2f, 1.0f,   // azul
		1.0f, 3.0f,        // ambient y diffuse
		0.25f, 0.45f, 1.70f,   // posición inicial
		0.0f, 1.0f, 0.0f,  // dirección inicial temporal
		1.0f, 0.0f, 0.0f,// atenuación
		25.0f               // ángulo del faro
	);
	spotLightCount++;

	// Spotlight del helicóptero: seguirá la transformación del helicóptero y apuntará hacia -Y (hacia abajo)
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;


//se crean mas luces puntuales y spotlight 

// Colores para el spotlight del coche: rojo, amarillo, verde, cian, azul, magenta
std::vector<glm::vec3> carSpotColors = {
	glm::vec3(1.0f, 0.0f, 0.0f), // rojo
	glm::vec3(1.0f, 1.0f, 0.0f), // amarillo
	glm::vec3(0.0f, 1.0f, 0.0f), // verde
	glm::vec3(0.0f, 1.0f, 1.0f), // cian
	glm::vec3(0.0f, 0.0f, 1.0f), // azul
	glm::vec3(1.0f, 0.0f, 1.0f)  // magenta
};

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();


		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));

		//información al shader de fuentes de iluminación
      shaderList[0].SetDirectionalLight(&mainLight);
		// Activar/desactivar la luz de la lámpara según estado de la ventana (tecla L)
		pointLights[1].SetEnabled(mainWindow.getLampOn());
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		////Instancia del coche 
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.5f, -3.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Kitt_M.RenderModel();

		////Llanta delantera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta trasera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta delantera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta trasera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		//Helicoptero
		// Construir matriz jerárquica del helicóptero
		glm::mat4 heliBase = glm::mat4(1.0f);
		heliBase = glm::translate(heliBase, glm::vec3(mainWindow.getEliX(), 10.0f, mainWindow.getEliZ()));
		heliBase = glm::scale(heliBase, glm::vec3(0.3f, 0.3f, 0.3f));
		heliBase = glm::rotate(heliBase, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		heliBase = glm::rotate(heliBase, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		// Offset local para montar el foco relativo al pivote del helicóptero
		glm::vec4 heliLocalOffset = glm::vec4(0.0f, -0.5f, 0.0f, 1.0f);
		glm::vec4 heliPosWorld4 = heliBase * heliLocalOffset;
		glm::vec3 heliPosWorld = glm::vec3(heliPosWorld4);
		// Dirección local -Y transformada por la matriz (w=0 para dirección)
		glm::vec4 heliDirWorld4 = heliBase * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		glm::vec3 heliDirWorld = glm::normalize(glm::vec3(heliDirWorld4));
		// Actualizar spotlight del helicóptero para que siga la jerarquía y apunte hacia -Y
		spotLights[2].SetFlash(heliPosWorld, heliDirWorld);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(heliBase));
		Blackhawk_M.RenderModel();


		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//meshList[4]->RenderMesh();



		color = glm::vec3(0.1f, 0.1f, 0.1f);

		// =====================
		// CUERPO DEL AUTO
		// =====================
        glm::mat4 autoBase = glm::mat4(1.0f);
		autoBase = glm::rotate(autoBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		autoBase = glm::translate(autoBase, glm::vec3(12.0f, 0.0f, mainWindow.getautoX()));
		autoBase = glm::scale(autoBase, glm::vec3(4.0f, 4.0f, 4.0f));
        // Calcular la posición y dirección del foco a partir de la matriz "autoBase"
		// Esto aplica la jerarquía (traslación/rotación/escala) y sitúa el foco en el sistema de mundo.
        // Offset local respecto al pivote del modelo (montaje del foco)
		glm::vec4 localOffset = glm::vec4(0.6f, 0.7f, 1.45f, 1.0f);
		glm::vec4 posWorld = autoBase * localOffset; // transforma el punto local a coordenadas mundo
		glm::vec3 posCoche = glm::vec3(posWorld);
		// Dirección local +Z del coche transformada por la matriz
		glm::vec4 dirWorld4 = autoBase * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		glm::vec3 dirCoche = glm::normalize(glm::vec3(dirWorld4));
		// Usar SetFlash (posición + dirección) para que el comportamiento sea equivalente a la linterna
		spotLights[1].SetFlash(posCoche, dirCoche);
        // Ajustar color del spotlight del coche según índice ciclado desde la ventana (tecla C)
		if (!carSpotColors.empty()) {
			int idx = mainWindow.getSpotColorIndex();
			idx = idx % (int)carSpotColors.size();
			glm::vec3 sc = carSpotColors[idx];
			spotLights[1].SetColor(sc.r, sc.g, sc.b);
		}
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(autoBase));
		Auto_Cuerpo.RenderModel();


		// =====================
		// COFRE
		// =====================
		glm::mat4 cofreBase = autoBase;

		// mover al pivote del cofre
		cofreBase = glm::translate(cofreBase, glm::vec3(0.0f, 1.0f, 1.0f));
		cofreBase = glm::rotate(cofreBase, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
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
		llanta2Base = glm::rotate(llanta2Base, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
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
		llanta4Base = glm::rotate(llanta4Base, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		llanta4Base = glm::rotate(llanta4Base, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 llanta4Model = llanta4Base;
		llanta4Model = glm::translate(llanta4Model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(llanta4Model));
		Auto_Llanta4.RenderModel();


		// Colocar el modelo Lamp
		glm::mat4 lampModel = glm::mat4(1.0f);
		glm::vec3 lampWorldPos = glm::vec3(18.0f, -0.7f, 25.0f);
		lampModel = glm::translate(lampModel, lampWorldPos);
		lampModel = glm::scale(lampModel, glm::vec3(7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(lampModel));
		Lamp_M.RenderModel();
		// Actualizar la posición de la luz puntual blanca (pointLights[1]) usando un offset local
		glm::vec4 lampLocalOffset = glm::vec4(0.0f, 20.0f, -4.0f, 1.0f); // ajusta el offset local según sea necesario para que el foco quede en la posición deseada respecto a la lámpara
		glm::vec3 pointPosWorld = lampWorldPos + glm::vec3(lampLocalOffset);
		pointLights[1].SetPos(pointPosWorld);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		animalesTexture.UseTexture();
		meshList[4]->RenderMesh();


		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
