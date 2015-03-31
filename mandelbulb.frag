#version 120
uniform vec2 window;
uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camUp;


float distest(vec3 pos) {
    const float scale = 1.01;
    const vec3 julia = vec3(1., 1., 0.99);

    vec3 p = pos;
    float dr = 1.0;

    for( int i = 0; i< 50; i++ ) {
        float p2 = dot(p,p);
        p = abs(p) * scale / p2 - julia;
        dr /= sqrt(p2);
    }
    return 1./dr;
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 camDirN = normalize(camDir);
    vec3 camSide = cross(camDirN, camUp);

    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDirN);
    vec3 ray = camPos;

    float d = 0.0, total_d = 0.0;
    const int MAX_MARCH = 20;
    const float mInc = 1./float(MAX_MARCH);
    for(int i=0; i<MAX_MARCH; ++i) {
        d = distest(ray);
        total_d += d*0.001;
        ray += rayDir * d;
    }

    gl_FragColor = vec4( 1.0-vec3(total_d, total_d, total_d), 1.0 );
}


