#version 330 core

//uniform vec4 u_color;
in vec4 color_out;

out vec4 frag_color;

void main()
{
    //frag_color = u_color;
    frag_color = color_out;
    //frag_color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );
}