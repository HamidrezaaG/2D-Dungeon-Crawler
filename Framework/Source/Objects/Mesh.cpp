#include "Framework.h"
#include "../Math/Math.h"
#include "Texture.h"
#include "Camera.h"

namespace fw
{

Mesh::Mesh()
{
}
Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_VBO);
}



void Mesh::MakeRect(Vector2 _anchor, float _width, float _height, bool _filled)
{
    assert(_width > 0 && _height > 0);

    _anchor.ClampLength(1);
    _anchor.x *= _width;
    _anchor.y *= _height;
    _anchor *= -1;

    glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);


    Vertex attribs[4]
    {
    Vertex(_anchor.x         , _anchor.y          , 0, 0),
    Vertex(_anchor.x         , _anchor.y + _height, 0, 1),
    Vertex(_anchor.x + _width, _anchor.y + _height, 1, 1),
    Vertex(_anchor.x + _width, _anchor.y          , 1, 0),
    };
    //float OffsetAngle = k_Convert_Deg2Rad * _offsetAngle;

    _filled? m_PrimitiveType = GL_TRIANGLE_FAN : m_PrimitiveType = GL_LINE_LOOP;

    m_NumVertices = 4;
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_NumVertices, &attribs[0], GL_STATIC_DRAW);

}

Mesh::Mesh(std::string fileAddress)
{
    LoadMeshTriangles(fileAddress.c_str());
    // Generate a buffer for our vertex attributes.
    glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    m_NumVertices = (int)(m_FaceData.size());
    m_PrimitiveType = GL_TRIANGLES;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

    // Copy our attribute data into the VBO.
    //int numAttributeComponents = (m_NumVertices * 4); // x & y & u & v  for each vertex.
    glBufferData(GL_ARRAY_BUFFER, sizeof(fw::Vertex) * m_NumVertices, &m_FaceData[0], GL_STATIC_DRAW);
}

Mesh::Mesh(std::string fileAddress, fw::k_RenderTypes type)
{
    if (type == fw::k_RenderTypes::Triangles)
    {
        LoadMeshTriangles(fileAddress.c_str());
        // Generate a buffer for our vertex attributes.
        glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.

        // Set this VBO to be the currently active one.
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        m_NumVertices = (int)(m_FaceData.size());
        m_PrimitiveType = GL_TRIANGLES;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

        // Copy our attribute data into the VBO.
        glBufferData(GL_ARRAY_BUFFER, sizeof(fw::Vertex) * m_NumVertices, &m_FaceData[0], GL_STATIC_DRAW);
    }
    else if (type == fw::k_RenderTypes::Lines)
    {
        LoadMeshLines(fileAddress.c_str());
        // Generate a buffer for our vertex attributes.
        glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.
        // Set this VBO to be the currently active one.
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        m_NumVertices = (int)(m_FaceData.size());
        m_PrimitiveType = GL_LINES;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

        // Copy our attribute data into the VBO.
        glBufferData(GL_ARRAY_BUFFER, sizeof(fw::Vertex) * m_NumVertices, &m_FaceData[0], GL_STATIC_DRAW);
    }
}


void Mesh::LoadMeshTriangles(const char * fileAddress)
{

    FILE* file;

    errno_t error = fopen_s(&file, fileAddress, "rb");

     std::vector<Vertex> tempVertexData;
     std::vector<int> tempFaceIndexData;

     int vindex  = 1;
     int facecount = 0;

     if (file) {
         while (1) {

             char lineHeader[128];
             // read the first word of the line
             int res = fscanf_s(file, "%s", lineHeader, (unsigned int)sizeof(lineHeader));

             if (res == EOF)
                 break; // EOF = End Of File. Quit the loop.


             if (strcmp(lineHeader, "v") == 0)
             {
                 float _x, _y, _z;
                 fscanf_s(file, "%f %f %f\n", &_x, &_y, &_z);
                 Vertex v(_x, _z);
                 tempVertexData.push_back(v);
                 vindex++;
             }
             else if (strcmp(lineHeader, "f") == 0)
             {
                 float _v1, _v2, _v3;
                 fscanf_s(file, "%f %f %f\n", &_v1, &_v2, &_v3);

                 tempFaceIndexData.push_back((int)_v1);
                 tempFaceIndexData.push_back((int)_v2);
                 tempFaceIndexData.push_back((int)_v3);

                 facecount++;
             }
         }
         fclose(file);
     }


 for (int i = 0; i < facecount*3; i++)
 {
     int vertexIndex = tempFaceIndexData[i]-1;

     Vertex v(tempVertexData.at(vertexIndex).x, tempVertexData.at(vertexIndex).y, 0.f, 0.f);
     m_FaceData.push_back(v);

 }

}

