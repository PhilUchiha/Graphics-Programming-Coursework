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
	Shader fogShader();
	Shader toonShader();
	Shader rimShader();
	Shader geoShader();
	Shader reflect();
	Shader ADSLighting();
	//Audio* audioDevice();
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
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\planet_obj.obj");
	mesh3.loadModel("..\\res\\Grass_block.obj");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
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
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
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


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void MainGame::linkFogShader()
{
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToon()
{
	toonShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkGeo()
{
	//float randX = ((float)rand() / (RAND_MAX));
	//float randY = ((float)rand() / (RAND_MAX));
	//float randZ = ((float)rand() / (RAND_MAX));
	// Frag: uniform float randColourX; uniform float randColourY; uniform float randColourZ;
	//geoShader.setFloat("randColourX", randX);
	//geoShader.setFloat("randColourY", randY);
	//geoShader.setFloat("randColourZ", randZ);
	// Geom: uniform float time;
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

void MainGame::linkRimLighting()
{
	glm::vec3 camDir;
	camDir = mesh2.getSpherePos() - myCamera.getPos();
	camDir = glm::normalize(camDir);
	rimShader.setMat4("u_pm", myCamera.getProjection());
	rimShader.setMat4("u_vm", myCamera.getView());
	rimShader.setMat4("model", transform.GetModel());
	rimShader.setMat4("view", myCamera.getView());
	rimShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::LinkADSLighting()
{
	ADSLighting.setMat4("projection", myCamera.getProjection());
	ADSLighting.setMat4("view", myCamera.getView());
	ADSLighting.setMat4("model", transform.GetModel());
	ADSLighting.setVec3("lightColour", glm::vec3(0.2f, 0.5f, 0.8f)); //(1.0, 1.0f, 1.0f) for white
	ADSLighting.setVec3("modelColour", glm::vec3(0.3f, 0.9f, 0.7f));
	ADSLighting.setVec3("lightPosition", glm::vec3(-1.0f, -1.0f, -2.0f));
	ADSLighting.setVec3("cameraPosition", myCamera.getPos());
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour

	//linkFogShader();
	//linkToon();
	//linkRimLighting();

	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture

	transform.SetPos(glm::vec3(-1.5, (-sinf(counter) + 0.0), -0.7));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.015, 0.015, 0.015));

	reflect.Bind();
	linkReflect();
	texture1.Bind(0);
	reflect.Update(transform, myCamera);
	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.62f);


	transform.SetPos(glm::vec3(1.5, (-sinf(counter) + 0.4), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	geoShader.Bind();
	linkGeo();
	texture.Bind(0);
	geoShader.Update(transform, myCamera);
	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.62f);


	transform.SetPos(glm::vec3(0.0, (-sinf(counter)-1.25), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

	ADSLighting.Bind();
	LinkADSLighting();
	texture1.Bind(0);
	toonShader.Update(transform, myCamera);
	mesh3.draw();
	mesh3.updateSphereData(*transform.GetPos(), 0.62f);

	counter = counter + 0.02f;

	skybox.draw(&myCamera);


	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();


	_gameDisplay.swapBuffer();



	

} 
