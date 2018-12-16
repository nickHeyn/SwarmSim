#version 150 core

in vec3 Color;
in vec3 vertNormal;
in vec3 pos;
in vec3 lightDir1;
in vec3 lightDir2;
in vec2 texcoord;

out vec4 outColor;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform int texID;

const float ambient = .3;
void main() {
  vec3 color;
  if (texID < 0)
    color = Color;
  else if (texID == 0)
    color = texture(tex0, texcoord).rgb;
  else if (texID == 1)
    color = texture(tex1, texcoord).rgb;
  else{
    outColor = vec4(1,0,0,1);
    return; //This was an error, stop lighting!
  }
  vec3 normal = normalize(vertNormal);
  vec3 diffuseC = color*max(dot(-lightDir1,normal),0.0);
  diffuseC = diffuseC + color*max(dot(-lightDir2,normal),0.0);
  vec3 ambC = color*ambient;
  vec3 oColor = ambC+diffuseC;
  if(texID != -2)
	outColor = vec4(oColor,1);
  else
	outColor = vec4(oColor, .4);
}