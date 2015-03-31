#version 120
uniform vec2 window;
uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camUp;
uniform vec3 camSide;
uniform vec3 julia;

float distest(vec3 pos) {
    const float scale = 0.99;
    float dr = 1.0;

    for(int i = 0; i < 10; i++) {
        float p2 = dot(pos,pos);
        float scp2 = scale / p2;
        pos = abs(pos) * scp2 - julia;
        dr *= scp2;
    }
    return 1./sqrt(dr);
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos;

    float d = 0.0, total_d = 0.0;
    for(int i = 0; i < 5; ++i) {
        d = distest(ray);
        total_d += d*0.1;
        ray += rayDir * d;
    }

    gl_FragColor = vec4(1.0-vec2(total_d).xxx, 1.0 );
}


