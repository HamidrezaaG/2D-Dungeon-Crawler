#include "GamePCH.h"
#include "Shapes.h"
//
//const fw::Vertex meshAttribs_Triangle[] =
//{
//     fw::Vertex(0.25f ,  0.0f, 0 , 0),
//     fw::Vertex(0.0f  , 0.25f, 0 , 0),
//     fw::Vertex(-0.25f,  0.0F, 0 , 0)
//};
//
//const int meshPrimType_Triangle = GL_TRIANGLES;
//const int meshNumVerts_Triangle = sizeof(meshAttribs_Triangle) / (sizeof(fw::Vertex));
//
//const fw::Vertex meshAttribs_Square[] =
//{
//    fw::Vertex( -0.25f, -0.25f, 0, 0),
//    fw::Vertex(  0.25f,  0.25f, 0, 0),
//    fw::Vertex(  0.25f, -0.25f, 0, 0),
//
//    fw::Vertex( -0.25f, -0.25f, 0, 0),
//    fw::Vertex(  0.25f,  0.25f, 0, 0),
//    fw::Vertex( -0.25f,  0.25f, 0, 0),
//};
//
//const int meshPrimType_Square = GL_TRIANGLES;
//const int meshNumVerts_Square = sizeof(meshAttribs_Square) / (sizeof(fw::Vertex));

const fw::Vertex meshAttribs_Sprite[] =
{
    fw::Vertex(0, 0, 0, 0),
    fw::Vertex(0, 1, 0, 1),
    fw::Vertex(1, 1, 1, 1),
    fw::Vertex(1, 0, 1, 0),
};

const int meshPrimType_Square = GL_TRIANGLE_FAN;
const int meshNumVerts_Square = sizeof(meshAttribs_Sprite) / (sizeof(fw::Vertex));
