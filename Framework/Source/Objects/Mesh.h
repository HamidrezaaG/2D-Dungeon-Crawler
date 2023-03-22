#pragma once
//#include <string>


namespace fw {

    class Texture;
    class ShaderProgram;

    class Vertex
    {
    public:
        Vertex(): x(0), y(0), u(0), v(0) {};
        Vertex(float _x, float _y) :x(_x), y(_y), u(0), v(0) {};
        Vertex(float _x, float _y, float _u, float _v) :x(_x), y(_y), u(_u), v(_v) {};
        float x;
        float y;
        float u;
        float v;
    };

    class Edge
    {
    public:
        Edge(int _index1, int _index2) : v1(_index1), v2(_index2) {};
        int v1;
        int v2;
    };

    enum class k_RenderTypes { Triangles, Lines };

class Mesh
{
public:
    Mesh();
    Mesh(int primitiveType, int numVertices, const Vertex* pVertices);
    void CreateShape(int primitiveType, int numVertices, const Vertex* pVertices);

    void MakeRect(Vector2 _anchor, float _width, float _height, bool _filled);

    Mesh(std::string fileAddress);
    Mesh(std::string fileAddress, fw::k_RenderTypes type);

    virtual ~Mesh();

    void Draw(Vector2 pos, float rot, class Camera* pCamera, ShaderProgram* pTextureShader, Vector2 scale, Texture* texture, Vector4 color, Vector2 uvScale, Vector2 uvOffset);
    void Mesh::Draw(Vector2 pos, float rot, Camera* pCamera, ShaderProgram* pBasicShader, Vector2 scale, Vector4 color);

    void MakeCircle(float _radius, int _vertCount, bool isfilled);
    //void MakeWavyCircle(float _radius, int _vertCount, float _offsetAngle, float _animValue, bool isFilled);

protected:

    void SetUniform1f(ShaderProgram* pShader, char* name, float value);
    void SetUniform2f(ShaderProgram* pShader, char* name, float value1, float value2);
    void SetUniform2f(ShaderProgram* pShader, char* name, Vector2 value);
    void SetUniform3f(ShaderProgram* pShader, char* name, Vector3 value);
    void SetUniform4f(ShaderProgram* pShader, char* name, Vector4 value);

    void SetUniform1i(ShaderProgram* pShader, char* name, int value);

    void LoadMeshTriangles(const char* fileAddress);
    void LoadMeshLines(const char* fileAddress);


    float GetLength(float x1, float x2, float y1, float y2);

    GLuint m_VBO = 0;

    std::vector<Vertex> m_FaceData;
    std::vector<float> m_tempVertexData;

    int m_NumVertices = 0;
    int m_PrimitiveType = GL_POINTS;
};

} // namespace fw
