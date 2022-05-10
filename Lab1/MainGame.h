#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "SkyBox.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void linkGeo();
	void linkReflect();
	void LinkADSLighting();

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Camera myCamera;
	Shader geoShader;
	Shader shaderSkybox;
	Shader reflect;
	Shader ADSLighting;

	Skybox skybox;

	vector<std::string> faces;

	float counter;
};

