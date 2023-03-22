
uniform float u_Time;
uniform vec4 u_Color;

uniform sampler2D u_Texture;
varying vec2 v_UVCoords;

void main()
{
    gl_FragColor = texture2D(u_Texture, v_UVCoords) * u_Color;//vec4( 0.5*sin(3*u_Time)+0.5, 1, 1, 1 );
}
