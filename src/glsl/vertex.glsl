#version 440
in vec3 vertexPosition;
in vec2 textureCoordinates;

out vec2 uvCoords;
void main() {
    gl_Position = vec4(vertexPosition, 1.0);
    uvCoords = textureCoordinates;
};