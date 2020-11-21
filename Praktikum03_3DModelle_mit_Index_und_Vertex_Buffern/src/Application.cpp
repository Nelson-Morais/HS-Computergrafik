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

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin)
{
    Matrix trans;
    BaseModel* pModel;
    ConstantShader* pConstShader;
    PhongShader* pPhongShader;
    
    // create LineGrid model with constant color shader
    pModel = new LinePlaneModel(10, 10, 10, 10);
    pConstShader = new ConstantShader();
	pConstShader->color( Color(1,1,1));
    pModel->shader(pConstShader, true);
    // add to render list
    Models.push_back( pModel );
     /*
    // create TrianglePlaneModel with phong shader and texture
    pModel = new TrianglePlaneModel(4, 4, 10, 10);
    pPhongShader = new PhongShader();
	pPhongShader->ambientColor(Color(0.2f,0.2f,0.2f));
	pPhongShader->diffuseColor(Color(1.0f,1.0f,1.0f));
	pPhongShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "colorchess.bmp"));
    pModel->shader(pPhongShader, true);
    Models.push_back( pModel );
   
    pModel = new TriangleSphereModel(1.0f);
	pPhongShader = new PhongShader();
	pPhongShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "earth.jpg"));
    pModel->shader(pPhongShader, true);
    trans.translation(0, 1.0f, 0);
    pModel->transform(trans);
    Models.push_back( pModel );
   
    // Exercise 1: LineBoxModel
     
    pModel = new LineBoxModel(2,3,4);
    pConstShader = new ConstantShader();
    pConstShader->color(Color(0,1,0));
    pModel->shader(pConstShader, true);
    Models.push_back(pModel);
    */
    
    // Exercise 2: TriangleBoxModel
    /*
    pModel = new TriangleBoxModel(4,4,4);
    pPhongShader = new PhongShader();
    pPhongShader->ambientColor(Color(0.2f,0.2f,0.2f));
    pPhongShader->diffuseColor(Color(1.0f,1.0f,1.0f));
    pPhongShader->specularColor(Color(1.0f,1.0f,1.0f));
    pPhongShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "smiley.png"));
    pModel->shader(pPhongShader, true);
    Models.push_back( pModel );
    */
    
    // Exercise 3: Model
  /*   */
    pModel = new Model(ASSET_DIRECTORY "lizard.dae");
    pPhongShader = new PhongShader();
    pModel->shader(pPhongShader, true);
    // add to render list
    Models.push_back( pModel );
   
    
}
void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Application::update()
{
    Cam.update();
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