#version 330 core
layout( location = 0 ) in vec3 position_clip;
layout( location = 1 ) in vec4 color_vertex;
layout( location = 2 ) in vec2 tex_coords;

uniform float offset_horizontal;
uniform mat4 transformation;

out vec4 out_color_vertex;
out vec2 out_tex_coords;

void main()
{
    out_color_vertex   = color_vertex;
    out_tex_coords     = tex_coords;
    gl_Position        = vec4( position_clip.x, position_clip.y, position_clip.z, 1.0 ) * transformation;
    gl_Position        = vec4( gl_Position.x + offset_horizontal, gl_Position.yzw );
}