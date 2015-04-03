#version 120
uniform vec2 window;
uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camUp;
uniform vec3 camSide;
uniform vec3 julia;

//float distest(vec3 pos) {
//    /*const float scale = 0.99;
//    float dr = 1.0;
//
//    for(int i = 0; i < 10; i++) {
//        float p2 = dot(pos,pos);
//        float scp2 = scale / p2;
//        pos = abs(pos) * scp2 - julia;
//        dr *= scp2;
//    }
//    return 1./sqrt(dr);*/
//    pos.xy = mod((pos.xy), 5.0)-vec2(2.5);
//    return min(
//        max(0.0, length(pos) - 1),
//        max(0.0, length(pos + vec3(0, 0, 5)) - 1)
//    );
//}

float distest(vec3 z)
{
    vec3 z2 = z;
	vec3 a1 = vec3(1,1,1);
	vec3 a2 = vec3(-1,-1,1);
	vec3 a3 = vec3(1,-1,-1);
	vec3 a4 = vec3(-1,1,-1);
	vec3 c;
	int n = 0;
    const float Scale = 2.0;
	float dist, d;
	while (n < 10) {
		 c = a1; dist = length(z2-a1);
	        d = length(z2-a2); if (d < dist) { c = a2; dist=d; }
		 d = length(z2-a3); if (d < dist) { c = a3; dist=d; }
		 d = length(z2-a4); if (d < dist) { c = a4; dist=d; }
		z2 = Scale*z2-c*(Scale-1.0);
		n++;
	}
 
	return length(z2) * pow(Scale, float(-n));
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos;

    float d = 0.0, total_d = 0.0;
    for(int i = 0; i < 15; ++i) {
        d = distest(ray);
        total_d += d;
        ray += rayDir * d;
        if (d < 0.0001) break;
        if (total_d > 100) break;
    }

    gl_FragColor = vec4(1.0-vec2(total_d/10.0).xxx, 1.0 );
}


