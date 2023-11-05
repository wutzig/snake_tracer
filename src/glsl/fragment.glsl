#version 440
precision highp float;
out vec4 fragmentColor;
in vec2 uvCoords;
uniform sampler2D renderedTexture;

void main() {
    // fragmentColor = vec4(vec3(0.7,0.2,0.2), 1.0);//color
    fragmentColor = vec4(texture( renderedTexture, uvCoords).rgb, 1.0);
    // fragmentColor = vec4(float(gl_FragCoord).x / 800.0, float(gl_FragCoord.y) / 600.0, 0.0, 1.0);
}