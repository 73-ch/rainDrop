#version 330

in vec2 v_texcoord;
in vec2 v_position;
uniform vec2 u_resolution;
uniform vec2 screen_size;

out vec4 out_color;

float hash( vec2 p ){
    return fract( sin( dot(p, vec2( 15.79, 81.93  ) ) * 45678.9123 ) );
}

float valueNoise( vec2 p ) {
    vec2 i = floor( p );
    vec2 f = fract( p );
    
    f = f*f*(3.0 - 2.0*f);
    
    float bottomOfGrid =    mix( hash( i + vec2( 0.0, 0.0 ) ), hash( i + vec2( 1.0, 0.0 ) ), f.x );
    float topOfGrid =       mix( hash( i + vec2( 0.0, 1.0 ) ), hash( i + vec2( 1.0, 1.0 ) ), f.x );
    
    return mix( bottomOfGrid, topOfGrid, f.y );
}

float fbm( vec2 uv , float diff)
{
    float sum = 0.00;
    float amp = 0.7;
    
    for( int i = 0; i < 4; ++i ){
        sum += valueNoise( uv ) * amp;
        uv += uv * diff;
        amp *= 0.4;
    }
    return sum;
}

float plot(vec2 st, float pct){
    return  smoothstep( pct - 0.005, pct, st.y) -
    smoothstep( pct, pct + 0.005, st.y);
}

void main() {

//    asin(x*2.-1.)/PI+.
//    vec2 p = (v_texcoord.xy*2.-u_resolution) / u_resolution;
    vec2 p = (v_texcoord / u_resolution + 1.) * .5;
    vec2 st = gl_FragCoord.xy/vec2(1024,768);
    
//    p *= 0.3 + fbm(st + p*0.6,1.2)*1.7;
    
    float begin = st.x;
    float range = 0.8;
    float vol = 0.5 * (1.0 - abs(begin));
    
    vec2 final;
//    final = mix(vec2(0.), st, smoothstep(vec2(0.0), vec2(0.1), p));
    final.x = mix(0., st.x, smoothstep(0., .4, p.x)) + mix(st.x, 1., smoothstep(.6,0.7, p.x));
    final.y = mix(0., st.y, smoothstep(0., .4, p.y)) + mix(st.y, 1., smoothstep(.6,0.7, p.y));
//    final  = st;
//    final = step(vec2(0.7), p);
//    final = vec2(step(0.8,p.x));
//    final.x = p.x+(smoothstep(clamp(begin-range,1.,-1.),begin, p.x)*smoothstep(clamp(-begin-range,-1.,1.), -begin, -p.x))*vol;
//    final.y = p.y+(smoothstep(clamp(begin-range,1.,-1.),begin, p.y)*smoothstep(clamp(-begin-range,-1.,1.), -begin, -p.y))*vol;

//    final = (final + vec2(1.0))*.5;
//    final = step(vec2(.0), final);
//    final = st;
//    final = vec2(plot(p, final.x));
    final *= pow(1. - length((p)*2.-1) -fbm(st+p, 1.2)*.4, 3.0);
    
//    final.x = final.x >= 1.0? 0.:final.x;
//    final.y = final.y >= 1.0? 0.:final.y;
    
    out_color = vec4(final, 0., 1.0);
}
