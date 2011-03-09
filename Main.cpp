
#include "Framework.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "InputListener.h"
#include "Framebuffer.h"

#include <cmath>

using namespace std;

// Note: See the SMFL documentation for info on setting up fullscreen mode
// and using rendering settings
// http://www.sfml-dev.org/tutorials/1.6/window-window.php
sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(800, 600), "CS248 Rules!", sf::Style::Close, settings);
 
// This is a clock you can use to control animation.  For more info, see:
// http://www.sfml-dev.org/tutorials/1.6/window-time.php
sf::Clock clck;

// This creates an asset importer using the Open Asset Import library.
// It automatically manages resources for you, and frees them when the program
// exits.
Assimp::Importer importer;

//TODO: put this somewhere else
Model spaceship;

Shader *phongShader, *normalShader, *toonShader;

Camera camera(
		aiVector3D(0.0, 0.0, 50.0),
		aiMatrix3x3(
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, -1.0));

vector<InputListener*> inputListeners;

Framebuffer *normalsBuffer = NULL;

void initOpenGL();
void loadAssets();
void handleInput();
void renderFrame();

int main(int argc, char** argv) {

    initOpenGL();
    loadAssets();

		inputListeners.push_back(&camera);

		normalsBuffer = new Framebuffer(window.GetWidth(), window.GetHeight());

    // Put your game loop here (i.e., render with OpenGL, update animation)
    while (window.IsOpened()) {

        handleInput();
        renderFrame();
        window.Display();
    }

		delete phongShader;
		delete normalShader;
		delete toonShader;

		delete normalsBuffer;

    return 0;
}



void initOpenGL() {
    // Initialize GLEW on Windows, to make sure that OpenGL 2.0 is loaded
#ifdef FRAMEWORK_USE_GLEW
    GLint error = glewInit();
    if (GLEW_OK != error) {
        std::cerr << glewGetErrorString(error) << std::endl;
        exit(-1);
    }
    if (!GLEW_VERSION_2_0 || !GL_EXT_framebuffer_object) {
        std::cerr << "This program requires OpenGL 2.0 and FBOs" << std::endl;
        exit(-1);
    }
#endif

    // This initializes OpenGL with some common defaults.  More info here:
    // http://www.sfml-dev.org/tutorials/1.6/window-opengl.php
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
}




void loadAssets() {
	phongShader = new Shader("shaders/phong");
	normalShader = new Shader("shaders/normal");
	toonShader = new Shader("shaders/toon");

	spaceship.loadFromFile("models/ship", "space_frigate_0.3DS", importer);
	aiMatrix4x4 rot;
	aiMatrix4x4::RotationX(-M_PI / 2.0, rot);
	spaceship.setTransformation(rot);
}




void handleInput() {
    //////////////////////////////////////////////////////////////////////////
    // TODO: ADD YOUR INPUT HANDLING HERE. 
    //////////////////////////////////////////////////////////////////////////

    // Event loop, for processing user input, etc.  For more info, see:
    // http://www.sfml-dev.org/tutorials/1.6/window-events.php
    sf::Event evt;
    while (window.GetEvent(evt)) {
        switch (evt.Type) {
        case sf::Event::Closed: 
            // Close the window.  This will cause the game loop to exit,
            // because the IsOpened() function will no longer return true.
            window.Close(); 
            break;
        case sf::Event::Resized: 
            // If the window is resized, then we need to change the perspective
            // transformation and viewport
            glViewport(0, 0, evt.Size.Width, evt.Size.Height);
            break;
        default: 
						for (unsigned i = 0; i < inputListeners.size(); i++)
							inputListeners[i]->handleEvent(evt);
            break;
        }
    }
}


void setupLights()
{
	GLfloat pos[] = { -4.0, 0.0, 1.0, 0.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambient[] = { 0.3, 0.3, 0.3, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}


void renderFrame() {
	camera.setProjectionAndView((float)window.GetWidth()/window.GetHeight());

	normalsBuffer->bind();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spaceship.useShader(normalShader);
	spaceship.render(NORMALS_PASS, normalsBuffer);

	normalsBuffer->unbind();

	glClearColor(0.0, 0.0, 0.15, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupLights();

	spaceship.useShader(toonShader);
	spaceship.render(FINAL_PASS, normalsBuffer);
}
