 #version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 worldToView;
uniform bool useAlpha = true;

out vec4 vColor;

void main(void)
{
    gl_Position = worldToView * vec4(position, 1.0);
    vColor = color;
    if(useAlpha == false)
        vColor.a = 1;
}
