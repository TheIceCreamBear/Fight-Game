#version 410

in vec3 pos;

out vec3 fragColor;

uniform vec3 color;

void main(void) {
    gl_Position = vec4(pos, 1.0);
    fragColor = vec3(clamp(pos.x + color.r, 0, 1), color.b, clamp(pos.y + color.g, 0, 1));
}