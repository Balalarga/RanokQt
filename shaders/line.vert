 #version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 dPos;
layout(location = 2) in vec4 color;

uniform mat4 worldToView;
uniform vec3 voxSize;
uniform bool useAlpha = true;

out vec4 vColor;
out vec4 vdPos;

void main(void)
{
    gl_Position = worldToView * vec4(pos, 1.0);
    vdPos = worldToView * vec4(pos + dPos * voxSize * 2, 1.0);
    vColor = color;
    if(useAlpha == false)
        vColor.a = 1;
}
