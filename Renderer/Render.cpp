#include "Render.h"

#include <fstream>
#include <string>
#include <vector>

#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION 1
#include "ThirdParty/stb_image.h"

#include "Util.h"
#include "tiny_obj_loader.h"
#include <iostream>

namespace aie
{
    Geometry aie::MakeGeometry(
        const Vertex* const Verts,
        GLsizei VertCount,
        const GLuint* const indices,
        GLsizei IndexCount)
    {
        // create an instance of geometry
        Geometry NewGeo = {};
        NewGeo.Size = IndexCount;

        // generate my buffers
        glGenVertexArrays(1, &NewGeo.Vao); // my awesome Vertex Array Object
        glGenBuffers(1, &NewGeo.Vbo);      // my awesome Vertex Buffer Object (vertex data)
        glGenBuffers(1, &NewGeo.Ibo);      // my awesome Index Buffer Object (indices that form triangles)

        for (int i = 0; i < VertCount; i++)
        {
            assert(Verts[i].Normal.length() != 1.0f && "Invalid normals!");
        }

        // bind my buffers
        glBindVertexArray(NewGeo.Vao);
        glBindBuffer(GL_ARRAY_BUFFER, NewGeo.Vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewGeo.Ibo);

        // buffer data to the bound buffers
        glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(Vertex), Verts, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, VertCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

        // describe what was buffered
        // position (location 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // UVs (location 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UVs));
        // normals (location 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // unbind my buffers to prevent accidental modifciation
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // return my awesome geometry
        return NewGeo;
    }

    Geometry aie::LoadGeometry(const char* filePath)
    {
        using namespace tinyobj;

        // contains all data for all vertex attributes loaded from the file
        attrib_t vertexAttributes;

        // enumeration of all shapes in obj file
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string error;
        std::string warn;

        bool success = LoadObj(&vertexAttributes, &shapes, &materials, &warn, &error, filePath);

        if (!error.empty())
        {
            std::cerr << error << std::endl;
        }
        if (!success || shapes.size() < 1)
        {
            return {};
        }

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        // Get mesh data
        size_t offSet = 0;
        for (GLsizei i = 0; i < shapes[0].mesh.num_face_vertices.size(); ++i)
        {
            // number of vertices for current face
            GLsizei faceVertices = shapes[0].mesh.num_face_vertices[i];

            assert(faceVertices == 3 && "Faces must be provided in triangles");

            // iterate over vertices used to form current face
            for (unsigned char j = 0; j < faceVertices; ++j)
            {
                tinyobj::index_t idx = shapes[0].mesh.indices[offSet + j];

                tinyobj::real_t vertX = vertexAttributes.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vertY = vertexAttributes.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vertZ = vertexAttributes.vertices[3 * idx.vertex_index + 2];

                tinyobj::real_t normalX = vertexAttributes.normals[3 * idx.normal_index + 0];
                tinyobj::real_t normalY = vertexAttributes.normals[3 * idx.normal_index + 1];
                tinyobj::real_t normalZ = vertexAttributes.normals[3 * idx.normal_index + 2];

                tinyobj::real_t tx = vertexAttributes.texcoords[2 * idx.texcoord_index + 0];
                tinyobj::real_t ty = vertexAttributes.texcoords[2 * idx.texcoord_index + 1];

                vertices.push_back(Vertex{ {vertX, vertY, vertZ, 1}, {tx, ty}, {normalX, normalY, normalZ} });
                indices.push_back(faceVertices * i + j);
            }
            offSet += faceVertices;
        }

        assert(vertices.size() <= std::numeric_limits<GLsizei>::max());
        assert(indices.size() <= std::numeric_limits<GLsizei>::max());

        return MakeGeometry(&vertices[0], (GLsizei)vertices.size(), &indices[0], (GLsizei)shapes[0].mesh.indices.size());
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

    Shader ReadShaderFromFiles(const char* vertShaderPath, const char* fragShaderPath)
    {
        return MakeShader(aie::DumpToString(vertShaderPath).c_str(), aie::DumpToString(fragShaderPath).c_str());
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

    void SetUniform(const Shader& shad, GLuint location, const Texture& texture, int textureSlot)
    {
        // Specify the texture slot we want to use
        glActiveTexture(GL_TEXTURE0 + textureSlot);

        // Bind the texture to that slot
        glBindTexture(GL_TEXTURE_2D, texture.Handle);

        // Specify that texture slot as the value for the uniform
        glProgramUniform1i(shad.Program, location, textureSlot);
    }

    void SetUniform(const Shader& shad, GLuint location, const glm::vec3 value)
    {
        glProgramUniform3fv(shad.Program, location, 1, glm::value_ptr(value));
    }

    void Draw(const Shader& shad, const Geometry& geo)
    {
        // bind the shader program & VAO
        glUseProgram(shad.Program);
        glBindVertexArray(geo.Vao);

        glDrawElements(GL_TRIANGLES, geo.Size, GL_UNSIGNED_INT, 0);
    }
}
