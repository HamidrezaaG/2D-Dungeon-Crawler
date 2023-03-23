
attribute vec2 a_Position;

uniform float u_WorldScale;
uniform float u_WorldAspect;

uniform vec2 u_CameraPos;
uniform vec2 u_CameraDimensions;
uniform float u_CameraZoom;

uniform float u_Time;
uniform float u_Rot;
uniform vec2 u_Scale;

uniform vec2 u_Pos;

void main()
{
    vec2 pos = a_Position; // Basic vert info
    pos *= u_Scale;
    vec2 rot;
    rot.x = pos.x * cos(u_Rot) - pos.y * sin(u_Rot);
    rot.y = pos.x * sin(u_Rot) + pos.y * cos(u_Rot);
    pos = rot;
    pos += u_Pos;   // apply world position

    pos -= u_CameraPos;

    pos *= u_CameraZoom;
    pos += u_CameraDimensions * (1-u_CameraZoom) * 0.5;

    pos /= u_WorldScale;    // apply world scale
    pos.x /= u_WorldAspect;
    pos -= 1; // 0 to bottom left corner

    gl_Position = vec4( pos, 0, 1 );

}
