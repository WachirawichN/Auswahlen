#shader vertex
#version 460 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vTexCoord;

void main()
{
   gl_Position = projection * view * model * position;
   vTexCoord = texCoord;
};


#shader fragment
#version 460 core

layout(location = 0)out vec4 color;

in vec2 vTexCoord;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main()
{
   vec4 texColor = texture(uTexture, vTexCoord);
   color = texColor;
};