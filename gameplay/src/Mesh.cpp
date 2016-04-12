#include "Base.h"
#include "Mesh.h"
#include "MeshPart.h"
#include "Effect.h"
#include "Model.h"
#include "Material.h"

namespace gameplay
{

Mesh::Mesh() :
    _primitiveType(TRIANGLES),
    _vertexCount(0), _vertexBuffer(0),
    _partCount(0), _parts(nullptr), _dynamic(false)
{
}

Mesh::Mesh(const VertexFormat& vertexFormat) :
    _primitiveType(TRIANGLES), _vertexFormat(vertexFormat),
    _vertexCount(0), _vertexBuffer(0),
    _partCount(0), _parts(nullptr), _dynamic(false)
{
}

Mesh::~Mesh()
{
    if (_parts)
    {
        for (unsigned int i = 0; i < _partCount; ++i)
        {
            SAFE_DELETE(_parts[i]);
        }
        SAFE_DELETE_ARRAY(_parts);
    }

    if (_vertexBuffer)
    {
        glDeleteBuffers(1, &_vertexBuffer);
        _vertexBuffer = 0;
    }
}

Mesh* Mesh::createMesh(const VertexFormat& vertexFormat, unsigned int vertexCount, bool dynamic)
{
    GLuint vbo;
    GL_ASSERT( glGenBuffers(1, &vbo) );
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, vbo) );
    GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize() * vertexCount, nullptr, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );

    Mesh* mesh = new Mesh(vertexFormat);
    mesh->_vertexCount = vertexCount;
    mesh->_vertexBuffer = vbo;
    mesh->_dynamic = dynamic;

    return mesh;
}


Mesh* Mesh::createQuad(float x, float y, float width, float height, float s1, float t1, float s2, float t2)
{
    float x2 = x + width;
    float y2 = y + height;

    float vertices[] =
    {
        x, y2, 0,   0, 0, 1,    s1, t2,
        x, y, 0,    0, 0, 1,    s1, t1,
        x2, y2, 0,  0, 0, 1,    s2, t2,
        x2, y, 0,   0, 0, 1,    s2, t1,
    };
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), 4, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        return nullptr;
    }
    mesh->_primitiveType = TRIANGLE_STRIP;
    mesh->setVertexData(vertices, 0, 4);

    return mesh;
}

Mesh* Mesh::createQuadFullscreen()
{
    float x = -1.0f;
    float y = -1.0f;
    float x2 = 1.0f;
    float y2 = 1.0f;

    float vertices[] =
    {
        x, y2,   0, 1,
        x, y,    0, 0,
        x2, y2,  1, 1,
        x2, y,   1, 0
    };
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 2),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), 4, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        return nullptr;
    }
    mesh->_primitiveType = TRIANGLE_STRIP;
    mesh->setVertexData(vertices, 0, 4);

    return mesh;
}

Mesh* Mesh::createQuad(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
{
    // Calculate the normal vector of the plane.
    Vector3 v1, v2, n;
    Vector3::subtract(p2, p1, &v1);
    Vector3::subtract(p3, p2, &v2);
    Vector3::cross(v1, v2, &n);
    n.normalize();

    float vertices[] =
    {
        p1.x, p1.y, p1.z, n.x, n.y, n.z, 0, 1,
        p2.x, p2.y, p2.z, n.x, n.y, n.z, 0, 0,
        p3.x, p3.y, p3.z, n.x, n.y, n.z, 1, 1,
        p4.x, p4.y, p4.z, n.x, n.y, n.z, 1, 0
    };
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), 4, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        return nullptr;
    }
    mesh->_primitiveType = TRIANGLE_STRIP;
    mesh->setVertexData(vertices, 0, 4);

    return mesh;
}

Mesh* Mesh::createLines(Vector3* points, unsigned int pointCount)
{
    GP_ASSERT(points);
    GP_ASSERT(pointCount);

    float* vertices = new float[pointCount*3];
    memcpy(vertices, points, pointCount*3*sizeof(float));

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 1), pointCount, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        SAFE_DELETE_ARRAY(vertices);
        return nullptr;
    }
    mesh->_primitiveType = LINE_STRIP;
    mesh->setVertexData(vertices, 0, pointCount);

    SAFE_DELETE_ARRAY(vertices);
    return mesh;
}

