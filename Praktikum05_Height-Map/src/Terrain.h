#ifndef Terrain_hpp
#define Terrain_hpp

#include <stdio.h>
#include "basemodel.h"
#include "texture.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class Terrain : public BaseModel
{
public:
    Terrain(const char* HeightMap=NULL, const char* DetailMap1=NULL, const char* DetailMap2=NULL, const char* DetailMap3 = NULL ,const char* DetailMap4 = NULL ,const char* MixTex=NULL);
    virtual ~Terrain();
    bool load( const char* HeightMap, const char* DetailMap1, const char* DetailMap2, const char* DetailMap3, const char* DetailMap4, const char* MixTex);

    virtual void shader( BaseShader* shader, bool deleteOnDestruction );
    virtual void draw(const BaseCamera& Cam);
    
    float width() const { return Size.X; }
    float height() const { return Size.Y; }
    float depth() const { return Size.Z; }

    void width(float v) { Size.X = v; }
    void height(float v) { Size.Y = v; }
    void depth(float v) { Size.Z = v; }

    Vector normal(const Vector& , const Vector& , const Vector& );
    
    const Vector& size() const { return Size; }
    void size(const Vector& s) { Size = s; }
protected:
    void applyShaderParameter();
    
    VertexBuffer VB;
    IndexBuffer IB;
    Texture DetailTex[4];
    Texture MixTex;
    Texture HeightTex;
    Vector Size;
    
};



#endif /* Terrain_hpp */
