#ifndef PISTA_H
#define PISTA_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath> // Required for sqrt

class Pista {
private:
    float trackWidth; // Ancho predeterminado para un carril
    float outerSize;  // Tamaño exterior de la pista
    float innerSize;  // Tamaño interior de la pista
    float sidewalkWidth; // Ancho de las veredas
    float centerDividerWidth; // Ancho del divisor central
    
public:
    // Constructor
    // Se ha ajustado el ancho de la pista (width) y el tamaño total (outer, inner)
    // Ahora la pista es más ancha (total 8.0f de ancho, 4.0f por carril)
    Pista(float width = 4.0f, float outer = 65.0f, float inner = 49.0f, float sidewalk = 2.0f, float divider = 1.5f) 
        : trackWidth(width), outerSize(outer), innerSize(inner), sidewalkWidth(sidewalk), centerDividerWidth(divider) {}
    
    // Destructor
    ~Pista() {}
    
    // Método para dibujar la pista cuadrada
    void draw() {
        drawSquareTrack();
    }
    
    // Getters para acceder a las dimensiones
    float getTrackWidth() const { return trackWidth; }
    float getOuterSize() const { return outerSize; }
    float getInnerSize() const { return innerSize; }
    float getSidewalkWidth() const { return sidewalkWidth; }
    float getCenterDividerWidth() const { return centerDividerWidth; }
    