Mesh* Mesh::createBoundingBox(const BoundingBox& box)
{
    Vector3 corners[8];
    box.getCorners(corners);

    float vertices[] =
    {
        corners[7].x, corners[7].y, corners[7].z,
        corners[6].x, corners[6].y, corners[6].z,
        corners[1].x, corners[1].y, corners[1].z,
        corners[0].x, corners[0].y, corners[0].z,
        corners[7].x, corners[7].y, corners[7].z,
        corners[4].x, corners[4].y, corners[4].z,
        corners[3].x, corners[3].y, corners[3].z, 
        corners[0].x, corners[0].y, corners[0].z,
        corners[0].x, corners[0].y, corners[0].z,
        corners[1].x, corners[1].y, corners[1].z,
        corners[2].x, corners[2].y, corners[2].z,
        corners[3].x, corners[3].y, corners[3].z, 
        corners[4].x, corners[4].y, corners[4].z,
        corners[5].x, corners[5].y, corners[5].z, 
        corners[2].x, corners[2].y, corners[2].z,
        corners[1].x, corners[1].y, corners[1].z,
        corners[6].x, corners[6].y, corners[6].z,
        corners[5].x, corners[5].y, corners[5].z
    };
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 1), 18, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        return nullptr;
    }
    mesh->_primitiveType = LINE_STRIP;
    mesh->setVertexData(vertices, 0, 18);

    return mesh;
}

Mesh* Mesh::createCube()
{
    static float vertices[] =
    {
        // Face front (A,B,C,D)
        -0.5f,-0.5f, 0.5f,      0.0f, 0.0f, 1.0f,       0.0f, 0.0f,     // A
        -0.5f,-0.5f, 0.5f,      0.0f, 0.0f, 1.0f,       1.0f, 0.0f,     // B
         0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,       1.0f, 1.0f,     // C
        -0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,       0.0f, 1.0f,     // D
        // Face back (E,H,G,F)
        -0.5f,-0.5f,-0.5f,      0.0f, 0.0f,-1.0f,       1.0f, 0.0f,     // E
        -0.5f, 0.5f,-0.5f,      0.0f, 0.0f,-1.0f,       0.0f, 0.0f,     // H
         0.5f, 0.5f,-0.5f,      0.0f, 0.0f,-1.0f,       0.0f, 1.0f,     // G
         0.5f,-0.5f,-0.5f,      0.0f, 0.0f,-1.0f,       1.0f, 1.0f,     // F
        // Face left (E,A,D,H)
        -0.5f,-0.5f,-0.5f,     -1.0f, 0.0f, 0.0f,       1.0f, 0.0f,		// E
        -0.5f,-0.5f, 0.5f,     -1.0f, 0.0f, 0.0f,       0.0f, 0.0f,     // A
        -0.5f, 0.5f, 0.5f,     -1.0f, 0.0f, 0.0f,       0.0f, 1.0f,     // D
        -0.5f, 0.5f,-0.5f,     -1.0f, 0.0f, 0.0f,       1.0f, 1.0f,     // H
        // Face right (B,F,G,C)
         0.5f,-0.5f, 0.5f,      1.0f, 0.0f, 0.0f,		1.0f, 0.0f,		// B
         0.5f,-0.5f,-0.5f,      1.0f, 0.0f, 0.0f,       0.0f, 0.0f,     // F
         0.5f, 0.5f,-0.5f,		1.0f, 0.0f, 0.0f,       0.0f, 1.0f,     // G
         0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,       1.0f, 1.0f,     // C
        // Face top (D,C,G,H)
        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,       0.0f, 1.0f,     // D
         0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,       1.0f, 1.0f,     // C
         0.5f, 0.5f,-0.5f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f,     // G
        -0.5f, 0.5f,-0.5f,      0.0f, 1.0f, 0.0f,   	0.0f, 0.0f,     // H
        // Face down (A,E,F,B)
        -0.5f,-0.5f,0.5f,       0.0f,-1.0f, 0.0f,       0.0f, 0.0f,     // A
        -0.5f,-0.5f,-0.5f,      0.0f,-1.0f, 0.0f,       1.0f, 0.0f,     // E
         0.5f,-0.5f,-0.5f,      0.0f,-1.0f, 0.0f,       1.0f, 1.0f,     // F
         0.5f,-0.5f,0.5f,       0.0f,-1.0f, 0.0f,       0.0f, 1.0f      // B
    };
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), 24, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        return nullptr;
    }
    mesh->_primitiveType = PrimitiveType::TRIANGLES;
    mesh->setVertexData((void*)vertices, 0, 24);
    // MeshPart
    static unsigned char indices[] =
    {
        0,1,2,          0,2,3,          // Face front
        4,5,6,          4,6,7,          // Face back
        8,9,10,         8,10,11,        // Face left
        12,13,14,		12,14,15,       // Face right
        16,17,18,		16,18,19,       // Face top
        20,21,22,		20,22,23        // Face down
    };
    MeshPart* part = mesh->addPart(PrimitiveType::TRIANGLES, IndexFormat::INDEX16, 12, false);
    part->setIndexData((void*)indices, 0, 12);

    return mesh;
}

