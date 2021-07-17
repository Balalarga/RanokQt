#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

uniform vec3 voxSize;
uniform mat4 worldToView;

in vec4 vColor[];
out vec4 gColor;

void AddQuad(vec4 center, vec4 dy, vec4 dx)
{
    gl_Position = center + (dx - dy);
    EmitVertex();
    gl_Position = center + (-dx - dy);
    EmitVertex();
    gl_Position = center + (dx + dy);
    EmitVertex();
    gl_Position = center + (-dx + dy);
    EmitVertex();
    EndPrimitive();
}

bool IsCulled(vec4 normal)
{
    return normal.z > 0;
}

void main()
{
    gColor = vColor[0];
    vec4 center = gl_in[0].gl_Position;

    vec4 dx = worldToView[0]/2.0f * voxSize.x;
    vec4 dy = worldToView[1]/2.0f * voxSize.y;
    vec4 dz = worldToView[2]/2.0f * voxSize.z;

    if(!IsCulled(dx))
    AddQuad(center + dx, dy, dz);
    if(!IsCulled(-dx))
    AddQuad(center - dx, dz, dy);
    if(!IsCulled(dy))
    AddQuad(center + dy, dz, dx);
    if(!IsCulled(-dy))
    AddQuad(center - dy, dx, dz);
    if(!IsCulled(dz))
    AddQuad(center + dz, dx, dy);
    if(!IsCulled(-dz))
    AddQuad(center - dz, dy, dx);
}
