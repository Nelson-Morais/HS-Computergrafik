#version 400

layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord0;
//Exercise 3
layout(location=3) in vec2 VertexTexcoord1;
//zusatzaufgabe 5
layout(location=4) in vec2 VertexTexcoord2;
layout(location=5) in vec2 VertexTexcoord3;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord0;
//Exercise 3
out vec2 Texcoord1;
//zusatzaufgabe 5
out vec2 Texcoord2;
out vec2 Texcoord3;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;
uniform vec3 Scaling;

void main()
{
   // Exercise 1
    vec4 newVertexPosition = VertexPos;
    newVertexPosition.x *= Scaling.x;
    newVertexPosition.y *= Scaling.y;
    newVertexPosition.z *= Scaling.z;

    vec4 newVertexNormal = VertexNormal;
    newVertexNormal.x /= Scaling.x;
    newVertexNormal.y /= Scaling.y;
    newVertexNormal.z /= Scaling.z;

    newVertexNormal = normalize(newVertexNormal);

    Position = (ModelMat * newVertexPosition).xyz;
    Normal = (ModelMat * vec4(newVertexNormal.xyz,0)).xyz;
    Texcoord0 = VertexTexcoord0;
    //Exercise 3
    Texcoord1 = VertexTexcoord1;
    //zusatzaufgabe 5
    Texcoord2 = VertexTexcoord2;
    Texcoord3 = VertexTexcoord3;
    
    gl_Position = ModelViewProjMat * newVertexPosition; 
}