Mesh* Mesh::createCylinder(unsigned int divs)
{
    unsigned int vertexCount = (divs + 1) * 2;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        return nullptr;
    }
    mesh->_primitiveType = PrimitiveType::TRIANGLES;

    float height = 0.0;
    float angle = 0.0;
    float angleIncrement = 20.0f * M_PI / divs;
    unsigned int vertexSize = mesh->getVertexSize();
    float* vertices = new float[vertexCount * vertexSize];
    for(unsigned int j = 0; j <= 1; j++, height += 2.0f)
    {
        for (unsigned int i = 0; i <= divs; i++, angle += angleIncrement)
        {
            float cosPt = std::cosf(angle);
            float sinPt = std::sinf(angle);
            unsigned int offset = vertexSize * j * (divs + 1) + vertexSize * i;
            // Position
            vertices[offset]   = 1.0f * cosPt;
            vertices[offset+1] = height;
            vertices[offset+2] = 1.0f * sinPt;
            // Normal
            vertices[offset+3] = cosPt;
            vertices[offset+4] = 0.0;
            vertices[offset+5] = sinPt;
            // TexCoord
            vertices[offset+6] = i / (float)divs;
            vertices[offset+7] = j / (float)2.0f;
        }
    }
    mesh->setVertexData((void*)vertices, 0, vertexCount);
    SAFE_DELETE_ARRAY(vertices);
    // MeshPart
    unsigned int primitiveCount = divs * 2;
    unsigned char* indices = new unsigned char[3 * primitiveCount];
    for(unsigned int  j = 0; j < 1.0f; j++)
    {
        for (unsigned int i = 0; i < divs; i++)
        {
            unsigned int offset = j * divs * 6 + 6 * i;

            indices[offset]   = j * (divs + 1) + i;                 // A
            indices[offset+1] = j * (divs + 1) + (i + 1);           // B
            indices[offset+2] = (j + 1) * (divs + 1) + i;           // C

            indices[offset+3] = (j + 1) * (divs + 1) + i;           // C
            indices[offset+4] = j * (divs + 1) + (i + 1);           // B
            indices[offset+5] = (j + 1) * (divs + 1) + (i + 1);     // D
        }
    }
    MeshPart* part = mesh->addPart(PrimitiveType::TRIANGLES, IndexFormat::INDEX16, primitiveCount, false);
    part->setIndexData((void*)indices, 0, primitiveCount);

    return mesh;
}

Mesh* Mesh::createCone(unsigned int divs)
{
    unsigned int vertexCount = (divs + 1) * 2;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        return nullptr;
    }
    mesh->_primitiveType = PrimitiveType::TRIANGLE_STRIP;

    float angle = 0;
    float angleIncr = 2.0f * M_PI / divs;
    float angleToPlaneXZ = atan(2.0f / 1.0f);
    float cosToXZ = std::cosf(angleToPlaneXZ);
    float sinToXZ = std::sinf(angleToPlaneXZ);
    unsigned int vertexSize = mesh->getVertexSize();
    float* vertices = new float[vertexCount * vertexSize];
    for ( unsigned int i = 0; i <= divs; i++, angle += angleIncr)
    {
        float cosPt = std::cosf(angle);
        float sinPt = std::sinf(angle);
        unsigned int offset = vertexSize * i * 2;

        // Position
        vertices[offset]   = 1.0f * cosPt;
        vertices[offset+1] = 0.0;
        vertices[offset+2] = 1.0f * sinPt;
        // Normal
        vertices[offset+3  ] = cosPt * sinToXZ;
        vertices[offset+4] = cosToXZ;
        vertices[offset+5] = sinPt * cosToXZ;
        // TexCoord
        vertices[offset+6] = (float)(i / divs);
        vertices[offset+7] = 0.0;

        // Position
        vertices[offset+8] = 0.0;
        vertices[offset+9] = 2.0f;
        vertices[offset+10] = 0.0;
        // Normal
        vertices[offset+11] = cosPt * sinToXZ;
        vertices[offset+12] = cosToXZ;
        vertices[offset+13] = sinPt * cosToXZ;
        // TexCoord
        vertices[offset+14] = (float)(i / divs);
        vertices[offset+15] = 1.0;
    }
    mesh->setVertexData((void*)vertices, 0, vertexCount);
    SAFE_DELETE_ARRAY(vertices);

    return mesh;
}

