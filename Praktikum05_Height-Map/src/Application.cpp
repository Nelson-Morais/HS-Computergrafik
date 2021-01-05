//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "terrainshader.h"
#include "rgbimage.h"


#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin)
{
    BaseModel* pModel;
    
    // create LineGrid model with constant color shader
    pModel = new LinePlaneModel(10, 10, 10, 10);
    ConstantShader* pConstShader = new ConstantShader();
	pConstShader->color( Color(1,0,0));
    pModel->shader(pConstShader, true);
    // add to render list
    Models.push_back( pModel );



     //Exercise 2
    /**/
    char* file = ASSET_DIRECTORY "heightmap.bmp";
    Texture t;
    t.load(file);
    const RGBImage* src = t.getRGBImage();

    RGBImage dst(src->width(), src->height());
    RGBImage::SobelFilter(dst, *src, 10.f);
    dst.saveToDisk(ASSET_DIRECTORY "my_mixmap_sobel.bmp");



    // Exercise 1
    /**/
    pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);
   
    pTerrain = new Terrain();
    TerrainShader* pTerrainShader = new TerrainShader(ASSET_DIRECTORY);
    pTerrainShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "grass2.bmp"));
    pTerrain->shader(pTerrainShader, true);
    pTerrain->load(ASSET_DIRECTORY "heightmap.bmp", ASSET_DIRECTORY"grass2.bmp", ASSET_DIRECTORY"rock2.jpg", ASSET_DIRECTORY"sand2.jpg", ASSET_DIRECTORY"snow2.jpg", ASSET_DIRECTORY "my_mixmap_sobel.bmp");
    pTerrain->width(10);
    pTerrain->depth(10);
    pTerrain->height(1);
    Models.push_back(pTerrain);


   
    
}
void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime)
{
    double newYPosition, newXPosition;
    glfwGetCursorPos(this->pWindow, &newXPosition, &newYPosition);
    // Exercise 1
    // TODO: Add keyboard & mouse input queries for terrain scaling ..
    if (glfwGetKey(this->pWindow, GLFW_KEY_S) == GLFW_PRESS) {

        double diffY = (newYPosition - this->oldYPosition) * 0.05;
        double diffX = (newXPosition - this->oldXPosition) * 0.05;

        //neu setzen 
        pTerrain->height(pTerrain->height() - diffY);
        pTerrain->width(pTerrain->width() + diffX);
        pTerrain->depth(pTerrain->depth() + diffX);

        this->oldYPosition = newYPosition;
        this->oldXPosition = newXPosition;
    }
    else {
        this->oldYPosition = newYPosition;
        this->oldXPosition = newXPosition;

        //nur ohne Drücken der s-Taste kann die Kamera bewegt werden
        Cam.update();
    }
}

void Application::draw()
{
    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. setup shaders and draw models
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
    
    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;
    
    Models.clear();
}