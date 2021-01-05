#version 400
uniform vec3 EyePos;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform sampler2D MixTex; // for exercise 3
uniform sampler2D DetailTex[4]; // for exercise 3 // 4 Plätze für Zusatzaufgabe 5
uniform vec3 Scaling;

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord0;
//Exercise 3
in vec2 Texcoord1;

out vec4 FragColor;

//zusatzaufgabe 4
float sat( in float a)
{
    float dmin = 0;
    float dmax = 50;
    
    return clamp(pow((length(EyePos-Position)-dmin)/dmax-dmin,a), 0.0, 1.0);
}


void main()
{
    vec3 N      = normalize(Normal);
    vec3 L      = normalize(LightPos); // light is treated as directional source
    vec3 D      = EyePos-Position;
    float Dist  = length(D);
    vec3 E      = D/Dist;
    vec3 R      = reflect(-L,N);
    
    vec3 DiffuseComponent = LightColor * DiffuseColor * sat(dot(N,L));
    vec3 SpecularComponent = LightColor * SpecularColor * pow( sat(dot(R,E)), SpecularExp);

   
    // Exercise 3
    vec4 color;
    vec4 MixTextureVec = texture(MixTex,Texcoord0).rgba; 
    //Textures
    vec4 Texel_GrasTexture = texture(DetailTex[0], Texcoord1).rgba;
    vec4 Texel_RockTexture = texture(DetailTex[1], Texcoord1).rgba;
    vec4 Texel_SandTexture = texture(DetailTex[2], Texcoord1).rgba;
    vec4 Texel_SnowTexture = texture(DetailTex[3], Texcoord1).rgba;

    //if() anhand höhe zwischen gras und schnee
    
   color = mix(Texel_GrasTexture,Texel_RockTexture,MixTextureVec);
   
   //zusatzaufgabe 5
   /*
   if(Position.y < 1) {
    color = Texel_SandTexture;
    }
    else if(Position.y >= 1 && Position.y < 3){
    color = mix(Texel_SandTexture,Texel_GrasTexture,MixTextureVec);
    }
    else if( Position.y >= 3 && Position.y < 7){
    color = Texel_GrasTexture;
    }
    else if (Position.y >= 7 && Position.y < 10) {
    color = mix(Texel_GrasTexture,Texel_RockTexture,MixTextureVec);
    }
    else if ( Position.y >= 10 && Position.y < 13){
    color = Texel_RockTexture;
    }
    else if ( Position.y >= 13 && Position.y < 15){
    color = mix(Texel_RockTexture,Texel_SnowTexture,MixTextureVec);
    }
    else if ( Position.y >= 15){
    color = Texel_SnowTexture;
    }*/
      
   // color *= vec4(((DiffuseComponent + AmbientColor) + SpecularComponent),1);
  
    
    //Zusatzaufgabe 1
    float s = sat(3);
    vec3 colorFogCalc = vec3(0.95,0.95,1);
    vec4 colorFog = vec4(colorFogCalc.rgb, 1);
    vec4 newColorFog = (1.f-s)*color+(s*colorFog);    


    FragColor = vec4(((DiffuseComponent + AmbientColor) + SpecularComponent),1) * newColorFog;

    


    


}
