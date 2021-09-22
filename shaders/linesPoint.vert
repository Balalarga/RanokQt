 #version 330

layout(location = 0) in vec3 position;

uniform mat4 worldToView;
uniform bool useAlpha = true;

out vec4 vColor;

void main(void)
{
    gl_Position = worldToView * vec4(position, 1.0);
    vColor = vec4(1.0, 0.2, 0.2, 1.0);
}
