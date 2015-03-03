#version 120
uniform vec2 window;

void main()
{
    vec2 coord = vec2(4.0 * (gl_FragCoord.x/window.x - 0.5), 4.0 * (gl_FragCoord.y/window.y - 0.5));
    vec2 z = coord;

    int i;
    int iter = 50;
    for (i=0; i < iter; i++) {
        vec2 newz = vec2(
            z.x * z.x - z.y * z.y + coord.x,
            z.y * z.x + z.x * z.y + coord.y
        );

        if (newz.x * newz.x + newz.y * newz.y > 4.0) {
            break;
        }

        z = newz;
    }

    if (i == iter) {
        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    } else {
        gl_FragColor = vec4(0.0, 0.0, float(i)/iter, 1.0);
    }

}
