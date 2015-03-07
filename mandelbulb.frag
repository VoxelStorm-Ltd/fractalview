#version 120
uniform vec2 window;
uniform float power;

const float scale = 0.99;
const vec3 julia = vec3(1.);

float distest(vec3 p) {
    float dr = 1.0;
    float p2;
  
    for( int i=0; i< 3; i++ ) {
        p2 = dot(p,p);

        if (p2 > 1.) break;

        p = abs(p);
        p /= p2;
        p *= scale;
        p -= julia;

        dr = dr / p2 * scale;
    }
    //return length(p)/dr;
    return .1*(abs(p.x)+abs(p.y))*length(p)/dr;
}

void main( void )
{
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 camPos = vec3(0., 0.1, 2.0-power/2.);
    vec3 camTarget = vec3(0.0, 0.0, 0.0);

    vec3 camDir = normalize(camTarget-camPos);
    vec3 camUp  = normalize(vec3(0.0, 6.0, 0.0));
    vec3 camSide = cross(camDir, camUp);

    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos;
    float m = 0.0;
    float d = 0.0, total_d = 0.0;
    const int MAX_MARCH = 30;
    const float MAX_DISTANCE = 10.0;
    for(int i=0; i<MAX_MARCH; ++i) {
        d = distest(ray);
        total_d += d;
        ray += rayDir * d;
        m += 1.0;
        if(d<0.001) { break; }
        if(total_d>MAX_DISTANCE) { total_d=MAX_DISTANCE; break; }
    }

    float c = (total_d)*0.0001;
    vec4 result = vec4( 1.0-vec3(c, c, c) - vec3(0.05, 0.05, 0.03)*m*0.8, 1.0 );
    gl_FragColor = result;
}


