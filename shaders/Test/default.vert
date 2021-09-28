 #version 330

layout(location = 0) in vec2 position;

uniform mat4 worldToView;

void main(void)
{
    gl_Position = vec4(position, 0.0, 1.0);
}
