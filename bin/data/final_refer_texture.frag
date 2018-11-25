#version 330

uniform sampler2DRect reference_texture;
uniform sampler2DRect texcoord_texture;

uniform vec2 u_resolution;

vec2 upset_y (vec2 a, vec2 res) {
    return vec2(a.x, res.y - a.y);
}

out vec4 output_color;

void main() {
    vec2 st = upset_y(gl_FragCoord.xy, u_resolution);
    vec2 texcoord = texture(texcoord_texture, st).xy * u_resolution;

    vec3 col;
    if (all(equal(texcoord.xy, vec2(0.)))) {
        col = texture(reference_texture, st).rgb;
    } else {
        col = texture(reference_texture, texcoord).rgb;
    }

    output_color = vec4(col, 1.0);
}