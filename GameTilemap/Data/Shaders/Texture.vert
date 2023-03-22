
attribute vec2 a_Position;
attribute vec2 a_UVCoords;

uniform float u_WorldScale;
uniform float u_WorldAspect;

uniform vec2 u_CameraPos;
uniform vec2 u_CameraDimensions;
uniform float u_CameraZoom;

uniform float u_Time;
uniform float u_Rot;
uniform vec2 u_Scale;

uniform vec2 u_Pos;

uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;
varying vec2 v_UVCoords; // output to frag


void main()
{
    vec2 pos = a_Position; // Basic vert info
    pos *= u_Scale;
    //pos *= 2;   // scale elements as we please
    vec2 rot;
    rot.x = pos.x * cos(u_Rot) - pos.y * sin(u_Rot);
    rot.y = pos.x * sin(u_Rot) + pos.y * cos(u_Rot);
    //pos -= u_Pos;
    pos = rot;
    //pos += u_Pos;
    ////pos += 0.5f;
    //
    ////pos.x *= sign(sin(6*u_Time)); // flipping behavior
    pos += u_Pos;   // apply world position

    pos -= u_CameraPos;

    pos *= u_CameraZoom;
    pos.x /= u_WorldAspect;
    pos += u_CameraDimensions * (1-u_CameraZoom) * 0.5;

    pos /= u_WorldScale;    // apply world scale
    pos -= 1; // 0 to bottom left corner
    ////pos *= abs(sin(u_Time));
    ////gl_Position = vec4( sin(pos*3.141592), 0, 1 );

    v_UVCoords = a_UVCoords * u_UVScale + u_UVOffset;;

    gl_Position = vec4( pos, 0, 1 );

}
