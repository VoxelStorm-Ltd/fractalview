#version 120
uniform vec2 window;
uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camUp;
uniform vec3 julia;

float distest(vec3 pos) {
    const float scale = 0.99;

    vec3 p = pos;
    float dr = 1.0;

    for( int i = 0; i< 10; i++ ) {
        float p2 = dot(p,p);
        float scp2 = scale / p2;
        p = abs(p) * scp2 - julia;
        dr *= scp2;
    }
    return 1./sqrt(dr);
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 camDirN = normalize(camDir);
    vec3 camSide = cross(camDirN, camUp);

    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDirN);
    vec3 ray = camPos;

    float d = 0.0, total_d = 0.0;
    const int MAX_MARCH = 10;
    for(int i=0; i<MAX_MARCH; ++i) {
        d = distest(ray);
        total_d += d*0.04;
        ray += rayDir * d;
    }

    gl_FragColor = vec4(1.0-vec2(total_d).xxx, 1.0 );
}


