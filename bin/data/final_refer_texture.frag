#version 330

layout(binding = 0) uniform sampler2DRect reference_texture;
layout(binding = 1) uniform sampler2DRect texcoord_texture;

layout(location = 0) uniform vec2 u_resolution;

out vec4 output_color;

void main() {
    vec2 texcoord = texture(texcoord_texture, gl_FragCoord.xy).xy / u_resolution;

    vec3 col;
    if (equal(texcoord.xy, vec2(0.)) {
        col = texture(reference_texture, gl_FragCoord.xy);
    } else {
        col = texture(reference_texture, texcoord).rgb;
    }

    output_color = vec4(col, 1.0);
}