    // Setters para modificar las dimensiones
    void setTrackWidth(float width) { trackWidth = width; }
    void setOuterSize(float outer) { outerSize = outer; }
    void setInnerSize(float inner) { innerSize = inner; }
    void setSidewalkWidth(float sidewalk) { sidewalkWidth = sidewalk; }
    void setCenterDividerWidth(float divider) { centerDividerWidth = divider; }
    
private:
    // Función auxiliar para dibujar un segmento de línea discontinua
    // Dibuja una serie de pequeños quads para simular una línea discontinua
    void drawDashedLine(float x1, float z1, float x2, float z2, float y, float segmentLength, float gapLength) {
        float dx = x2 - x1;
        float dz = z2 - z1;
        float totalLength = sqrt(dx * dx + dz * dz); // Calcula la longitud total de la línea
        float currentLength = 0.0f;

        // Normaliza el vector de dirección
        float unitDx = dx / totalLength;
        float unitDz = dz / totalLength;

        // Ancho medio de la línea para los segmentos discontinuos
        float halfLineWidth = 0.1f; // Esto determina el grosor de la línea discontinua

        while (currentLength < totalLength) {
            // Calcula los puntos de inicio y fin del segmento de guion actual
            float currentX1 = x1 + unitDx * currentLength;
            float currentZ1 = z1 + unitDz * currentLength;
            float currentX2 = x1 + unitDx * (currentLength + segmentLength);
            float currentZ2 = z1 + unitDz * (currentLength + segmentLength);

            // Asegúrate de no dibujar más allá del final de la línea
            if (currentLength + segmentLength > totalLength) {
                currentX2 = x2;
                currentZ2 = z2;
            }

            // Calcula el vector perpendicular para el ancho del quad
            float perpDx = -unitDz * halfLineWidth;
            float perpDz = unitDx * halfLineWidth;

            // Dibuja el quad para el segmento de línea
            glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f); // Normal para una superficie plana
            glVertex3f(currentX1 - perpDx, y, currentZ1 - perpDz);
            glVertex3f(currentX1 + perpDx, y, currentZ1 + perpDz);
            glVertex3f(currentX2 + perpDx, y, currentZ2 + perpDz);
            glVertex3f(currentX2 - perpDx, y, currentZ2 - perpDz);
            glEnd();

            currentLength += (segmentLength + gapLength); // Avanza al inicio del siguiente segmento
        }
    }

    // Función para dibujar las veredas exteriores
    void drawOuterSidewalks() {
        // Color gris claro para las veredas
        glColor3f(0.7f, 0.7f, 0.7f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        
        float sidewalkHeight = 0.005f; // Altura de las veredas
        float outerSidewalk = outerSize + sidewalkWidth;
        
        // Vereda superior (Norte)
        glBegin(GL_QUADS);
        glVertex3f(-outerSidewalk, sidewalkHeight, outerSidewalk);
        glVertex3f(outerSidewalk, sidewalkHeight, outerSidewalk);
        glVertex3f(outerSidewalk, sidewalkHeight, outerSize);
        glVertex3f(-outerSidewalk, sidewalkHeight, outerSize);
        glEnd();
        
        // Vereda inferior (Sur)
        glBegin(GL_QUADS);
        glVertex3f(-outerSidewalk, sidewalkHeight, -outerSize);
        glVertex3f(outerSidewalk, sidewalkHeight, -outerSize);
        glVertex3f(outerSidewalk, sidewalkHeight, -outerSidewalk);
        glVertex3f(-outerSidewalk, sidewalkHeight, -outerSidewalk);
        glEnd();
        
        // Vereda izquierda (Oeste)
        glBegin(GL_QUADS);
        glVertex3f(-outerSidewalk, sidewalkHeight, outerSize);
        glVertex3f(-outerSize, sidewalkHeight, outerSize);
        glVertex3f(-outerSize, sidewalkHeight, -outerSize);
        glVertex3f(-outerSidewalk, sidewalkHeight, -outerSize);
        glEnd();
        
        // Vereda derecha (Este)
        glBegin(GL_QUADS);
        glVertex3f(outerSize, sidewalkHeight, outerSize);
        glVertex3f(outerSidewalk, sidewalkHeight, outerSize);
        glVertex3f(outerSidewalk, sidewalkHeight, -outerSize);
        glVertex3f(outerSize, sidewalkHeight, -outerSize);
        glEnd();
        
        // Veredas interiores (alrededor del área central)
        glColor3f(0.6f, 0.6f, 0.6f); // Color ligeramente más oscuro para distinguir
        float innerSidewalk = innerSize - sidewalkWidth;
        
        // Vereda interior superior (Norte)
        glBegin(GL_QUADS);
        glVertex3f(-innerSize, sidewalkHeight, innerSize);
        glVertex3f(innerSize, sidewalkHeight, innerSize);
        glVertex3f(innerSize, sidewalkHeight, innerSidewalk);
        glVertex3f(-innerSize, sidewalkHeight, innerSidewalk);
        glEnd();
        
        // Vereda interior inferior (Sur)
        glBegin(GL_QUADS);
        glVertex3f(-innerSize, sidewalkHeight, -innerSidewalk);
        glVertex3f(innerSize, sidewalkHeight, -innerSidewalk);
        glVertex3f(innerSize, sidewalkHeight, -innerSize);
        glVertex3f(-innerSize, sidewalkHeight, -innerSize);
        glEnd();
        
        // Vereda interior izquierda (Oeste)
        glBegin(GL_QUADS);
        glVertex3f(-innerSize, sidewalkHeight, innerSidewalk);
        glVertex3f(-innerSidewalk, sidewalkHeight, innerSidewalk);
        glVertex3f(-innerSidewalk, sidewalkHeight, -innerSidewalk);
        glVertex3f(-innerSize, sidewalkHeight, -innerSidewalk);
        glEnd();
        
        // Vereda interior derecha (Este)
        glBegin(GL_QUADS);
        glVertex3f(innerSidewalk, sidewalkHeight, innerSidewalk);
        glVertex3f(innerSize, sidewalkHeight, innerSidewalk);
        glVertex3f(innerSize, sidewalkHeight, -innerSidewalk);
        glVertex3f(innerSidewalk, sidewalkHeight, -innerSidewalk);
        glEnd();
    }

    // Función para dibujar el divisor central con pasto
    void drawCenterDivider() {
        // Color verde para el pasto del divisor central
        glColor3f(0.2f, 0.6f, 0.2f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        
        float dividerHeight = 0.03f; // Altura del divisor central
        float halfDivider = centerDividerWidth / 2.0f;
        
        // Calcula el centro de la pista
        float centerPos = (outerSize + innerSize) / 2.0f;
        
        // Divisor central superior (Norte)
        glBegin(GL_QUADS);
        glVertex3f(-outerSize, dividerHeight, centerPos + halfDivider);
        glVertex3f(outerSize, dividerHeight, centerPos + halfDivider);
        glVertex3f(outerSize, dividerHeight, centerPos - halfDivider);
        glVertex3f(-outerSize, dividerHeight, centerPos - halfDivider);
        glEnd();
        
        // Divisor central inferior (Sur)
        glBegin(GL_QUADS);
        glVertex3f(-outerSize, dividerHeight, -centerPos + halfDivider);
        glVertex3f(outerSize, dividerHeight, -centerPos + halfDivider);
        glVertex3f(outerSize, dividerHeight, -centerPos - halfDivider);
        glVertex3f(-outerSize, dividerHeight, -centerPos - halfDivider);
        glEnd();
        
        // Divisor central izquierdo (Oeste)
        glBegin(GL_QUADS);
        glVertex3f(-centerPos - halfDivider, dividerHeight, innerSize);
        glVertex3f(-centerPos + halfDivider, dividerHeight, innerSize);
        glVertex3f(-centerPos + halfDivider, dividerHeight, -innerSize);
        glVertex3f(-centerPos - halfDivider, dividerHeight, -innerSize);
        glEnd();
        
        // Divisor central derecho (Este)
        glBegin(GL_QUADS);
        glVertex3f(centerPos - halfDivider, dividerHeight, innerSize);
        glVertex3f(centerPos + halfDivider, dividerHeight, innerSize);
        glVertex3f(centerPos + halfDivider, dividerHeight, -innerSize);
        glVertex3f(centerPos - halfDivider, dividerHeight, -innerSize);
        glEnd();
    }

    void drawSquareTrack() {
        // Primero dibuja las veredas exteriores
        drawOuterSidewalks();
        
        // Color principal de la pista (gris)
        glColor3f(0.4f, 0.4f, 0.4f);
        glNormal3f(0.0f, 1.0f, 0.0f); // Normal para una superficie plana

        // Pista superior (Norte) - Quad gris sólido
        glBegin(GL_QUADS);
        glVertex3f(-outerSize, 0.01f, outerSize);
        glVertex3f(outerSize, 0.01f, outerSize);
        glVertex3f(outerSize, 0.01f, innerSize);
        glVertex3f(-outerSize, 0.01f, innerSize);
        glEnd();
        
        // Pista inferior (Sur) - Quad gris sólido
        glBegin(GL_QUADS);
        glVertex3f(-outerSize, 0.01f, -innerSize);
        glVertex3f(outerSize, 0.01f, -innerSize);
        glVertex3f(outerSize, 0.01f, -outerSize);
        glVertex3f(-outerSize, 0.01f, -outerSize);
        glEnd();
        
        // Pista izquierda (Oeste) - Quad gris sólido
        glBegin(GL_QUADS);
        glVertex3f(-outerSize, 0.01f, innerSize);
        glVertex3f(-innerSize, 0.01f, innerSize);
        glVertex3f(-innerSize, 0.01f, -innerSize);
        glVertex3f(-outerSize, 0.01f, -innerSize);
        glEnd();
        
        // Pista derecha (Este) - Quad gris sólido
        glBegin(GL_QUADS);
        glVertex3f(innerSize, 0.01f, innerSize);
        glVertex3f(outerSize, 0.01f, innerSize);
        glVertex3f(outerSize, 0.01f, -innerSize);
        glVertex3f(innerSize, 0.01f, -innerSize);
        glEnd();
        
        // Dibuja el divisor central con pasto
        drawCenterDivider();
        
        // Líneas amarillas discontinuas para división de carriles
        glColor3f(1.0f, 1.0f, 0.0f); // Color amarillo
        float lineY = 0.02f; // Ligeramente por encima de la pista para asegurar visibilidad
        float segmentLength = 4.0f; // Longitud de cada segmento de guion
        float gapLength = 4.0f;     // Longitud del espacio entre guiones

        // Calcula el ancho total de la pista y el ancho de cada carril
        float totalTrackWidth = outerSize - innerSize; 
        float laneWidth = totalTrackWidth / 2.0f; // Ahora cada carril tiene este ancho

        // Posiciones para las dos líneas discontinuas dentro de cada segmento de pista
        // Una línea en el centro de cada carril
        float zLine1 = innerSize + (laneWidth / 2.0f); // Centro del primer carril
        float zLine2 = innerSize + laneWidth + (laneWidth / 2.0f); // Centro del segundo carril

        // Para los segmentos Este/Oeste, las coordenadas X serán similares
        float xLine1 = innerSize + (laneWidth / 2.0f);
        float xLine2 = innerSize + laneWidth + (laneWidth / 2.0f);

        // Líneas superiores (Norte) - Dos líneas discontinuas
        drawDashedLine(-outerSize, zLine1, outerSize, zLine1, lineY, segmentLength, gapLength);
        drawDashedLine(-outerSize, zLine2, outerSize, zLine2, lineY, segmentLength, gapLength);
        
        // Líneas inferiores (Sur) - Dos líneas discontinuas
        drawDashedLine(-outerSize, -zLine1, outerSize, -zLine1, lineY, segmentLength, gapLength);
        drawDashedLine(-outerSize, -zLine2, outerSize, -zLine2, lineY, segmentLength, gapLength);
        
        // Líneas izquierdas (Oeste) - Dos líneas discontinuas
        drawDashedLine(-xLine1, innerSize, -xLine1, -innerSize, lineY, segmentLength, gapLength);
        drawDashedLine(-xLine2, innerSize, -xLine2, -innerSize, lineY, segmentLength, gapLength);
        
        // Líneas derechas (Este) - Dos líneas discontinuas
        drawDashedLine(xLine1, innerSize, xLine1, -innerSize, lineY, segmentLength, gapLength);
        drawDashedLine(xLine2, innerSize, xLine2, -innerSize, lineY, segmentLength, gapLength);
    }
};

#endif // PISTA_H
