#version 440
precision highp float;
out vec4 fragmentColor;
in vec2 uvCoords;
uniform sampler2D renderedTexture;

void main() {
    fragmentColor = vec4(texture( renderedTexture, uvCoords).rgb, 1.0);
}