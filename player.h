#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include <cmath> // For cosf, sinf, sqrtf

class Camera {
private:
    float eyeX, eyeY, eyeZ;
    float pitch, yaw;
    float frontX, frontY, frontZ;
    float upX, upY, upZ;
    float rightX, rightY, rightZ;

    bool thirdPerson;
    float thirdPersonDistance;

    // Variables para física de salto
    float velocityY;
    bool isGrounded;
    float groundLevel;

    // Propiedades para el cuerpo del personaje
    float bodyRadius;
    int bodySlices;
    int bodyStacks;

    void updateVectors() {
        // Calculate front vector from pitch and yaw
        frontX = cosf(pitch * M_PI / 180.0f) * cosf(yaw * M_PI / 180.0f);
        frontY = sinf(pitch * M_PI / 180.0f);
        frontZ = cosf(pitch * M_PI / 180.0f) * sinf(yaw * M_PI / 180.0f);

        // Normalize front vector
        float lengthFront = sqrtf(frontX * frontX + frontY * frontY + frontZ * frontZ);
        if (lengthFront > 0.0f) {
            frontX /= lengthFront;
            frontY /= lengthFront;
            frontZ /= lengthFront;
        }

        float worldUpX = 0.0f;
        float worldUpY = 1.0f;
        float worldUpZ = 0.0f;

        // Calculate right vector as cross product of front and worldUp
        rightX = (frontY * worldUpZ) - (frontZ * worldUpY);
        rightY = (frontZ * worldUpX) - (frontX * worldUpZ);
        rightZ = (frontX * worldUpY) - (frontY * worldUpX);

        // Normalize right vector
        float lengthRight = sqrtf(rightX * rightX + rightY * rightY + rightZ * rightZ);
        if (lengthRight > 0.0f) {
            rightX /= lengthRight;
            rightY /= lengthRight;
            rightZ /= lengthRight;
        } else {
            rightX = 1.0f; rightY = 0.0f; rightZ = 0.0f;
        }

        // Calculate up vector as cross product of right and front
        upX = (rightY * frontZ) - (rightZ * frontY);
        upY = (rightZ * frontX) - (rightX * frontZ);
        upZ = (rightX * frontY) - (rightY * frontX);

        // Normalize up vector
        float lengthUp = sqrtf(upX * upX + upY * upY + upZ * upZ);
        if (lengthUp > 0.0f) {
            upX /= lengthUp;
            upY /= lengthUp;
            upZ /= lengthUp;
        }
    }

public:
    Camera() : eyeX(0.0f), eyeY(2.0f), eyeZ(5.0f),
               pitch(0.0f), yaw(-90.0f), // Yaw -90 makes it look down the -Z axis initially
               frontX(0.0f), frontY(0.0f), frontZ(-1.0f),
               upX(0.0f), upY(1.0f), upZ(0.0f),
               rightX(1.0f), rightY(0.0f), rightZ(0.0f),
               thirdPerson(false), thirdPersonDistance(8.0f),
               velocityY(0.0f), isGrounded(false), groundLevel(-0.1f),
               bodyRadius(0.33f), bodySlices(30), bodyStacks(30) {
        updateVectors();
        eyeY = groundLevel + bodyRadius + 0.1f; // Ajusta la altura inicial al radio del cuerpo + un poco más arriba
    }

    void setup() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        float aspect = (float)viewport[2] / (float)viewport[3];
        gluPerspective(60.0f, aspect, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if (thirdPerson) {
            // Calculate camera position for third-person view
            float camX = eyeX - frontX * thirdPersonDistance;
            float camY = eyeY - frontY * thirdPersonDistance + bodyRadius * 1.5f;
            float camZ = eyeZ - frontZ * thirdPersonDistance;

            gluLookAt(camX, camY, camZ,
                      eyeX, eyeY, eyeZ,
                      upX, upY, upZ);
        } else {
            // En primera persona, el ojo está en la posición del personaje.
            // Para evitar que la cámara esté exactamente en el centro de la esfera,
            // la elevamos ligeramente o ajustamos el punto de vista.
            // Aquí, la cámara mira desde la posición del ojo + front.
            gluLookAt(eyeX, eyeY, eyeZ,
                      eyeX + frontX, eyeY + frontY, eyeZ + frontZ,
                      upX, upY, upZ);
        }
    }

