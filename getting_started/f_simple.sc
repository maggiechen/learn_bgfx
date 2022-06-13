$input v_color0, v_pos, v_normal, v_pointLightPos
uniform vec4 u_color;
uniform vec4 u_pointLightColor;

void main()
{
    vec3 light = normalize(v_pointLightPos - v_pos);
    vec3 normal = normalize(v_normal); // we will not account for scale because bgfx doesn't have an inverse matrix operator
    gl_FragColor = u_color * max(0, dot(light, normal));
}