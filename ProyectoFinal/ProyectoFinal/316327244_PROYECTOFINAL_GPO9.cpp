/*Sosa Alvirde Pedro Angel
Laboratorio de Computacion Grafica e Interaccion Humano Computadora
Proyecto Final
Renderizado de una recamara y fachada en OpenGL */
 

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion(); 
///
///@fn void *animacion()
///@brief Realiza la animacion de la puerta
/// 

// Dimensiones de la ventana
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-50.0f, 0.0f, 20.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

//Variables para las animaciones sencillas.
float tiempo;
glm::vec3 PosIni(25.68f, -25.03f, 71.48f);


//Varaibles for controlling states animations.
float rotPuerta1 = 0.0f;
float rotPuerta2 = 0.0f;
float rotSillon = 0.0f;

int giro = 0;
int giro2 = 0;
int giro3 = 0;

bool abierto = false;
bool cerrado = false;
bool abierto2 = false;
bool cerrado2 = false;
bool abierto3 = false;
bool cerrado3 = false;

//Animacion compleja
bool movSol = false;
bool movSol2 = false;
float movSolY = 0.0;
float movSolZ = 0.0;

bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;


//VARAIBLES PARA ENCENDER LAS LUCES
// Light attributes
// Positions of the point lights

glm::vec3 pointLightPositions[] = {
	glm::vec3(29.68f, -25.03f, 71.48f),
	glm::vec3(5.24f, 2.281f, -1.93f),
	glm::vec3(5.31f, 2.15f, -1.3099f),
	glm::vec3(5.85f, 2.50f, -1.56f)
};
/// <summary>
/// Cada vector representa la ubicacion de cada PointLight
/// </summary>
glm::vec3 spotLightPositions[] = {
	glm::vec3(4.28f, 5.10f, -0.22f),
	glm::vec3(-2.33f, 5.13f, -0.19f),
	glm::vec3(-8.64f, 5.13f, -0.23f),
	glm::vec3(4.23f, 5.13f, 1.37f),
	glm::vec3(-2.31f, 5.13f, 1.17f),
	glm::vec3(-8.66f, 5.13f, 1.25f),
};
/// <summary>
/// Cada vector representa la ubicacion de cada SpotLight
/// </summary>


//Variables para encender y apagar la luz Pointlight y SpotLight
glm::vec3 ambientP = glm::vec3(0);
glm::vec3 diffuseP = glm::vec3(0);
glm::vec3 ambientS = glm::vec3(0);
glm::vec3 diffuseS = glm::vec3(0);


int luzPointLight = 0;
int luzSpotLight = 0;

bool encendidoP = false;
bool apagadoP = false;
bool encendidoS = false;
bool apagadoS = false;







// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	//Definimos los shaders que se ocuparan para renderizar modelos.
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");

	//Modelos que se renderizaran
	Model Asta((char*)"Models/Asta.obj");
	Model Antena((char*)"Models/Antena.obj");
	Model Bandera((char*)"Models/Bandera.obj");
	Model Cajon((char*)"Models/cajon.obj");
	Model Cama((char*)"Models/cama.obj");
	Model Casa((char*)"Models/Casa.obj");
	Model CasaTecho((char*)"Models/CasaTecho.obj");
	Model DVD((char*)"Models/DVD.obj");
	Model Escaleras((char*)"Models/escaleras.obj");
	Model Lampara((char*)"Models/Lampara.obj");
	Model Lamparas((char*)"Models/Lamparas.obj");
	Model Libro((char*)"Models/Libro.obj");
	Model Marcos((char*)"Models/Marcos.obj");
	Model MuebleM((char*)"Models/MuebleMorado.obj");
	Model MuebleC((char*)"Models/MueblesCafes.obj");
	Model ParedArnold((char*)"Models/ParedArnold.obj");
	Model ParedFachada((char*)"Models/ParedFachada.obj");
	Model Piso((char*)"Models/Piso.obj");
	Model Planta((char*)"Models/Planta.obj");
	Model Plano((char*)"Models/Plano.obj");
	Model Puerta((char*)"Models/Puerta.obj");
	Model PuertaFachada((char*)"Models/PuertaFachada.obj");
	Model Radio((char*)"Models/Radio.obj");
	Model Rotoplas((char*)"Models/Rotoplas.obj");
	Model Sillon((char*)"Models/Sillon.obj");
	Model Sol((char*)"Models/Sol.obj");
	Model Sky((char*)"Models/Skybox.obj");
	Model Tubos((char*)"Models/Tubos.obj");
	Model Ventanas((char*)"Models/Ventanas.obj");
	Model VentanaPuerta((char*)"Models/VentanaPuerta.obj");
	Model TV((char*)"Models/TV.obj");

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.01f, 300.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(0.717f, 0.886f, 0.921f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);




		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.30f, 0.30f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.01f, 0.01f, 0.03f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 2.0f, 2.0f, 0.8f);


		// Point light SOL


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y + movSolY, pointLightPositions[0].z + movSolZ);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1000.0f, 100.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.30f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.30f);

		// Point light LAMPARA DORADA1



		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), ambientP.x,ambientP.y,ambientP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), diffuseP.x, diffuseP.y, diffuseP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 2.0f);

		// Point light LAMPARA DORADA2

		

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), ambientP.x, ambientP.y, ambientP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), diffuseP.x, diffuseP.y, diffuseP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 1.8f);

		// Point light LAMPARA DORADA3
		

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), ambientP.x, ambientP.y, ambientP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), diffuseP.x, diffuseP.y, diffuseP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 10.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 1.8f);

		// SpotLight1


		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].position"), spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].direction"),0.0f, -1.0f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].ambient"), ambientS.x, ambientS.y, ambientS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].diffuse"),diffuseS.x, diffuseS.y, diffuseS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].quadratic"), 1.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// SpotLight2


		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].position"), spotLightPositions[1].x, spotLightPositions[1].y, spotLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].direction"), 0.1f, -1.0f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].ambient"), ambientS.x, ambientS.y, ambientS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].diffuse"), diffuseS.x, diffuseS.y, diffuseS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].quadratic"), 1.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// SpotLight3


		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].position"), spotLightPositions[2].x, spotLightPositions[2].y, spotLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].direction"), 0.0f, -1.0f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].ambient"), ambientS.x, ambientS.y, ambientS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].diffuse"), diffuseS.x, diffuseS.y, diffuseS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].quadratic"), 1.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// SpotLight4


		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].position"), spotLightPositions[3].x, spotLightPositions[3].y, spotLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].direction"), -0.05f, -1.0f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].ambient"), ambientS.x, ambientS.y, ambientS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].diffuse"), diffuseS.x, diffuseS.y, diffuseS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].quadratic"), 1.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].outerCutOff"), glm::cos(glm::radians(15.0f)));


		// SpotLight5


		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[4].position"), spotLightPositions[4].x, spotLightPositions[4].y, spotLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[4].direction"), 0.0f, -1.0f, -0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[4].ambient"), ambientS.x, ambientS.y, ambientS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[4].diffuse"), diffuseS.x, diffuseS.y, diffuseS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[4].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[4].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[4].quadratic"), 1.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[4].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[4].outerCutOff"), glm::cos(glm::radians(15.0f)));



		// SpotLight6


		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[5].position"), spotLightPositions[5].x, spotLightPositions[5].y, spotLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[5].direction"), -0.42f, -1.0f, -0.53f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[5].ambient"), ambientS.x, ambientS.y, ambientS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[5].diffuse"), diffuseS.x, diffuseS.y, diffuseS.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[5].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[5].linear"), 2.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[5].quadratic"), 2.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[5].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[5].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 15.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		

		//Carga de modelo 

		view = camera.GetViewMatrix();
		//ASTA
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Asta.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Antena
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Antena.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);


		//Antena
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Antena.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//Cajon
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cajon.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Cama
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cama.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Casa
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Casa.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//CasaTecho
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		CasaTecho.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//DVD
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		DVD.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//Escaleras
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Escaleras.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//Lampara
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Lampara.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//Lamparas
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Lamparas.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//Libro
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Libro.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Marcos
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Marcos.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//MuebleMorado
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MuebleM.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//MueblesCafes
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MuebleC.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//ParedArnold
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		ParedArnold.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//ParedFachada
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		ParedFachada.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Piso
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Piso.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Plano
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Plano.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Planta
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Planta.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Rotoplas
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Rotoplas.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Radio
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Radio.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//TechoTubos
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Tubos.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Skybox
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Sky.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//TV
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		TV.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);





		//ANIMACION A OBJETOS SENCILLAS
		//Puerta
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.135f, 0.0f, 3.0f));
		model = glm::rotate(model, glm::radians(rotPuerta1), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Puerta.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//PuertaFachada
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.445f, -0.1f, 36.205f));
		model = glm::rotate(model, glm::radians(rotPuerta2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PuertaFachada.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.45f, -0.1f, 36.213f));
		model = glm::rotate(model, glm::radians(rotPuerta2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "ColorAlpha"), 1.0, 1.0, 1.0, 0.5);
		VentanaPuerta.Draw(lightingShader);
		glDisable(GL_BLEND);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		//Sillon
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		model = glm::translate(model, glm::vec3(-5.348, -1.74f, 2.863f));
		model = glm::rotate(model, glm::radians(rotSillon), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sillon.Draw(lightingShader);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);


		
		//ANIMACION A OBJETOS COMPLEJAS

		//Sol
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "ColorAlpha"), 1.0, 1.0, 1.0, 0.9);
		model = glm::translate(model, PosIni + glm::vec3(0.0f, movSolY, movSolZ));
		model = glm::scale(model, glm::vec3(2.0f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sol.Draw(lightingShader);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glDisable(GL_BLEND);



		//Ventanas
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "ColorAlpha"), 1.0, 1.0, 1.0, 0.5);
		Ventanas.Draw(lightingShader);
		glDisable(GL_BLEND);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);


		glBindVertexArray(0);

		//Bandera
		Anim.Use();
		tiempo = glfwGetTime();
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		glBindVertexArray(0);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Bandera.Draw(Anim);

		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();


	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);

	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);

	}

	//Animacion compleja Sol

	if (keys[GLFW_KEY_B]) {
		movSol = true;
	}

	if (keys[GLFW_KEY_V]) {
		movSol = false;
		movSol2 = false;

	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	//Animacion Puerta1
	if (keys[GLFW_KEY_M]) {

		if (giro == 0) {
			abierto = true;
			cerrado = false;
		}
		if (giro == 1) {
			cerrado = true;
			abierto = false;
		}
	}
	//Animacion Puerta2
	if (keys[GLFW_KEY_K]) {

		if (giro2 == 0) {
			abierto2 = true;
			cerrado2 = false;
		}
		if (giro2 == 1) {
			cerrado2 = true;
			abierto2 = false;
		}
	}
	//Anmimacion Sillon
	if (keys[GLFW_KEY_L]) {

		if (giro3 == 0) {
			abierto3 = true;
			cerrado3 = false;
		}
		if (giro3 == 1) {
			cerrado3 = true;
			abierto3 = false;
		}
	}

	//Apagar luces PointLight 
	if (keys[GLFW_KEY_P]) {

		if (luzPointLight == 0) {
			encendidoP = true;
			apagadoP = false;
		}
		if (luzPointLight == 1) {
			apagadoP = true;
			encendidoP = false;
		}
	}

	//Apagar luces SpotLight
	if (keys[GLFW_KEY_O]) {

		if (luzSpotLight == 0) {
			encendidoS = true;
			apagadoS = false;
		}
		if (luzSpotLight == 1) {
			apagadoS = true;
			encendidoS = false;
		}
	}
}

