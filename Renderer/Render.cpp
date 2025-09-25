#include "Render.h"

#include <fstream>
#include <string>

#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION 1
#include "ThirdParty/stb_image.h"

namespace aie
{
    Geometry aie::MakeGeometry(const Vertex* const Verts, GLsizei VertCount, const GLuint* const indices, GLsizei IndexCount)
    {
        // create an instance of geometry
        Geometry NewGeo = {};
        NewGeo.Size = IndexCount;

        // generate my buffers
        glGenVertexArrays(1, &NewGeo.Vao); // my awesome Vertex Array Object
        glGenBuffers(1, &NewGeo.Vbo);      // my awesome Vertex Buffer Object (vertex data)
        glGenBuffers(1, &NewGeo.Ibo);      // my awesome Index Buffer Object (indices that form triangles)

        // bind my buffers
        glBindVertexArray(NewGeo.Vao);
        glBindBuffer(GL_ARRAY_BUFFER, NewGeo.Vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewGeo.Ibo);

        // buffer daa to the bound buffers
        glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(Vertex), Verts, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, VertCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

        // describe what was buffered
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        // get your ass back her when UVs are born

        // unbind my buffers to prevent accidental modifciation
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // return my awesome geometry
        return NewGeo;
    }

    void aie::FreeGeometry(Geometry& Geo)
    {
        glDeleteBuffers(1, &Geo.Vbo);
        glDeleteBuffers(1, &Geo.Ibo);
        glDeleteVertexArrays(1, &Geo.Vao);

        Geo = {};
    }

    Texture MakeTexture(unsigned int width, unsigned int height, unsigned int channels, const unsigned char* pixels)
    {
        Texture returnValue = { 0, width, height, channels };

        GLenum oglFormat = GL_RED;
        switch (channels)
        {
        case 1:
            oglFormat = GL_RED;
            break;
        case 2:
            oglFormat = GL_RG;
            break;
        case 3:
            oglFormat = GL_RGB;
            break;
        case 4:
            oglFormat = GL_RGBA;
            break;
        }

        glGenTextures(1, &returnValue.Handle);
        glBindTexture(GL_TEXTURE_2D, returnValue.Handle);

        glTexImage2D(
            GL_TEXTURE_2D,      // texture type
            0,
            oglFormat,          // color format
            width,              // width
            height,             // height
            0,
            oglFormat,          // color format
            GL_UNSIGNED_BYTE,   // type of data
            pixels);            // pointer to data

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        return returnValue;
    }

    Texture LoadTexture(const char* imagePath)
    {
        int imageWidth, imageHeight, imageFormat;
        unsigned char* rawPixelData = nullptr;

        Texture newTexture = {};

        stbi_set_flip_vertically_on_load(true);

        rawPixelData = stbi_load(imagePath,
            &imageWidth,
            &imageHeight,
            &imageFormat,
            STBI_default);

        assert(rawPixelData != nullptr && "Image failed to load.");
        newTexture = MakeTexture(imageWidth, imageHeight, imageFormat, rawPixelData);

        // free data after, and i must emphasize, *after* buffering texture to GPU
        stbi_image_free(rawPixelData);

        return newTexture;
    }

    void FreeTexture(Texture& tex)
    {
        glDeleteTextures(1, &tex.Handle);
        tex = {};
    }

    const char* aie::ReadShader(const char* Path)
    {
        std::string* Contents = new std::string();

        if (auto File = std::fstream(Path, std::ios::in))
        {
            /* run until we hit end of file (EOF)*/
            while (!File.eof())
            {
                std::string Line;
                std::getline(File, Line);
                Contents->append(Line);
                Contents->append("\n");
            }

            File.close();
        }
        else
        {
            return nullptr;
        }

        return Contents->c_str();
    }

    Shader MakeShader(const char* vertSource, const char* fragSource)
    {
        // make shaders
        Shader newShad = {};
        newShad.Program = glCreateProgram();

        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

        // compile shaders
        glShaderSource(vert, 1, &vertSource, 0);
        glShaderSource(frag, 1, &fragSource, 0);
        glCompileShader(vert);
        glCompileShader(frag);

        // attach & link shaders
        glAttachShader(newShad.Program, vert);
        glAttachShader(newShad.Program, frag);

        glLinkProgram(newShad.Program);

        // delete the shaders
        glDeleteShader(vert);
        glDeleteShader(frag);

        return newShad;
    }

    void FreeShader(Shader& shad)
    {
        glDeleteProgram(shad.Program);
        shad = {};
    }

    void SetUniform(const Shader& shad, GLuint location, const glm::mat4 value)
    {
        glProgramUniformMatrix4fv(shad.Program, location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void SetUniform(const Shader& shad, GLuint location, const float value)
    {
        glProgramUniform1fv(shad.Program, location, 1, &value);
    }

    void Draw(const Shader& shad, const Geometry& geo)
    {
        // bind the shader program & VAO
        glUseProgram(shad.Program);
        glBindVertexArray(geo.Vao);

        glDrawElements(GL_TRIANGLES, geo.Size, GL_UNSIGNED_INT, 0);
    }
}
