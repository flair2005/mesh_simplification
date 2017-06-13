#version 330 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 modelview;

void main()
{
    TexCoords = pos;
    vec4 pers_division_pos = projection * modelview * vec4(pos, 1.0);
    gl_Position = pers_division_pos.xyww;
}
