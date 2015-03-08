#version 120
uniform vec2 window;
uniform float power;


float distest(vec3 pos) {
    const float scale = 0.99;
    const vec3 julia = vec3(1.);
    
    vec3 p = pos;
    float dr = 1.0;
    float p2;
  
    for( int i=0; i< 10; i++ ) {
        p2 = dot(p,p);

        if (p2 > 5.) break;

        p = abs(p) * scale / p2 - julia;
        dr = dr / p2 * scale;
    }
    return (abs(pos.x)+abs(pos.y))/dr;
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 camPos = vec3(0., 0.1 + power/100., 5.0-power/20.);
    const vec3 camTarget = vec3(0.0, 0.0, 0.0);

    vec3 camDir = normalize(camTarget-camPos);
    const vec3 camUp  = normalize(vec3(0.0, 6.0, 0.0));
    vec3 camSide = cross(camDir, camUp);

    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos;
    float m = 0.0;
    float d = 0.0, total_d = 0.0;
    const int MAX_MARCH = 7;
    const float MAX_DISTANCE = 8.0;
    const float mInc = 1./float(MAX_MARCH);
    for(int i=0; i<MAX_MARCH; ++i) {
        d = distest(ray);
        total_d += d;
        ray += rayDir * d;
        m += mInc;
        if(d<0.001) { break; }
        if(total_d>MAX_DISTANCE) { total_d=MAX_DISTANCE; break; }
    }

    float c = (total_d)*0.0001;
    vec4 result = vec4( 1.0-vec3(c, c, c) - vec3(m, m, m), 1.0 );
    gl_FragColor = result;
}


