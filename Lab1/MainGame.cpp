#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;
Transform transform2;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Shader geoShader();
	Shader reflect();
	Shader ADSLighting();

}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\planet_obj.obj");
	mesh3.loadModel("..\\res\\Grass_block.obj");
	reflect.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	ADSLighting.init("..\\res\\ADSLighting.vert", "..\\res\\ADSLighting.frag");

	geoShader.initGeo();

	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.png",
		"..\\res\\skybox\\left.png",
		"..\\res\\skybox\\top.png",
		"..\\res\\skybox\\bottom.png",
		"..\\res\\skybox\\front.png",
		"..\\res\\skybox\\back.png"
	};

	skybox.init(faces);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}

void MainGame::linkGeo()
{
	geoShader.setFloat("time", counter);
}

void MainGame::linkReflect()
{
	reflect.setMat4("model", transform.GetModel());
	reflect.setMat4("view", myCamera.getView());
	reflect.setMat4("projection", myCamera.getProjection());
	reflect.setInt("skybox", 0);
	reflect.setVec3("cameraPos", myCamera.getPos());
}


void MainGame::LinkADSLighting()
{
	ADSLighting.setMat4("projection", myCamera.getProjection());
	ADSLighting.setMat4("view", myCamera.getView());
	ADSLighting.setMat4("model", transform.GetModel());
	ADSLighting.setVec3("lightColour", glm::vec3(0.2f, 0.5f, 0.8f)); //(1.0, 1.0f, 1.0f) for white
	ADSLighting.setVec3("modelColour", glm::vec3(0.3f, 0.9f, 0.7f));
	ADSLighting.setVec3("lightPos", glm::vec3(-1.0f, -1.0f, -2.0f));
	ADSLighting.setVec3("camPos", myCamera.getPos());
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour

	Texture texture("..\\res\\bricks.jpg"); //load texture
	// mesh 2 settings (Planet)
	transform.SetPos(glm::vec3(-1.5, (-sinf(counter) + 0.0), -0.7));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.015, 0.015, 0.015));

	reflect.Bind();
	linkReflect();
	reflect.Update(transform, myCamera);
	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.62f);

	// mesh 1 settings (Monkey)
	transform.SetPos(glm::vec3(1.5, (-sinf(counter) + 0.4), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	geoShader.Bind();
	linkGeo();
	texture.Bind(0);
	geoShader.Update(transform, myCamera);
	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.62f);

	// mesh 3 settings (Grass Block)
	transform.SetPos(glm::vec3(0.0, (-sinf(counter)-1.25), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

	ADSLighting.Bind();
	LinkADSLighting();
	ADSLighting.Update(transform, myCamera);
	mesh3.draw();
	mesh3.updateSphereData(*transform.GetPos(), 0.62f);

	counter = counter + 0.02f;

	skybox.draw(&myCamera);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();
} 
