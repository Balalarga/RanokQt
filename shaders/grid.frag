#version 330

out vec4 finalColor;  // output: final color value as rgba-value

uniform vec4 gridColor;     // parameter: grid color as rgb triple
uniform vec4 backColor;     // parameter: background color as rgb triple
const float FARPLANE = 1000;// threshold

void main()
{
    float distanceFromCamera = (gl_FragCoord.z / gl_FragCoord.w) / FARPLANE;
    distanceFromCamera = max(0, min(1, distanceFromCamera)); // clip to valid value range
    finalColor = mix(gridColor, backColor, distanceFromCamera);
}
