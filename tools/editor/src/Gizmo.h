#ifndef GIZMO_H
#define GIZMO_H

/**
 * Defines a class for rendering visual indicators
 * to help with context based editing in the editor
 * window.
 */
class Gizmo
{
public:
    /**
     * Destructor.
     */
    virtual ~Gizmo();

    /**
     * Initializes the gizmo.
     */
    virtual void initialize() = 0;

    /**
     * Finalizes the gizmo.
     */
    virtual void finalize() = 0;

    /**
     * Updates the gizmo.
     * @param elapsedTime The elapased time.
     */
    virtual void update(float elapsedTime) = 0;

    /**
     * Renders the gizmo.
     * @param elapsedTime The elapsed time.
     */
    virtual void render(float elapsedTime) = 0;
};

#endif
