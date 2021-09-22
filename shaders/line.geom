#version 330

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform vec3 voxSize;
uniform mat4 worldToView;

in vec4 vdPos[];
in vec4 vColor[];
out vec4 gColor;

void main()
{
    gColor = vColor[0];

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = vdPos[0];
    EmitVertex();

    EndPrimitive();
}
