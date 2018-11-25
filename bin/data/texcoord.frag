#version 330

uniform vec2 u_resolution;

out vec4 output_texcoord; // vec4(texcoord.xy, 0., 0.)

void main() {
    output_texcoord = vec4(gl_FragCoord.xy/u_resolution, 0., 1.);
}