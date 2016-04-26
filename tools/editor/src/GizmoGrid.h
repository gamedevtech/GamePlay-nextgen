#ifndef GIZMOGRID_H
#define GIZMOGRID_H

#include "gameplay.h"
using namespace gameplay;

/**
 * Creates a grid gizmo for drawing
 * a grid of lines in the editor window.
 */
class GizmoGrid
{
public:
    /**
     * Cosntructor.
     */
    GizmoGrid();

    /**
     * Destructor.
     */
    ~GizmoGrid();

    /**
     * @see Gizmo::inialize
     */
    void initialize();

    /**
     * @see Gizmo::finalize
     */
    void finalize();

    /**
     * @see Gizmo::update
     */
    void update(float elapsedTime);

    /**
     * @see Gizmo::render
     */
    void render(float elapsedTime);

private:
    Mesh* createGridMesh(unsigned int lineCount);
    Model* createGridModel(unsigned int lineCount);

    Model* _gridModel;
    MaterialParameter* _paramWorldViewProjection;
};

#endif
