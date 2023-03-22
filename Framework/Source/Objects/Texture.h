#pragma once


namespace fw {
    class Texture {

    public:
        Texture(const char* filename);
        virtual ~Texture();

        GLuint GetHandle();
    protected:
        GLuint m_TextureHandle = 0;
    };
} // namespace fw
