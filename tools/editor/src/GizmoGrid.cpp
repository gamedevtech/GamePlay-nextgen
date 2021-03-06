#include "GizmoGrid.h"

static const unsigned int LINE_COUNT = 81;


GizmoGrid::GizmoGrid()
{
}

GizmoGrid::~GizmoGrid()
{
}

void GizmoGrid::initialize()
{
    _gridModel = createGridModel(LINE_COUNT);
    Material* material = Material::create("res/shaders/colored.vert", "res/shaders/colored.frag", "VERTEX_COLOR");
    GP_ASSERT(material && material->getStateBlock());
    _gridModel->setMaterial(material);
    _paramWorldViewProjection = material->getParameter("u_worldViewProjectionMatrix");
}

void GizmoGrid::finalize()
{
    SAFE_RELEASE(_gridModel);
}

void GizmoGrid::update(float elapsedTime)
{
}

void GizmoGrid::render(float elapsedTime)
{
    if (_paramWorldViewProjection)
    {
        Matrix worldViewProjection = Scene::getScene()->getActiveCamera()->getViewProjectionMatrix();
        _paramWorldViewProjection->setValue(worldViewProjection);
    }
}

Mesh* GizmoGrid::createGridMesh(unsigned int lineCount)
{
    // There needs to be an odd number of lines
    lineCount |= 1;
    const unsigned int pointCount = lineCount * 4;
    const unsigned int verticesSize = pointCount * (3 + 3);

    std::vector<float> vertices;
    vertices.resize(verticesSize);

    const float gridLength = (float)(lineCount / 2);
    float value = -gridLength;
    for (unsigned int i = 0; i < verticesSize; ++i)
    {
        // Default line color is dark grey
        Vector4 color(0.3f, 0.3f, 0.3f, 1.0f);

        // Very 10th line is brighter grey
        if (((int)value) % 10 == 0)
        {
            color.set(0.45f, 0.45f, 0.45f, 1.0f);
        }

        // The Z axis is blue
        if (value == 0.0f)
        {
            color.set(0.15f, 0.15f, 0.7f, 1.0f);
        }

        // Build the lines
        vertices[i] = value;
        vertices[++i] = 0.0f;
        vertices[++i] = -gridLength;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        vertices[++i] = value;
        vertices[++i] = 0.0f;
        vertices[++i] = gridLength;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        // The X axis is red
        if (value == 0.0f)
        {
            color.set(0.7f, 0.15f, 0.15f, 1.0f);
        }
        vertices[++i] = -gridLength;
        vertices[++i] = 0.0f;
        vertices[++i] = value;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        vertices[++i] = gridLength;
        vertices[++i] = 0.0f;
        vertices[++i] = value;
        vertices[++i] = color.x;
        vertices[++i] = color.y;
        vertices[++i] = color.z;

        value += 1.0f;
    }
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), pointCount, false);
    if (mesh == NULL)
    {
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::LINES);
    mesh->setVertexData(&vertices[0], 0, pointCount);

    return mesh;
}

Model* GizmoGrid::createGridModel(unsigned int lineCount)
{
    Mesh* mesh = createGridMesh(lineCount);
    if (!mesh)
        return NULL;
    Model* model = Model::create(mesh);
    mesh->release();
    assert(model);
    return model;
}
