#include "Physics/PhysicsScene.h"
#include "Physics/sphere.h"
#include "Physics/plane.h"
#include "Physics/SpringJoint.h"
#include "Gizmos.h"
#include "ImguiImpl.h"
#include "glm/glm.hpp"
#include <iostream>

using namespace glm;

#define GRAVITY -9.80665f

const int windowWidth = 768;
const int windowHeight = 768;

float beginTime;
float endTime;
float deltaTime = 1.0f / 60.0f;

GLFWwindow* window;

bool Initialize();
void Shutdown();
void SetupScene(PhysicsScene& scene);
void EvaluateInput(PhysicsScene& scene);

int main()
{
	Initialize();

	PhysicsScene scene;
	SetupScene(scene);

	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Gizmos::clear();
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Scene Information");
		ImGui::Value("Actor Count", PhysicsActor::actorCount);
		ImGui::End();

		EvaluateInput(scene);

		// Update & Draw physics actors
		scene.Update(scene.gravity, .1f);
		scene.Render();

		// Calculating deltaTime
		endTime = (float)glfwGetTime();
		deltaTime = (float)(endTime - beginTime);
		beginTime = endTime;
		// If the deltaTime is too large we can assume we continued execution from a breakpoint
		// and reset the deltaTime back to a optimal scenario of 16ms
		if (deltaTime > 1.0f)
			deltaTime = 1.0f / 60.0f;

		glfwSwapBuffers(window);
	}

	Shutdown();

	return 0;
}

void EvaluateInput(PhysicsScene& scene)
{
	static bool mouseRepeat;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
		mouseRepeat = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		if (!mouseRepeat)
		{
			mouseRepeat = true;
											  
			double x, y;					  
			glfwGetCursorPos(window, &x, &y); 

			std::cout << "Dropping ball at X: " << x << std::endl;

			//Place new ball
			vec3 mPos = glm::unProject(vec3(x, y, 1), mat4(1), glm::ortho<float>(-100, 100, -100 / (16 / 9), 100 / (16 / 9), -1.0f, 1.0f), vec4(0, y - windowHeight, windowWidth, windowHeight));

			Sphere* ball = new Sphere(vec2(mPos.x, mPos.y), vec2(0, 0), 3.0f, 10, vec4(0.5, 0.7, 1.0f, 1.0));
			scene.AddActor(ball);
		}
	}
}

void SetupScene(PhysicsScene& scene)
{
	scene.gravity = vec2(0.0f, GRAVITY);

	// Spring Actors
	Sphere* con1 = new Sphere(vec2(-20, 30), vec2(0, 0), 3.0f, 4, vec4(0.5, 0.7, 1.0f, 1.0));
	con1->isKinematic = true;
	scene.AddActor(con1);

	Sphere* con2 = new Sphere(vec2(-10, 30), vec2(0, 0), 3.0f, 4, vec4(0.5, 0.7, 1.0f, 1.0));
	scene.AddActor(con2);

	Sphere* con3 = new Sphere(vec2(10, 30), vec2(0, 0), 3.0f, 4, vec4(0.5, 0.7, 1.0f, 1.0));
	scene.AddActor(con3);

	Sphere* con4 = new Sphere(vec2(20, 30), vec2(0, 0), 3.0f, 4, vec4(0.5, 0.7, 1.0f, 1.0));
	con4->isKinematic = true;
	scene.AddActor(con4);

	SpringJoint* joint = new SpringJoint(con1, con2, 3.f, 0.2f);
	scene.AddActor(joint);

	SpringJoint* joint2 = new SpringJoint(con2, con3, 3.f, 0.2f);
	scene.AddActor(joint2);

	SpringJoint* joint3 = new SpringJoint(con3, con4, 3.f, 0.2f);
	scene.AddActor(joint3);

	// Generic Actors
	Sphere* ball3 = new Sphere(vec2(40, 0), vec2(0, 0), 3.0f, 10, vec4(0.5, 0.7, 1.0f, 1.0));
	scene.AddActor(ball3);

	Sphere* ball4 = new Sphere(vec2(-40, 0), vec2(0, 0), 3.0f, 10, vec4(0.5, 0.7, 1.0f, 1.0));
	scene.AddActor(ball4);

	Sphere* ball5 = new Sphere(vec2(-60, 100), vec2(0, 0), 3.0f, 10, vec4(0.5, 0.7, 1.0f, 1.0));
	scene.AddActor(ball5);

	Sphere* ball6 = new Sphere(vec2(60, 100), vec2(0, 0), 3.0f, 10, vec4(0.5, 0.7, 1.0f, 1.0));
	scene.AddActor(ball6);

	Plane* plane = new Plane(glm::normalize(vec2(0.25f, 0.75f)), -70);
	scene.AddActor(plane);

	Plane* plane2 = new Plane(glm::normalize(vec2(-0.25f, 0.75f)), -70);
	scene.AddActor(plane2);
}

bool Initialize()
{
	// Attempt to initialize GLFW
	if (!glfwInit())
		return false;

	// Window hints for window creation
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a new OpenGL window
	window = glfwCreateWindow(windowWidth, windowHeight, "DIY Physics Playground", nullptr, nullptr);

	// Terminate if glfw fails to create a window
	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	// Set the newly created window to the current openGL context
	glfwMakeContextCurrent(window);

	// Attempt to load openGL functions through loadgen
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwTerminate();
		return false;
	}

	Gizmos::create(0, 0, 10000, 10000);
	ImGui_ImplGlfwGL3_Init(window, true);

	return true;
}

void Shutdown()
{
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}