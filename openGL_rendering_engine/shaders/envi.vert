#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 modelview;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * normal;
    Position = vec3(model * vec4(pos, 1.0));
    gl_Position = projection * modelview * vec4(pos, 1.0);
}