    void drawBody() {
        // Dibuja la esfera del personaje.
        // glPushMatrix y glPopMatrix son cruciales para no afectar el resto de la escena.
        glPushMatrix();
        // Mueve la esfera a la posición del personaje, ajustando para que la base esté en eyeY
        // El centro de la esfera será (eyeX, eyeY - bodyRadius, eyeZ)
        glTranslatef(eyeX, eyeY - bodyRadius, eyeZ);
        glColor3f(0.8f, 0.2f, 0.2f); // Color rojo
        glutSolidSphere(bodyRadius, bodySlices, bodyStacks);
        glPopMatrix();
    }

    // Método move() completamente reescrito para manejo correcto de colisiones
    void move(float forward, float right, float deltaTime) {
        // Only move on the XZ plane for horizontal movement
        float frontX_flat = frontX;
        float frontZ_flat = frontZ;
        float rightX_flat = rightX;
        float rightZ_flat = rightZ;

        float lengthFrontFlat = sqrtf(frontX_flat * frontX_flat + frontZ_flat * frontZ_flat);
        if (lengthFrontFlat > 0.0f) {
            frontX_flat /= lengthFrontFlat;
            frontZ_flat /= lengthFrontFlat;
        }

        float lengthRightFlat = sqrtf(rightX_flat * rightX_flat + rightZ_flat * rightZ_flat);
        if (lengthRightFlat > 0.0f) {
            rightX_flat /= lengthRightFlat;
            rightZ_flat /= lengthRightFlat;
        }

        // Movimiento horizontal
        eyeX += frontX_flat * forward + rightX_flat * right;
        eyeZ += frontZ_flat * forward + rightZ_flat * right;

        // FÍSICA VERTICAL MEJORADA
        float gravity = -32.8f;
        float groundOffset = bodyRadius + 0.35f;
        float groundY = groundLevel + groundOffset;

        // Aplicar gravedad
        if (!isGrounded) {
            velocityY += gravity * deltaTime;
        }

        // Aplicar velocidad vertical
        float newY = eyeY + velocityY * deltaTime;

        // Verificar colisión con el suelo básico
        if (newY <= groundY) {
            newY = groundY;
            if (velocityY < 0) {
                velocityY = 0.0f;
            }
            isGrounded = true;
        } else {
            isGrounded = false;
        }

        eyeY = newY;
    }

    // MEJORADO: Método específico para manejo de colisiones externas
    void handleCollision(float penetrationX, float penetrationY, float penetrationZ, 
                        float normalX, float normalY, float normalZ) {
        // Tolerancia para evitar micro-penetraciones
        const float COLLISION_TOLERANCE = 0.01f;
        
        // Ajustar posición según penetración con un pequeño margen adicional
        if (penetrationX != 0.0f) {
            eyeX += penetrationX + (penetrationX > 0 ? COLLISION_TOLERANCE : -COLLISION_TOLERANCE);
        }
        if (penetrationY != 0.0f) {
            eyeY += penetrationY + (penetrationY > 0 ? COLLISION_TOLERANCE : -COLLISION_TOLERANCE);
        }
        if (penetrationZ != 0.0f) {
            eyeZ += penetrationZ + (penetrationZ > 0 ? COLLISION_TOLERANCE : -COLLISION_TOLERANCE);
        }

        // Normalizar el vector normal para asegurar cálculos correctos
        float normalLength = sqrtf(normalX * normalX + normalY * normalY + normalZ * normalZ);
        if (normalLength > 0.0f) {
            normalX /= normalLength;
            normalY /= normalLength;
            normalZ /= normalLength;
        }

        // Manejo de colisiones verticales mejorado
        if (normalY > 0.7f && velocityY <= 0.0f) {
            // Colisión fuerte desde abajo - el jugador está definitivamente sobre algo
            velocityY = 0.0f;
            isGrounded = true;
        }
        else if (normalY > 0.3f && velocityY <= 0.0f) {
            // Colisión moderada desde abajo - probablemente sobre una superficie inclinada
            velocityY = 0.0f;
            isGrounded = true;
        }
        else if (normalY < -0.7f && velocityY > 0.0f) {
            // Colisión desde arriba - detener movimiento hacia arriba
            velocityY = 0.0f;
        }
        
        // Asegurar que el jugador no esté por debajo del suelo básico
        float groundOffset = bodyRadius + 0.37f;
        float minY = groundLevel + groundOffset;
        if (eyeY < minY) {
            eyeY = minY;
            if (velocityY < 0) {
                velocityY = 0.0f;
            }
            isGrounded = true;
        }
    }