void Mesh::LoadMeshLines(const char* fileAddress)
{
  
  FILE* file;
  errno_t error = fopen_s(&file, fileAddress, "rb");

  std::vector<Vertex> tempVertexData;
  std::vector<Edge> tempEdgeData;

  std::vector<int> tempFaceIndexData;

  int vindex = 0;
  int eindex = 0;
  int facecount = 0;

  if (file)
  {

      while (1) {

          char lineHeader[128];
          // read the first word of the line
          int res = fscanf_s(file, "%s", lineHeader, (unsigned int) sizeof(lineHeader));

          if (res == EOF)
              break; // EOF = End Of File. Quit the loop.


          if (strcmp(lineHeader, "v") == 0)
          {
              float _x, _y, _z;
              fscanf_s(file, "%f %f %f\n", &_x, &_y, &_z);
              Vertex v(_x, _z);
              tempVertexData.push_back(v);
              vindex++;
          }
          else if (strcmp(lineHeader, "l") == 0)
          {
              float _v1, _v2;
              fscanf_s(file, "%f %f\n", &_v1, &_v2);

              Edge e((int)_v1, (int)_v2);

              tempEdgeData.push_back(e);

              eindex++;
          }
      }
      fclose(file);
  }

  for (int i = 0; i < tempEdgeData.size(); i++)
  {
      Vertex v1(tempVertexData.at((tempEdgeData[i].v1 - (size_t)1)).x, tempVertexData.at((tempEdgeData[i].v1 - (size_t)1)).y, 0.f, 0.f);
      m_FaceData.push_back(v1);
  
      Vertex v2(tempVertexData.at((tempEdgeData[i].v2 - (size_t)1)).x, tempVertexData.at((tempEdgeData[i].v2 - (size_t)1)).y, 0.f, 0.f);
      m_FaceData.push_back(v2);
  }
}
void Mesh::MakeCircle(float _radius, int _vertCount, bool isFilled)
{
    assert(_vertCount > 2);

    glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    std::vector<Vertex> attribs;

    float Angle = 2 * (float)k_PI / _vertCount;
    //float OffsetAngle = k_Convert_Deg2Rad * _offsetAngle;

        for (int i = 0; i < _vertCount; i++)
        {
            float tempx = _radius * cosf(Angle * i);
            float tempy = _radius * sinf(Angle * i);
            float tempu = 0.5f * (1 + cosf(Angle * i));
            float tempv = 0.5f * (1 + sinf(Angle * i));


            attribs.push_back(Vertex(tempx, tempy, tempu, tempv));
        }
        
    if(isFilled)
    {
        m_PrimitiveType = GL_TRIANGLE_FAN;
    }
    else {
        m_PrimitiveType = GL_LINE_LOOP;
    }

    m_NumVertices = _vertCount;
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_NumVertices, &attribs[0], GL_STATIC_DRAW);

}

Mesh::Mesh(int primitiveType, int numVertices, const Vertex* pVertices)
{
    CreateShape(primitiveType, numVertices, pVertices);
}
void Mesh::CreateShape(int primitiveType, int numVertices, const Vertex* pVertices)
{
    // Delete the old buffer if we had one.
    glDeleteBuffers(1, &m_VBO);

    // Generate a buffer for our vertex attributes.
    glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    m_NumVertices = numVertices;
    m_PrimitiveType = primitiveType;

    // Copy our attribute data into the VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_NumVertices, pVertices, GL_STATIC_DRAW);
}

