#version 330

out vec4 color;

uniform vec2 resolution;  // Width and height of the shader
uniform vec3 cameraPosition;
uniform vec2 cameraRotation;

#define PI 3.1415925359
#define TWO_PI 6.2831852
const float MAX_STEPS = 100;
const float MAX_DIST = 100.;
const float SURFACE_DIST = .01;


float GetDist(vec3 p)
{
    vec4 s = vec4(0, 1, 9, 1); //Sphere. xyz is position w is radius
    float sphereDist = length(p - s.xyz) - s.w;
    float planeDist = p.y;
    float d = min(sphereDist, planeDist);

    return d;
}

float RayMarch(vec3 ro, vec3 rd)
{
    float dO = 0.; //Distane Origin
    for(int i=0; i<MAX_STEPS; i++)
    {
        vec3 p = ro + rd * dO;
        float ds = GetDist(p); // ds is Distance Scene
        dO += ds;
        if(dO > MAX_DIST || ds < SURFACE_DIST) break;
    }
    return dO;
}

void main()
{
    vec2 uv = (gl_FragCoord.xy-.5*resolution.xy)/resolution.y;
    vec3 ro = vec3(0,1,0); // Ray Origin/ Camera
    vec3 rd = normalize(vec3(uv.x,uv.y,1));
    float d = RayMarch(ro,rd); // Distance
    d/= 10.;

    // Set the output color
    color = vec4(vec3(d),1.0);
}
