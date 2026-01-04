#version 330 core
layout (location = 0) in vec3 aPos; // position
layout (location = 1) in vec3 aColor; // color
layout (location = 2) in vec2 aTexCoords; // Texture

out vec3 ourColor;
out vec2 ourTexCoords;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    ourTexCoords = aTexCoords;
}