    // MEJORADO: Método para verificar si el jugador debería estar en el suelo
    void checkGroundStatus(float minSupportY) {
        float groundOffset = bodyRadius + 0.37f;
        float groundY = groundLevel + groundOffset;
        
        // Tolerancia para detección de suelo más estricta
        const float GROUND_TOLERANCE = 0.05f;
        
        // Si estamos muy cerca del suelo o hay soporte desde abajo
        if (eyeY <= groundY + GROUND_TOLERANCE || eyeY <= minSupportY + groundOffset + GROUND_TOLERANCE) {
            isGrounded = true;
            if (velocityY < 0.0f) {
                velocityY = 0.0f;
            }
            // Ajustar posición para evitar hundimiento
            if (eyeY < minSupportY + groundOffset) {
                eyeY = minSupportY + groundOffset;
            }
        } else {
            isGrounded = false;
        }
    }

    void jump() {
        if (isGrounded) {
            velocityY = 8.0f;
            isGrounded = false;
        }
    }

    void rotate(float deltaX, float deltaY) {
        float sensitivity = 0.5f;

        yaw += deltaX * sensitivity;
        pitch -= deltaY * sensitivity;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateVectors();
    }

    void toggleView() {
        thirdPerson = !thirdPerson;
    }

    bool isThirdPerson() const { return thirdPerson; }
    bool getIsGrounded() const { return isGrounded; }
    float getX() const { return eyeX; }
    float getY() const { return eyeY; }
    float getZ() const { return eyeZ; }
    float getPitch() const { return pitch; }
    float getYaw() const { return yaw; }
    float getVelocityY() const { return velocityY; }

    // MEJORADO: Method to set position directly (useful for collision resolution)
    void setPosition(float x, float y, float z) {
        eyeX = x;
        eyeY = y;
        eyeZ = z;

        // Verificar que no estemos por debajo del suelo con tolerancia
        float groundOffset = bodyRadius + 0.37f;
        float minY = groundLevel + groundOffset;
        if (eyeY < minY) {
            eyeY = minY;
            if (velocityY < 0) {
                velocityY = 0.0f;
            }
            isGrounded = true;
        }
    }

    // Method to get the body radius
    float getBodyRadius() const {
        return bodyRadius;
    }

    // MEJORADO: Method to check if a position would be on the ground
    bool wouldBeGrounded(float y) const {
        float groundOffset = bodyRadius + 0.37f;
        const float GROUND_TOLERANCE = 0.05f;
        return y <= (groundLevel + groundOffset + GROUND_TOLERANCE);
    }

    // Method to get normalized direction vectors (useful for movement)
    void getDirectionVectors(float& frontX_out, float& frontZ_out,
                             float& rightX_out, float& rightZ_out) const {
        frontX_out = cos(pitch * M_PI / 180.0f) * cos(yaw * M_PI / 180.0f);
        frontZ_out = cos(pitch * M_PI / 180.0f) * sin(yaw * M_PI / 180.0f);
        rightX_out = cos((yaw - 90.0f) * M_PI / 180.0f);
        rightZ_out = sin((yaw - 90.0f) * M_PI / 180.0f);

        // Normalizar para movimiento horizontal
        float frontLengthFlat = sqrt(frontX_out * frontX_out + frontZ_out * frontZ_out);
        if (frontLengthFlat > 0.0f) {
            frontX_out /= frontLengthFlat;
            frontZ_out /= frontLengthFlat;
        }

        float rightLengthFlat = sqrt(rightX_out * rightX_out + rightZ_out * rightZ_out);
        if (rightLengthFlat > 0.0f) {
            rightX_out /= rightLengthFlat;
            rightZ_out /= rightLengthFlat;
        }
    }

    // NUEVO: Método para obtener la posición del fondo de la esfera (útil para colisiones)
    void getBottomPosition(float& bottomX, float& bottomY, float& bottomZ) const {
        bottomX = eyeX;
        bottomY = eyeY - bodyRadius;
        bottomZ = eyeZ;
    }

    // NUEVO: Método para validar y corregir la posición después de colisiones
    void validatePosition() {
        // Asegurar que nunca estemos por debajo del suelo básico
        float groundOffset = bodyRadius + 0.37f;
        float minY = groundLevel + groundOffset;
        if (eyeY < minY) {
            eyeY = minY;
            if (velocityY < 0) {
                velocityY = 0.0f;
            }
            isGrounded = true;
        }
    }
};

#endif // PLAYER_H
