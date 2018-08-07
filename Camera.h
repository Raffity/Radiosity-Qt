#ifndef CAMERA_H
#define CAMERA_H

#include "GL/gl.h"
#include "vec3.h"

struct Camera {
    vec3 position;
    float pitch;
    float yaw;
    double sX=1,sY=1,sZ=1;

    void applyTransformation ()  {
        glRotatef(-pitch, 1, 0, 0);
        glRotatef(-yaw, 0, 1, 0);
        glTranslatef(-position.x, -position.y, -position.z);
        glScaled(sX,sY,sZ);
    }
};

#endif // CAMERA_H
