#version 120
uniform vec2 window;
uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camUp;
uniform vec3 camSide;
uniform vec3 julia;

float distest(vec3 pos) {
    const float scale = 0.99;
    float scp2 = 0;
    for(int i = 0; i < 8; i++) {
        float p2 = dot(pos,pos);
        scp2 = scale / p2;
        pos = abs(pos) * scp2 - julia;
    }
    return scp2;
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos + rayDir;

    float d;
    for (int i = 0; i < 3; i++) {
        d = distest(ray);
        ray += rayDir * d;
        if (d < 0.001) break;
    }

    gl_FragColor = vec4(1-d, 1-d, 1-d, 1.0 );
}


