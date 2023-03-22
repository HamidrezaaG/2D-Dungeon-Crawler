
attribute vec2 a_Position;

uniform float u_WorldScale;
uniform float u_Time;
uniform float u_Rot;
uniform vec2 u_Scale;

uniform vec2 u_Pos;

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
    pos /= u_WorldScale;    // apply world scale
    pos -= 1; // 0 to bottom left corner
    ////pos *= abs(sin(u_Time));
    ////gl_Position = vec4( sin(pos*3.141592), 0, 1 );
    gl_Position = vec4( pos, 0, 1 );

}
