#version 330

layout(location = 0) in vec3 position;

uniform mat4 worldToView;

void main(void)
{
    gl_PointSize = 8.0;
    gl_Position = worldToView * vec4(position, 1.0);
}
