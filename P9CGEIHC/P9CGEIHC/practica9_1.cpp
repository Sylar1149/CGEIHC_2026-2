/*
Animación:
Sesión 1: Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Sesión 2: Compleja: Por medio de funciones y algoritmos. Textura Animada
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

//variables para animación
float movCoche = 0.0f;            // recorrido actual (0..target)
float movOffset = 0.1f;          // velocidad de traslación (unidades/seg)
float rotllanta = 0.0f;          // ángulo de giro de las llantas (grados)
float rotllantaOffset = 10.0f;  // velocidad de giro de las llantas (grados/seg)
int carState = 0;                // 0: avanzar, 1: girar, 2: regresar, 3: girar de regreso
float carYaw = 0.0f;             // ángulo de giro actual del coche (grados)
float yawSpeed = 180.0f;         // velocidad de giro durante el giro (grados/seg)
const float carTarget = 50.0f;   // distancia a recorrer antes de girar
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

// Nave (animación jerárquica)
float naveMov = 0.0f;              // recorrido actual de la nave (0..naveTarget)
int naveState = 0;                 // 0: avanzar, 1: girar, 2: regresar, 3: girar regreso
float naveYaw = 0.0f;              // guiñada de la nave (grados)
const float naveTarget = 20.0f;    // distancia a recorrer
float naveSpeed = 0.1f;            // velocidad lineal (unidades/seg)
float naveYawSpeed = 90.0f;       // velocidad de giro (grados/seg)
float naveBobPhase = 0.0f;         // fase para el movimiento de subida/bajada
float naveBobSpeed = 0.1f;         // velocidad angular para el bob (rad/s)
float naveBobAmp = 2.0f;           // amplitud del bob en Y (± unidades)

// Ala (aleteo)
float alaFlapPhase = 0.0f;
float alaFlapSpeed = 0.1f;         // velocidad del aleteo (rad/s)
float alaFlapAmp = 30.0f;          // amplitud del aleteo en grados

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;



Model Kitt_M;
Model Llanta_M;
Model Pista_M;
Model Nave_M;
Model Ala_M;

Model Auto_Cuerpo;
Model Auto_Llanta1;
Model Auto_Llanta2;
Model Auto_Llanta3;
Model Auto_Llanta4;
Model Auto_Cofre;

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



//cálculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");

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
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
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
		1.0f, 0.09f, 0.02f,// atenuación (menos deslumbrante a distancia)
		25.0f               // ángulo del faro
	);
	spotLightCount++;

    // Spotlight 2: luz de la nave. Se actualizará cada cuadro para seguir la nave.
	// Color blanco, intensidad moderada. Atenuación no nula para evitar iluminar infinitamente.
    spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.5f, 2.0f,
		0.0f, 0.0f, 0.0f,    // posición inicial (se actualizará en el loop)
		0.0f, 0.0f, 1.0f,    // dirección inicial (se actualizará en el loop)
		1.0f, 0.09f, 0.025f, // atenuación ajustada: constant, linear, exponent (menos caída)
		30.0f);
	spotLightCount++;

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
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
 

	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;

        // Actualización del coche: avanzar carTarget, girar 180, regresar, girar de nuevo (cíclico)
		switch (carState)
		{
		case 0: // avanzar
			if (movCoche < carTarget)
			{
				movCoche += movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime; // giran hacia adelante
			}
			else
			{
				carState = 1;
			}
			break;
		case 1: // girar 180 grados
			if (carYaw < 180.0f)
			{
				carYaw += yawSpeed * deltaTime;
				if (carYaw > 180.0f) carYaw = 180.0f;
			}
			else
			{
				carState = 2;
			}
			break;
		case 2: // regresar
			if (movCoche > 0.0f)
			{
				movCoche -= movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime; // giran hacia atrás (relativo al cuerpo)
			}
			else
			{
				carState = 3;
			}
			break;
		case 3: // girar para quedar en 0
			if (carYaw > 0.0f)
			{
				carYaw -= yawSpeed * deltaTime;
				if (carYaw < 0.0f) carYaw = 0.0f;
			}
			else
			{
				carState = 0;
			}
			break;
		}
		// --- Actualizar nave (movimiento cíclico) y aleteo del ala
		// Actualizar fases para bob y aleteo
		naveBobPhase += naveBobSpeed * deltaTime;
		alaFlapPhase += alaFlapSpeed * deltaTime;

		// Máquina de estados para la nave (avanzar, girar, regresar, girar de nuevo)
		switch (naveState)
		{
		case 0: // avanzar
			if (naveMov < naveTarget)
			{
				naveMov += naveSpeed * deltaTime;
			}
			else
			{
				naveState = 1;
			}
			break;
		case 1: // girar 180
			if (naveYaw < 180.0f)
			{
				naveYaw += naveYawSpeed * deltaTime;
				if (naveYaw > 180.0f) naveYaw = 180.0f;
			}
			else
			{
				naveState = 2;
			}
			break;
		case 2: // regresar
			if (naveMov > 0.0f)
			{
				naveMov -= naveSpeed * deltaTime;
			}
			else
			{
				naveState = 3;
			}
			break;
		case 3: // girar a 0
			if (naveYaw > 0.0f)
			{
				naveYaw -= naveYawSpeed * deltaTime;
				if (naveYaw < 0.0f) naveYaw = 0.0f;
			}
			else
			{
				naveState = 0;
			}
			break;
		}
	
		// Actualizar la posición y dirección del spotlight de la nave para pasarlo al shader
		{
			float naveBob = naveBobAmp * sin(naveBobPhase);
			// construir matriz parcial para calcular posición/dirección del foco
			glm::mat4 tmp = glm::mat4(1.0f);
			tmp = glm::translate(tmp, glm::vec3(0.0f, 3.0f + naveBob, 1.5f + naveMov));
			tmp = glm::rotate(tmp, glm::radians(naveYaw), glm::vec3(0.0f, 1.0f, 0.0f));
			// offset local del foco (ajustar según modelo)
			glm::vec4 localOffsetShip = glm::vec4(0.0f, 0.5f, 1.5f, 1.0f);
			glm::vec4 posWorld = tmp * localOffsetShip;
			glm::vec3 posNave = glm::vec3(posWorld);
			glm::vec3 dirNave = glm::normalize(glm::vec3(tmp * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
			spotLights[2].SetFlash(posNave, dirNave);
		}

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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Pista_M.RenderModel();

		////Instancia del coche 
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Kitt_M.RenderModel();

		////Llanta delantera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta trasera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta delantera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta trasera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();


		color = glm::vec3(0.1f, 0.1f, 0.1f);

		// =====================
		// CUERPO DEL AUTO
		// =====================
        glm::mat4 autoBase = glm::mat4(1.0f);
		// posicion del coche en el mundo
		glm::vec3 carPos = glm::vec3(12.0f, 0.0f, movCoche - 50.0f);
		autoBase = glm::translate(autoBase, carPos);
		// rotación base -90 para orientar el modelo + giro de la máquina de estados (carYaw)
		autoBase = glm::rotate(autoBase, glm::radians(carYaw), glm::vec3(0.0f, 1.0f, 0.0f));
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
        // usar rotllanta calculada por la animación para que las llantas giren mientras el coche se mueve
		float giroLlanta = rotllanta;

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
		// la rueda está espejada en Y, invertir signo de giro para que rote en la misma dirección
		llanta2Base = glm::rotate(llanta2Base, glm::radians(-giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
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
		// la rueda está espejada en Y, invertir signo de giro para que rote en la misma dirección
		llanta4Base = glm::rotate(llanta4Base, glm::radians(-giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 llanta4Model = llanta4Base;
		llanta4Model = glm::translate(llanta4Model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(llanta4Model));
		Auto_Llanta4.RenderModel();



        // Aquí va la nave con jerarquía de modelos y animación cíclica
		// Usar las fases y estados ya actualizados arriba para obtener movimiento y ángulo del ala
		float naveBob = naveBobAmp * sin(naveBobPhase);
		float alaFlapAngle = alaFlapAmp * sin(alaFlapPhase); // grados

		// Construir la matriz jerárquica de la nave
		float naveScale = 4.0f; // factor de escala
		glm::mat4 naveBase = glm::mat4(1.0f);
		// posición: base Y = 3.0 + bob, Z = 1.5 + naveMov
		naveBase = glm::translate(naveBase, glm::vec3(0.0f, 3.0f + naveBob, 1.5f + naveMov));
		naveBase = rotate(naveBase, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// aplicar la guiñada
		naveBase = glm::rotate(naveBase, glm::radians(naveYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		// aplicar escala
		naveBase = glm::scale(naveBase, glm::vec3(naveScale, naveScale, naveScale));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(naveBase));
		Nave_M.RenderModel();

		// Ala: hereda la matriz de la nave y aplica un aleteo (rotación local)
		glm::mat4 alaBase = naveBase;
		// desplazar al punto local donde está el ala (coordenadas que ya usabas)
		alaBase = glm::translate(alaBase, glm::vec3(0.0f, 0.0f, -0.2f));
		// aplicar el aleteo (rotación alrededor del eje X local)
		alaBase = glm::rotate(alaBase, glm::radians(alaFlapAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(alaBase));
		Ala_M.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
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
