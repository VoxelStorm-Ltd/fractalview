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
    for(int i = 0; i < 40; i++) {
        float p2 = dot(pos,pos);
        scp2 = scale / p2;
        pos = abs(pos) * scp2 - julia;
    }
    if (scp2 > 1) {
        return 0.0;
    } else {
        return 1.0;
    }
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos + rayDir * 10;

    float d = 0.0, total_d = 0.0;
    d = distest(ray);

    gl_FragColor = vec4(d, d, d, 1.0 );
}


