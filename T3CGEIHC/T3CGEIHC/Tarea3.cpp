// Tarea 3: Circunferencia por Bresenham
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <set>
#include <iostream>

#include <glew.h>
#include <glfw3.h>

// glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

// clases del laboratorio
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

//
// CONFIGURACIÓN GLOBAL
const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader> shaderList;

// shaders del laboratorio
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

// índices de meshes
int idxCircle = -1;
int idxAxes = -1;

// datos de entrada
int gCenterX = 0;
int gCenterY = 0;
int gRadius = 0;

// tamaño visual de cada “pixel” dibujado como cuadrito
float gPixelSize = 0.2f;

// FUNCIONES AUXILIARES

// Agrega un cuadrito (2 triángulos) con color RGB
static void addCellAs2TrisRGB(std::vector<GLfloat>& v,
    float x, float y, float s,
    float r, float g, float b)
{
    float x2 = x + s;
    float y2 = y + s;

    // Triángulo 1
    v.push_back(x);  v.push_back(y);  v.push_back(0.0f);
    v.push_back(r);  v.push_back(g);  v.push_back(b);

    v.push_back(x2); v.push_back(y);  v.push_back(0.0f);
    v.push_back(r);  v.push_back(g);  v.push_back(b);

    v.push_back(x2); v.push_back(y2); v.push_back(0.0f);
    v.push_back(r);  v.push_back(g);  v.push_back(b);

    // Triángulo 2
    v.push_back(x);  v.push_back(y);  v.push_back(0.0f);
    v.push_back(r);  v.push_back(g);  v.push_back(b);

    v.push_back(x2); v.push_back(y2); v.push_back(0.0f);
    v.push_back(r);  v.push_back(g);  v.push_back(b);

    v.push_back(x);  v.push_back(y2); v.push_back(0.0f);
    v.push_back(r);  v.push_back(g);  v.push_back(b);
}

// Construye un MeshColor desde vector dinámico
static MeshColor* BuildMeshColorFromVector(const std::vector<GLfloat>& vertices)
{
    MeshColor* obj = new MeshColor();
    obj->CreateMeshColor((GLfloat*)&vertices[0], (unsigned int)vertices.size());
    return obj;
}

// Agrega un “pixel” centrado aproximadamente en coordenadas enteras
static void addPixel(std::vector<GLfloat>& vertices,
    float px, float py, float pixelSize,
    float r, float g, float b)
{
    float half = pixelSize * 0.5f;
    addCellAs2TrisRGB(vertices, px - half, py - half, pixelSize, r, g, b);
}

// Agrega los 8 puntos simétricos de Bresenham evitando duplicados
static void addSymmetricPoints(std::set<std::pair<int, int>>& points,
    int xc, int yc, int x, int y)
{
    points.insert({ xc + x, yc + y });
    points.insert({ xc - x, yc + y });
    points.insert({ xc + x, yc - y });
    points.insert({ xc - x, yc - y });

    points.insert({ xc + y, yc + x });
    points.insert({ xc - y, yc + x });
    points.insert({ xc + y, yc - x });
    points.insert({ xc - y, yc - x });
}

// BRESENHAM CÍRCULO
void CrearCircunferenciaBresenham(int xc, int yc, int r, float pixelSize)
{
    std::set<std::pair<int, int>> puntos;
    std::vector<GLfloat> vertices;

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y)
    {
        addSymmetricPoints(puntos, xc, yc, x, y);

        if (d < 0)
            d = d + 4 * x + 6;
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    // Color azul para la circunferencia
    for (auto& p : puntos)
    {
        addPixel(vertices, (float)p.first, (float)p.second, pixelSize, 0.0f, 0.0f, 1.0f);
    }

    MeshColor* circle = BuildMeshColorFromVector(vertices);
    idxCircle = (int)meshColorList.size();
    meshColorList.push_back(circle);
}


// EJES CARTESIANOS
void CrearEjes(float minX, float maxX, float minY, float maxY, float pixelSize)
{
    std::vector<GLfloat> vertices;

    // Eje X en rojo
    for (int x = (int)floor(minX); x <= (int)ceil(maxX); x++)
    {
        addPixel(vertices, (float)x, 0.0f, pixelSize, 1.0f, 0.0f, 0.0f);
    }

    // Eje Y en verde
    for (int y = (int)floor(minY); y <= (int)ceil(maxY); y++)
    {
        addPixel(vertices, 0.0f, (float)y, pixelSize, 0.0f, 0.7f, 0.0f);
    }

    MeshColor* axes = BuildMeshColorFromVector(vertices);
    idxAxes = (int)meshColorList.size();
    meshColorList.push_back(axes);
}


// SHADERS
void CreateShaders()
{
    Shader* shaderColor = new Shader();
    shaderColor->CreateFromFiles(vShaderColor, fShaderColor);
    shaderList.push_back(*shaderColor);
}

// MAIN
int main()
{
    // ENTRADA Y VALIDACIÓN
    std::cout << "==== Circunferencia por Bresenham ====\n";
    std::cout << "Ingrese coordenada X del centro: ";
    std::cin >> gCenterX;

    std::cout << "Ingrese coordenada Y del centro: ";
    std::cin >> gCenterY;

    std::cout << "Ingrese radio entero (>0): ";
    std::cin >> gRadius;

    if (std::cin.fail() || gRadius <= 0)
    {
        std::cout << "Error: el radio debe ser un entero mayor que 0.\n";
        return -1;
    }

    // VENTANA
    mainWindow = Window(800, 800);
    mainWindow.Initialise();

    //
    // AJUSTE PARA QUE LA CIRCUNFERENCIA OCUPE ~ 3/4 DE VENTANA
    //
    // diámetro = 2r
    // si queremos que sea 3/4 del alto visible:
    // visibleSize = (2r) / 0.75 = 8r/3
    // halfRange = 4r/3
    //
    // además agregamos pequeño margen extra
    //
    float halfRange = (4.0f * gRadius) / 3.0f + 2.0f;

    float minX = (float)gCenterX - halfRange;
    float maxX = (float)gCenterX + halfRange;
    float minY = (float)gCenterY - halfRange;
    float maxY = (float)gCenterY + halfRange;

    // tamaño del “pixel” relativo al rango visible
    gPixelSize = std::max(0.08f, halfRange / 80.0f);

    // Proyección ortográfica ajustada al tamaño de la circunferencia
    glm::mat4 projection = glm::ortho(minX, maxX, minY, maxY, 0.1f, 100.0f);

    // crear geometría
    CrearCircunferenciaBresenham(gCenterX, gCenterY, gRadius, gPixelSize);
    CrearEjes(minX, maxX, minY, maxY, gPixelSize);
    CreateShaders();

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    glm::mat4 model(1.0f);

    // LOOP DE RENDER
    while (!mainWindow.getShouldClose())
    {
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Colocar todo al plano visible
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        // Ejes
        if (idxAxes >= 0)
            meshColorList[idxAxes]->RenderMeshColor();

        // Circunferencia
        if (idxCircle >= 0)
            meshColorList[idxCircle]->RenderMeshColor();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}