Mesh* Mesh::createSphere(unsigned int divs)
{
    /*unsigned int vertexCount = (divs + 1) * 2;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, false);
    if (mesh == nullptr)
    {
        GP_ERROR("Failed to create mesh.");
        return nullptr;
    }
    mesh->_primitiveType = PrimitiveType::TRIANGLE



    return mesh;
    */
    return nullptr;
}

const char* Mesh::getUrl() const
{
    return _url.c_str();
}

const VertexFormat& Mesh::getVertexFormat() const
{
    return _vertexFormat;
}

unsigned int Mesh::getVertexCount() const
{
    return _vertexCount;
}

unsigned int Mesh::getVertexSize() const
{
    return _vertexFormat.getVertexSize();
}

VertexBufferHandle Mesh::getVertexBuffer() const
{
    return _vertexBuffer;
}

bool Mesh::isDynamic() const
{
    return _dynamic;
}

Mesh::PrimitiveType Mesh::getPrimitiveType() const
{
    return _primitiveType;
}

void Mesh::setPrimitiveType(PrimitiveType type)
{
    _primitiveType = type;
}

void* Mesh::mapVertexBuffer()
{
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );

    return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

bool Mesh::unmapVertexBuffer()
{
    return glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Mesh::setVertexData(const void* vertices, unsigned int vertexStart, unsigned int vertexCount)
{
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );

    if (vertexStart == 0 && vertexCount == 0)
    {
        GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, _vertexFormat.getVertexSize() * _vertexCount, vertices, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
    }
    else
    {
        if (vertexCount == 0)
        {
            vertexCount = _vertexCount - vertexStart;
        }

        GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, vertexStart * _vertexFormat.getVertexSize(), vertexCount * _vertexFormat.getVertexSize(), vertices) );
    }
}

MeshPart* Mesh::addPart(PrimitiveType primitiveType, IndexFormat indexFormat, unsigned int indexCount, bool dynamic)
{
    MeshPart* part = MeshPart::create(this, _partCount, primitiveType, indexFormat, indexCount, dynamic);
    if (part)
    {
        // Increase size of part array and copy old subets into it.
        MeshPart** oldParts = _parts;
        _parts = new MeshPart*[_partCount + 1];
        for (unsigned int i = 0; i < _partCount; ++i)
        {
            _parts[i] = oldParts[i];
        }

        // Add new part to array.
        _parts[_partCount++] = part;

        // Delete old part array.
        SAFE_DELETE_ARRAY(oldParts);
    }

    return part;
}

unsigned int Mesh::getPartCount() const
{
    return _partCount;
}

MeshPart* Mesh::getPart(unsigned int index)
{
    GP_ASSERT(_parts);
    return _parts[index];
}

const BoundingBox& Mesh::getBoundingBox() const
{
    return _boundingBox;
}

void Mesh::setBoundingBox(const BoundingBox& box)
{
    _boundingBox = box;
}

const BoundingSphere& Mesh::getBoundingSphere() const
{
    return _boundingSphere;
}

void Mesh::setBoundingSphere(const BoundingSphere& sphere)
{
    _boundingSphere = sphere;
}

const char* Mesh::getSerializedClassName() const
{
    return "gameplay::Mesh";
}

