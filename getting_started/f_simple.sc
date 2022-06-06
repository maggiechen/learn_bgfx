$input v_color0
uniform vec4 u_color;

void main()
{
    gl_FragColor = u_color * v_color0;
}