void animacion()
{//Animacion Puerta 1

	if (abierto)
	{
		if (rotPuerta1 >= -90) {
			rotPuerta1 -= 0.5;
		}
		if (rotPuerta1 == -90) {
			giro = 1;

		}

	}

	if (cerrado) {
		if (rotPuerta1 <= 0) {
			rotPuerta1 += 0.5;
		}
		if (rotPuerta1 == 0) {
			giro = 0;
		}
	}
	//Animacion Puerta 2

	if (abierto2)
	{
		if (rotPuerta2 >= -90) {
			rotPuerta2 -= 0.5;
		}
		if (rotPuerta2 == -90) {
			giro2 = 1;

		}

	}

	if (cerrado2) {
		if (rotPuerta2 <= 0) {
			rotPuerta2 += 0.5;
		}
		if (rotPuerta2 == 0) {
			giro2 = 0;
		}
	}

	//Animacion Sillon
	if (abierto3)
	{
		if (rotSillon <= 110) {
			rotSillon += 0.5;
		}
		if (rotSillon == 110) {
			giro3 = 1;

		}

	}

	if (cerrado3) {
		if (rotSillon >= 0) {
			rotSillon -= 0.5;
		}
		if (rotSillon == 0) {
			giro3 = 0;
		}
	}
	//animacion compleja sol
	if (movSol) {
		if (recorrido1)
		{
			movSolY += 0.01f;
			movSolZ -= 0.005f;
			if (movSolY > 37.3)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			movSolY += 0.005f;
			movSolZ -= 0.01f;
			if (movSolZ < -51.78) {
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		if (recorrido3)
		{
			movSolY -= 0.005;
			movSolZ -= 0.01f;
			if (movSolZ < -73.5) {
				printf("%f", movSolY);
				recorrido3 = false;
				recorrido4 = true;

			}
		}
		if (recorrido4) {
			movSolY -= 0.01f;
			movSolZ -= 0.01f;
			if (movSolY < 0) {
				recorrido4 = false;
				recorrido1 = true;
				movSolY = 0;
				movSolZ = 0;


			}
		}
	}

	if (encendidoP)
	{
		ambientP = glm::vec3(0.3f, 0.2f, 0.001f);
		diffuseP = glm::vec3(1.0f, 1.0f, 0.0f);
		luzPointLight = 1;
	}

	if (apagadoP) {
		ambientP = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuseP = glm::vec3(0.0f, 0.0f, 0.0f);
		luzPointLight = 0; 
		
	}

	if (encendidoS)
	{
		ambientS = glm::vec3(0.8f,0.8f,10.0f);
		diffuseS = glm::vec3(10.0f, 10.0f, 00.0f);
		luzSpotLight = 1;

	}

	if (apagadoS) {
		ambientS = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuseS = glm::vec3(0.0f, 0.0f, 0.0f);
		luzSpotLight = 0;

	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}