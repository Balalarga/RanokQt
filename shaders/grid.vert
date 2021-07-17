#version 330



layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;


out vec4 gridColor;

uniform mat4 worldToView;

void main()
{
    gridColor = color;
    gl_Position = worldToView * vec4(position.x, position.y, position.z, 1.0);
}

