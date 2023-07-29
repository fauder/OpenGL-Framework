#version 330 core
layout( location = 0 ) in vec3 position_clip;
layout( location = 1 ) in vec2 tex_coords;
layout( location = 2 ) in vec4 color_vertex;

uniform float offset_horizontal;
uniform mat4 transformation_world;
uniform mat4 transformation_view;
uniform mat4 transformation_projection;

out vec4 out_color_vertex;
out vec2 out_tex_coords;

void main()
{
    out_color_vertex = color_vertex;
    out_tex_coords   = tex_coords;

    vec4 position_world = vec4( position_clip.xyz, 1.0 ) * transformation_world;
    gl_Position         = position_world * transformation_view * transformation_projection;
}