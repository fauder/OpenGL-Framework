#version 330 core
layout( location = 0 ) in vec3 position_clip;
layout( location = 1 ) in vec4 color_vertex;

uniform float offset_horizontal;

out vec4 color_out;

void main()
{
    color_out   = color_vertex;
    gl_Position = vec4( position_clip.x + offset_horizontal, position_clip.y, position_clip.z, 1.0 );
}