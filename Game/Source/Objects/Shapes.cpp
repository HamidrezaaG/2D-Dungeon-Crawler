#include "GamePCH.h"
#include "Shapes.h"

float meshAttribs_Triangle[] =
{
     0.25f,  0.0f,
      0.0f, 0.25f,
    -0.25f,  0.0F,
};

int meshPrimType_Triangle = GL_TRIANGLES;
int meshNumVerts_Triangle = sizeof(meshAttribs_Triangle) / (sizeof(float) * 2);

float meshAttribs_Square[] =
{
    -0.25f, -0.25f,
     0.25f,  0.25f,
     0.25f, -0.25f,

    -0.25f, -0.25f,
     0.25f,  0.25f,
    -0.25f,  0.25f,
};

int meshPrimType_Square = GL_TRIANGLES;
int meshNumVerts_Square = sizeof(meshAttribs_Square) / (sizeof(float) * 2);