void Mesh::serialize(Serializer* serializer)
{
    serializer->writeString("url", _url.c_str(), "");
    serializer->writeEnum("primitiveType", "gameplay::Mesh::PrimitiveType", _primitiveType, Mesh::TRIANGLES);
    serializer->writeObject("vertexFormat", &_vertexFormat);
    serializer->writeInt("vertexCount", _vertexCount, 0);
    do
    {
        void* vertices = mapVertexBuffer();
        serializer->writeByteArray("vertices", (unsigned char*)vertices, _vertexFormat.getVertexSize() * _vertexCount);
    } while(!unmapVertexBuffer());
    serializer->writeBool("dynamic", _dynamic, true);
    serializer->writeObjectList("parts", _partCount);
    for (unsigned int i = 0; i < _partCount; i++)
    {
        serializer->writeObject(nullptr, _parts[i]);
    }
    serializer->writeObject("boundingBox", _boundingBox.isEmpty() ? nullptr : &_boundingBox);
    serializer->writeObject("boundingSphere", _boundingSphere.isEmpty() ? nullptr : &_boundingSphere);
}

void Mesh::deserialize(Serializer* serializer)
{
    serializer->readString("url", _url, "");
    _primitiveType = static_cast<Mesh::PrimitiveType>(serializer->readEnum("primitiveType", "gameplay::Mesh::PrimitiveType", Mesh::TRIANGLES));
    serializer->readObject("vertexFormat", &_vertexFormat);
    _vertexCount = serializer->readInt("vertexCount", 0);
    unsigned char* vertices = new unsigned char[_vertexFormat.getVertexSize() * _vertexCount];
    serializer->readByteArray("vertices", &vertices);
    _dynamic = serializer->readBool("dynamic", true);
    
    GLuint vbo;
    GL_ASSERT(glGenBuffers(1, &vbo));
    GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, _vertexFormat.getVertexSize() * _vertexCount, nullptr, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    
    _vertexBuffer = vbo;
    setVertexData((void*)vertices, 0, _vertexCount);
    SAFE_DELETE_ARRAY(vertices);
    
    _partCount = serializer->readObjectList("parts");
    _parts = new MeshPart*[_partCount];
    for (unsigned int i = 0; i < _partCount; i++)
    {
        _parts[i] = dynamic_cast<MeshPart*>(serializer->readObject(nullptr));
        _parts[i]->_mesh = this;
        _parts[i]->_meshIndex = i;
    }
    serializer->readObject("boundingBox", &_boundingBox);
    serializer->readObject("boundingSphere", &_boundingSphere);
}

Serializable* Mesh::createInstance()
{
    return static_cast<Serializable*>(new Mesh());
}

const char* Mesh::enumToString(const char* enumName, int value)
{
    if (std::strcmp("gameplay::Mesh::PrimitiveType", enumName) == 0)
    {
        switch (value)
        {
            case Mesh::TRIANGLES:
                return "TRIANGLES";
            case Mesh::TRIANGLE_STRIP:
                return "TRIANGLE_STRIP";
            case Mesh::LINES:
                return "LINES";
            case Mesh::LINE_STRIP:
                return "LINE_STRIP";
            case Mesh::POINTS:
                return "POINTS";
            default:
                return nullptr;
        }
    }
    else if (std::strcmp("gameplay::Mesh::IndexFormat", enumName) == 0)
    {
        switch (value)
        {
            case Mesh::INDEX8:
                return "INDEX8";
            case Mesh::INDEX16:
                return "INDEX16";
            case Mesh::INDEX32:
                return "INDEX32";
            default:
                return nullptr;
        }
    }
    return nullptr;
}

int Mesh::enumParse(const char* enumName, const char* str)
{
    if (std::strcmp("gameplay::Mesh::PrimitiveType", enumName) == 0)
    {
        if (std::strcmp("TRIANGLES", str) == 0)
            return Mesh::TRIANGLES;
        else if (std::strcmp("TRIANGLE_STRIP", str) == 0)
            return Mesh::TRIANGLE_STRIP;
        else if (std::strcmp("LINES", str) == 0)
            return Mesh::LINES;
        else if (std::strcmp("LINE_STRIP", str) == 0)
            return Mesh::LINE_STRIP;
        else if (std::strcmp("POINTS", str) == 0)
            return Mesh::POINTS;
    }
    else if (std::strcmp("gameplay::Mesh::IndexFormat", enumName) == 0)
    {
        if (std::strcmp("INDEX8", str) == 0)
            return Mesh::INDEX8;
        else if (std::strcmp("INDEX16", str) == 0)
            return Mesh::INDEX16;
        else if (std::strcmp("INDEX32", str) == 0)
            return Mesh::INDEX32;
    }
    return -1;
}

}
