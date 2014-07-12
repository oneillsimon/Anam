#version 120

varying vec2 texCoord0;

uniform vec3 R_ambient;
uniform vec4 R_colour;
uniform sampler2D diffuse;

void main()
{
    vec4 finalCol = vec4(R_ambient, 1) + R_colour * R_colour.a;
    gl_FragColor = texture2D(diffuse, texCoord0.xy) * finalCol;
}
