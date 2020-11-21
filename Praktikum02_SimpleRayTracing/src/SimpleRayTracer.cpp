//
//  SimpleRayTracer.cpp
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 03.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "SimpleRayTracer.h"
#include "rgbimage.h"
#include <stdio.h>
#include <math.h>



Camera::Camera(float zvalue, float planedist, float width, float height, unsigned int widthInPixel, unsigned int heightInPixel)
	:zvalue(zvalue), planedist(planedist), width(width), height(height), widthInPixel(widthInPixel), heigthInPixel(heightInPixel)
{

}


Vector Camera::generateRay(unsigned int x, unsigned int y) const
{
	
	float widthRaumEinheit = this->width / this->widthInPixel;
	float heightRaumEinheit = this->height / this->heigthInPixel;


	float tmpX = x - (float)(widthInPixel/2);
	float tmpY = y- (float)(heigthInPixel/2);

	float xInRaum = tmpX * widthRaumEinheit +widthRaumEinheit/2;
	float yInRaum = -(tmpY * heightRaumEinheit + widthRaumEinheit / 2);

	Vector richtungsVector(xInRaum, yInRaum, (planedist));

	return richtungsVector.normalize();
}

Vector Camera::Position() const
{
	return Vector(0, 0, zvalue);
}

unsigned int Camera::getWidthInPixel() {
	return this->widthInPixel;
}
unsigned int Camera::getHeightInPixel() {
	return this->heigthInPixel;
}

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth) :maxDepth(MaxDepth)
{

}


void SimpleRayTracer::traceScene(const Scene& SceneModel, RGBImage& Image)
{
	Camera cam(-8, 1, 1, 0.75, 640, 480);


	//1,2,3
	for (int y = 0; y < cam.getHeightInPixel(); y++) {
		for (int x = 0; x < cam.getWidthInPixel(); x++) {
			Image.setPixelColor(x, y, Color(0, 0, 0));
			Image.setPixelColor(x, y, trace(SceneModel, cam.Position(), cam.generateRay(x, y), 0));
		}
	}
}

Color SimpleRayTracer::localIllumination(const Vector& Surface, const Vector& Eye, const Vector& N, const PointLight& Light, const Material& Mtrl)
{	

	//L Direction Vektor von Surface auf Licht quelle
	Vector L = Light.Position - Surface;
	//E Direction Vektor Eye to surface
	Vector E = Eye - Surface;
	

	float NdotL = N.dot(L.normalize());
	if (NdotL < 0) NdotL = 0;


	Vector R = N * 2 * (N.dot(L)) - L;
	float SurfacedotR = E.normalize().dot(R);
	if (SurfacedotR < 0) SurfacedotR = 0;

	//D = Diffuse Component, S = Specular Component
	Color D = Light.Intensity * Mtrl.getDiffuseCoeff(Surface) * NdotL;
	Color S = Light.Intensity * Mtrl.getSpecularCoeff(Surface) * std::pow(SurfacedotR, Mtrl.getSpecularExp(Surface));

	return D+S;
}

Color SimpleRayTracer::trace(const Scene& SceneModel, const Vector& o, const Vector& d, int depth)
{
	float t = 0;
	float s = INFINITY;
	Triangle triangle;
	Color color;

	//4 Brechnen nähste triangle
	for (int i = 0; i < SceneModel.getTriangleCount();i++) {
		if (o.triangleIntersection(d, SceneModel.getTriangle(i).A, SceneModel.getTriangle(i).B, SceneModel.getTriangle(i).C, t)) {
			if (t <= 1e-4 || t > s){
				continue;
			}
				s=t;
				triangle = SceneModel.getTriangle(i);
		}
	}
	if (s == INFINITY) {
		return color;
	}
	//4 Berechnen auftreffpunk
	Vector p = o + (d * s);

	//5 Prüfe, ob von Punkt p Sichtverbindung zur Lichtquelle besteht
	for (int i = 0; i < SceneModel.getLightCount(); i++) {
		color += localIllumination(p, o, triangle.calcNormal(p), SceneModel.getLight(i), *triangle.pMtrl);
	}
	color += triangle.pMtrl->getAmbientCoeff(p);

	//6 Berechne Reflexionsstrahl r von s im Punkt p;
	Vector r = d.reflection(triangle.calcNormal(p));


	//7. F = F + Raytracing(r) * Reflexionskoeffizient;
	if (depth < maxDepth) {
		color += trace(SceneModel, p, r, ++depth) * triangle.pMtrl->getReflectivity(p);
	}

	return color;
}

