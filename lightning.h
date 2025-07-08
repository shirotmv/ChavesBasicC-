// lightning.h
#ifndef LIGHTNING_H
#define LIGHTNING_H

#include <GL/glut.h>

class Lightning {
private:
    bool enabled;

public:
    Lightning() : enabled(true) {}

    void setup() {
        if (!enabled) return;

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        // Aumentar la luz ambiental para ver mejor los colores
        GLfloat ambientLight[] = {0.8f, 0.8f, 0.8f, 1.0f}; // Más luz ambiental
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

        GLfloat lightPos[] = {10.0f, 15.0f, 10.0f, 0.0f};
        GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

        GLfloat matSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat matShininess[] = {32.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

        glShadeModel(GL_SMOOTH);
        glEnable(GL_NORMALIZE);
    }

    void cleanup() {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_NORMALIZE);
    }

    bool isEnabled() const { return enabled; }
};

#endif // LIGHTNING_H
