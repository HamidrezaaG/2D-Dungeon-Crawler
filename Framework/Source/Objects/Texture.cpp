#include "FrameworkPCH.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../Libraries/stb/stb_image.h"

namespace fw
{
    Texture::Texture(const char* filename)
    {
        int width,height,numComponents;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filename, &width, &height, &numComponents, 4);
        // ... process data if not NULL ...
        // ... x = width, y = height, n = # 8-bit components per pixel ...
        // ... replace '0' with '1'..'4' to force that many components per pixel
        // ... but 'n' will always be the number that it would have been if you said 0
        if (data != nullptr)
        {
        // make a gl texture
        glGenTextures(1, &m_TextureHandle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureHandle);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);


        stbi_image_free(data);
        }
    }

    Texture::~Texture()
    {
    }

    GLuint Texture::GetHandle()
    {
        return m_TextureHandle;
    }
} // namespace fw
