#version 330 core

in vec4 out_color_vertex;
in vec2 out_tex_coords;

uniform sampler2D texture_sampler;

out vec4 frag_color;

void main()
{
    vec4 tex_sample = texture( texture_sampler, out_tex_coords );
    frag_color = mix( out_color_vertex, tex_sample, 0.5 );
}