/// TEXTURED DRAW
void Mesh::Draw(Vector2 pos, float rot, Camera* pCamera, ShaderProgram* pTextureShader, Vector2 scale, Texture* texture, Vector4 color, Vector2 uvScale, Vector2 uvOffset)
{

    glUseProgram(pTextureShader->GetProgram());

    // Set this VBO to be the currently active one.
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

    // Get the attribute variable’s location from the shader.
    GLint loc = glGetAttribLocation(pTextureShader->GetProgram(), "a_Position" );
    if (loc != -1)
    {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 16, (void*)0);
    }

    loc = glGetAttribLocation(pTextureShader->GetProgram(), "a_UVCoords");
    if (loc != -1)
    {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);
    }

    // UNIFORMS
    {
        SetUniform1f(pTextureShader, "u_WorldScale", k_World_Scale * 0.5f);
        SetUniform1f(pTextureShader, "u_WorldAspect", k_Window_Aspect);

        SetUniform1f(pTextureShader, "u_Time", (float)GetSystemTimeSinceGameStart());
        SetUniform1f(pTextureShader, "u_Rot", (float)(rot * k_Convert_Deg2Rad));
        SetUniform2f(pTextureShader, "u_Pos", pos);
        SetUniform2f(pTextureShader, "u_Scale", scale);
        SetUniform4f(pTextureShader, "u_Color", color);
        SetUniform2f(pTextureShader, "u_CameraPos", pCamera->GetPosition());
        SetUniform2f(pTextureShader, "u_CameraDimensions", pCamera->GetDimensions());
        SetUniform1f(pTextureShader, "u_CameraZoom", pCamera->GetZoom());

        SetUniform2f(pTextureShader, "u_UVScale", uvScale);
        SetUniform2f(pTextureShader, "u_UVOffset", uvOffset);
    }

    if (texture != nullptr)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
        SetUniform1i(pTextureShader, "u_Texture", 0);
    }
    // Draw the primitive.
    glDrawArrays( m_PrimitiveType, 0, m_NumVertices );
}

/// VERT DRAW
void Mesh::Draw(Vector2 pos, float rot, Camera* pCamera, ShaderProgram* pBasicShader, Vector2 scale, Vector4 color)
{

    glUseProgram(pBasicShader->GetProgram());

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Get the attribute variable’s location from the shader.
    GLint loc = glGetAttribLocation(pBasicShader->GetProgram(), "a_Position");
    if (loc != -1)
    {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 16, (void*)0);
    }

    loc = glGetAttribLocation(pBasicShader->GetProgram(), "a_UVCoords");
    if (loc != -1)
    {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);
    }

    // UNIFORMS
    {
        SetUniform1f(pBasicShader, "u_WorldScale", k_World_Scale * 0.5f);
        SetUniform1f(pBasicShader, "u_WorldAspect", k_Window_Aspect);

        SetUniform1f(pBasicShader, "u_Time", (float)GetSystemTimeSinceGameStart());
        SetUniform1f(pBasicShader, "u_Rot", (float)(rot * k_Convert_Deg2Rad));
        SetUniform2f(pBasicShader, "u_Pos", pos);
        SetUniform2f(pBasicShader, "u_Scale", scale);
        SetUniform4f(pBasicShader, "u_Color", color);
        SetUniform2f(pBasicShader, "u_CameraPos", pCamera->GetPosition());
        SetUniform2f(pBasicShader, "u_CameraDimensions", pCamera->GetDimensions());
        SetUniform1f(pBasicShader, "u_CameraZoom", pCamera->GetZoom());

    }
    // Draw the primitive.
    glDrawArrays(m_PrimitiveType, 0, m_NumVertices);
}

void Mesh::SetUniform1f(ShaderProgram* pShader, char* name, float value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform1f(loc, value);
}
void Mesh::SetUniform2f(ShaderProgram* pShader, char* name, float value1, float value2)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform2f(loc, value1, value2);
}
void Mesh::SetUniform2f(ShaderProgram* pShader, char* name, Vector2 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform2f(loc, value.x, value.y);
}
void Mesh::SetUniform3f(ShaderProgram* pShader, char* name, Vector3 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform3f(loc, value.x, value.y, value.z);
}
void Mesh::SetUniform4f(ShaderProgram* pShader, char* name, Vector4 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Mesh::SetUniform1i(ShaderProgram* pShader, char* name, int value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform1i(loc, value);
}

} // namespace fw
