#version 330

out vec4 finalColor;

in vec4 gridColor;

uniform vec4 backColor;
const float FARPLANE = 1000;// threshold

void main()
{
//    float distanceFromCamera = (gl_FragCoord.z / gl_FragCoord.w) / FARPLANE;
//    distanceFromCamera = max(0, min(1, distanceFromCamera)); // clip to valid value range
//    finalColor = mix(gridColor, backColor, distanceFromCamera);
    finalColor = gridColor;
}
