// The varying.def.sc file defines the "varying" variables (i.e. they are different for different pixels, they are not uniforms)
// and associates them with a type. That way we don't have to declare them in the vertex/fragment shaders
// NOTE: YOU MUST NOT PUT COMMENTS IN VARYING.DEF.SC. IT WILL NOT TELL YOU THAT IT FAILED BECAUSE OF THE COMMENTS
// https://bkaradzic.github.io/bgfx/tools.html#shader-compiler-shaderc :
//    "A varying.def.sc file must be used to define input/output semantics and types,
//     instead of using attribute/in and varying/in/out. This file cannot include comments,
//     and typically only one is necessary."
// It will just say that it can't find the varying variable, which is a very unintuitive error. 

$input a_position, a_texcoord0, a_normal
$output v_color0, v_pos, v_normal, v_pointLightPos
uniform vec4 u_pointLightPos;
uniform mat4 u_modelMatrix;
#include <bgfx_shader.sh>

void main()
{
    // u_modelViewProj is from the bgfx_shader header. 
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_color0 = vec4(a_texcoord0, 0, 1.0);
    v_pointLightPos = (transpose(u_modelMatrix) * u_pointLightPos).xyz;
    v_pos = a_position;
    v_normal = a_normal;

}