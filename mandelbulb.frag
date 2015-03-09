#version 120
uniform vec2 window;
uniform float power;


float distest(vec3 pos) {
    const float scale = 0.99;
    const vec3 julia = vec3(1.);
    
    vec3 p = pos;
    float dr = 1.0;

    for( int i = 0; i< 100; i++ ) {
        float p2 = dot(p,p);
        p = abs(p) * scale / p2 - julia;
        dr /= sqrt(p2);
    }
    return 1./dr;
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 camPos = vec3(4.0 - power, -6.0 + power, -16.0 + power * 2.0);
    const vec3 camTarget = vec3(0.0, 0.0, 0.0);

    vec3 camDir = normalize(camTarget-camPos);
    const vec3 camUp  = normalize(vec3(0.0, 1.0, 0.0));
    vec3 camSide = cross(camDir, camUp);

    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos;

    float d = 0.0, total_d = 0.0;
    const int MAX_MARCH = 10;
    const float MAX_DISTANCE = 70.0;
    const float mInc = 1./float(MAX_MARCH);
    for(int i=0; i<MAX_MARCH; ++i) {
        d = distest(ray);
        total_d += d*0.1;
        ray += rayDir * d;
    }

    gl_FragColor = vec4( 1.0-vec3(total_d, total_d, total_d), 1.0 );
}


