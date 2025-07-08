#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include <cmath>
#include <algorithm> // Para std::min, std::max

// Estructura para representar un cubo colisionable
struct CollisionBox {
    float x, y, z;       // Posición del centro del cubo
    float width, height, depth; // Dimensiones del cubo
    
    CollisionBox(float x = 0.0f, float y = 0.0f, float z = 0.0f,
                 float w = 1.0f, float h = 1.0f, float d = 1.0f)
        : x(x), y(y), z(z), width(w), height(h), depth(d) {}
    
    // Obtener límites del cubo
    float getMinX() const { return x - width * 0.5f; }
    float getMaxX() const { return x + width * 0.5f; }
    float getMinY() const { return y - height * 0.5f; }
    float getMaxY() const { return y + height * 0.5f; }
    float getMinZ() const { return z - depth * 0.5f; }
    float getMaxZ() const { return z + depth * 0.5f; }
};

// Estructura para representar una esfera colisionable (jugador)
struct CollisionSphere {
    float x, y, z;       // Posición del centro
    float radius;        // Radio de la esfera
    
    CollisionSphere(float x = 0.0f, float y = 0.0f, float z = 0.0f, float r = 1.0f)
        : x(x), y(y), z(z), radius(r) {}
};

// Estructura para resultado de colisión
struct CollisionResult {
    bool hasCollision;
    float penetrationX, penetrationY, penetrationZ;
    float normalX, normalY, normalZ; // Normal de la superficie de colisión
    
    CollisionResult() : hasCollision(false), 
                        penetrationX(0.0f), penetrationY(0.0f), penetrationZ(0.0f),
                        normalX(0.0f), normalY(0.0f), normalZ(0.0f) {}
};

class CollisionSystem {
private:
    std::vector<CollisionBox> boxes;
    float groundLevel;
    
    // Funciones de utilidad para cálculos
    float clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
    
    float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        float dz = z2 - z1;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }
    
    // Detectar colisión entre esfera y cubo usando el método del punto más cercano
    CollisionResult checkSphereBoxCollision(const CollisionSphere& sphere, const CollisionBox& box) {
        CollisionResult result;
        
        // Encontrar el punto más cercano del cubo a la esfera
        float closestX = clamp(sphere.x, box.getMinX(), box.getMaxX());
        float closestY = clamp(sphere.y, box.getMinY(), box.getMaxY());
        float closestZ = clamp(sphere.z, box.getMinZ(), box.getMaxZ());
        
        // Calcular distancia desde el centro de la esfera al punto más cercano
        float dist = distance(sphere.x, sphere.y, sphere.z, closestX, closestY, closestZ);
        
        if (dist < sphere.radius) {
            result.hasCollision = true;
            
            // Calcular vector de penetración
            float dx = sphere.x - closestX;
            float dy = sphere.y - closestY;
            float dz = sphere.z - closestZ;
            
            // Si la esfera está exactamente en el centro del cubo, usar Y como normal por defecto
            if (dist == 0.0f) {
                result.normalX = 0.0f;
                result.normalY = 1.0f;
                result.normalZ = 0.0f;
                result.penetrationX = 0.0f;
                result.penetrationY = sphere.radius;
                result.penetrationZ = 0.0f;
            } else {
                // Normalizar el vector de penetración
                float invDist = 1.0f / dist;
                result.normalX = dx * invDist;
                result.normalY = dy * invDist;
                result.normalZ = dz * invDist;
                
                // Calcular penetración
                float penetrationDepth = sphere.radius - dist;
                result.penetrationX = result.normalX * penetrationDepth;
                result.penetrationY = result.normalY * penetrationDepth;
                result.penetrationZ = result.normalZ * penetrationDepth;
            }
        }
        
        return result;
    }
    
public:
    CollisionSystem() : groundLevel(-0.1f) {}
    
    ~CollisionSystem() {
        boxes.clear();
    }
    
    // Añadir un cubo colisionable
    void addBox(float x, float y, float z, float width, float height, float depth) {
        boxes.push_back(CollisionBox(x, y, z, width, height, depth));
    }
    
    // Añadir un cubo colisionable con tamaño uniforme (adjusts Y for base position)
    void addCube(float x, float y, float z, float size) {
        // The y parameter here is assumed to be the base of the cube,
        // so we adjust it to be the center for CollisionBox.
        float adjustedY = y + size * 0.5f;
        boxes.push_back(CollisionBox(x, adjustedY, z, size, size, size));
    }
    
    // Limpiar todas las cajas de colisión
    void clearBoxes() {
        boxes.clear();
    }
    
    // Establecer nivel del suelo
    void setGroundLevel(float level) {
        groundLevel = level;
    }
    
    // Verificar colisión con el suelo
    bool checkGroundCollision(float x, float y, float z, float radius) {
        return (y - radius) <= groundLevel;
    }
    
    // Verificar colisión principal y resolver penetración
    CollisionResult checkCollision(float x, float y, float z, float radius) {
        CollisionSphere sphere(x, y, z, radius);
        CollisionResult finalResult;
        
        // Variables para acumular penetración
        float totalPenetrationX = 0.0f;
        float totalPenetrationY = 0.0f;
        float totalPenetrationZ = 0.0f;
        
        // Verificar colisión con cada caja
        for (size_t i = 0; i < boxes.size(); ++i) {
            CollisionResult result = checkSphereBoxCollision(sphere, boxes[i]);
            
            if (result.hasCollision) {
                finalResult.hasCollision = true;
                
                // Acumular penetración (tomar la máxima en cada eje)
                // Note: This accumulation might not be perfect for complex multi-object collisions
                // but provides a reasonable approximation for simple scenarios.
                if (fabs(result.penetrationX) > fabs(totalPenetrationX)) {
                    totalPenetrationX = result.penetrationX;
                    finalResult.normalX = result.normalX; // Store the normal of the dominant penetration
                }
                if (fabs(result.penetrationY) > fabs(totalPenetrationY)) {
                    totalPenetrationY = result.penetrationY;
                    finalResult.normalY = result.normalY;
                }
                if (fabs(result.penetrationZ) > fabs(totalPenetrationZ)) {
                    totalPenetrationZ = result.penetrationZ;
                    finalResult.normalZ = result.normalZ;
                }
            }
        }
        
        finalResult.penetrationX = totalPenetrationX;
        finalResult.penetrationY = totalPenetrationY;
        finalResult.penetrationZ = totalPenetrationZ;
        
        return finalResult;
    }
    
    // Verificar si una posición está libre de colisiones
    bool isPositionFree(float x, float y, float z, float radius) {
        CollisionSphere sphere(x, y, z, radius);
        
        for (size_t i = 0; i < boxes.size(); ++i) {
            CollisionResult result = checkSphereBoxCollision(sphere, boxes[i]);
            if (result.hasCollision) {
                return false;
            }
        }
        
        return true;
    }
    
    // Resolver colisión ajustando posición
    void resolveCollision(float& x, float& y, float& z, float radius) {
        const int maxIterations = 5; // Evitar bucles infinitos para resolver múltiples colisiones
        
        for (int iter = 0; iter < maxIterations; ++iter) {
            CollisionResult result = checkCollision(x, y, z, radius);
            
            if (!result.hasCollision) {
                break; // No hay más colisiones
            }
            
            // Ajustar posición según penetración
            x += result.penetrationX;
            y += result.penetrationY;
            z += result.penetrationZ;
        }
    }
    
    // Verificar colisión en una dirección específica (útil para movimiento)
    bool wouldCollideAt(float x, float y, float z, float radius) {
        CollisionSphere sphere(x, y, z, radius);
        
        for (size_t i = 0; i < boxes.size(); ++i) {
            CollisionResult result = checkSphereBoxCollision(sphere, boxes[i]);
            if (result.hasCollision) {
                return true;
            }
        }
        
        return false;
    }
    
    // Obtener el número de cajas de colisión
    size_t getBoxCount() const {
        return boxes.size();
    }
    
    // Obtener una caja específica (para debug)
    const CollisionBox& getBox(size_t index) const {
        return boxes[index];
    }
    
    // Método para añadir colisiones específicas de la escalera
    void addEscaleraCollisions(float posX, float posY, float posZ, float scale = 1.0f) {
        // Datos exactos de Escalera.h
        const int numSteps = 8;
        const float stepWidth = 4.0f;
        const float stepHeight = 0.5f;
        const float stepDepth = 1.0f;
        const float railingHeight = 2.0f;
        
        // Añadir colisiones para cada escalón
        for (int i = 0; i < numSteps; i++) {
            float x = posX;
            float y = posY + (i * stepHeight * scale) + (stepHeight * scale); // Centro del escalón
            float z = posZ - (i * stepDepth * scale);
            
            // Cada escalón como caja de colisión
            addBox(x, y, z, 
                   stepWidth * scale, 
                   stepHeight * 2.0f * scale, // Altura duplicada como en el código original
                   stepDepth * scale);
        }
        
        // Añadir colisiones para los postes del barandal izquierdo
        for (int i = 0; i < numSteps; i++) {
            float x = posX - 1.8f * scale;
            float y = posY + (i * stepHeight * scale) + (railingHeight * scale * 0.5f);
            float z = posZ - (i * stepDepth * scale);
            
            addBox(x, y, z, 
                   0.1f * scale, 
                   railingHeight * scale, 
                   0.1f * scale);
        }
        
        // Añadir colisiones para el barandal horizontal izquierdo
        for (int i = 0; i < numSteps - 1; i++) {
            float x = posX - 1.8f * scale;
            float y = posY + (i * stepHeight * scale) + (railingHeight * scale);
            float z = posZ - (i * stepDepth * scale) - (stepDepth * scale * 0.5f);
            
            addBox(x, y, z, 
                   0.08f * scale, 
                   0.08f * scale, 
                   stepDepth * scale);
        }
        
        // Añadir colisiones para los postes del barandal derecho
        for (int i = 0; i < numSteps; i++) {
            float x = posX + 1.8f * scale;
            float y = posY + (i * stepHeight * scale) + (railingHeight * scale * 0.5f);
            float z = posZ - (i * stepDepth * scale);
            
            addBox(x, y, z, 
                   0.1f * scale, 
                   railingHeight * scale, 
                   0.1f * scale);
        }
        
        // Añadir colisiones para el barandal horizontal derecho
        for (int i = 0; i < numSteps - 1; i++) {
            float x = posX + 1.8f * scale;
            float y = posY + (i * stepHeight * scale) + (railingHeight * scale);
            float z = posZ - (i * stepDepth * scale) - (stepDepth * scale * 0.5f);
            
            addBox(x, y, z, 
                   0.08f * scale, 
                   0.08f * scale, 
                   stepDepth * scale);
        }
    }
    
    // Método para sincronizar con el mundo
    void setupWorldCollisions() {
        clearBoxes();
        
        // --- Casa Jaimito ---
        // World offset for the house from World::render()
        // glTranslatef(0.0f, 0.0f, -20.0f);
        float jaimitoOffsetX = 8.0f;
        float jaimitoOffsetY = 3.0f;
        float jaimitoOffsetZ = -3.3f;

        // Add collision boxes for the house components based on World.h drawHouse()
        // Front wall
        addBox(0.0f + jaimitoOffsetX, 1.5f + jaimitoOffsetY, 1.5f + jaimitoOffsetZ, 4.0f, 3.0f, 0.1f);
        // Left wall (rotated 90 degrees around Y, so local width becomes world depth and local depth becomes world width)
        addBox(-2.0f + jaimitoOffsetX, 1.5f + jaimitoOffsetY, 0.0f + jaimitoOffsetZ, 0.1f, 3.0f, 3.0f);
        // Right wall (rotated -90 degrees around Y)
        addBox(2.0f + jaimitoOffsetX, 1.5f + jaimitoOffsetY, 0.0f + jaimitoOffsetZ, 0.1f, 3.0f, 3.0f);
        // Back wall
        addBox(0.0f + jaimitoOffsetX, 1.5f + jaimitoOffsetY, -1.5f + jaimitoOffsetZ, 4.0f, 3.0f, 0.1f);

        // Main door (frame and panel, treating as solid for collision)
        // Door frame
        addBox(0.0f + jaimitoOffsetX, 1.0f + jaimitoOffsetY, (1.55f + 0.02f) + jaimitoOffsetZ, 1.0f, 2.2f, 0.05f);
        // Door panel
        addBox(0.0f + jaimitoOffsetX, 1.0f + jaimitoOffsetY, (1.55f + 0.04f) + jaimitoOffsetZ, 0.8f, 2.0f, 0.04f);

        // Window on left wall (frame and glass, treating as solid for collision)
        // The window's local Z offset (0.02f or 0.04f) becomes a negative X offset after 90 deg Y rotation
        // Left window frame
        addBox((-2.05f - 0.02f) + jaimitoOffsetX, 1.8f + jaimitoOffsetY, 0.5f + jaimitoOffsetZ, 0.05f, 1.1f, 1.1f);
        // Left window glass
        addBox((-2.05f - 0.04f) + jaimitoOffsetX, 1.8f + jaimitoOffsetY, 0.5f + jaimitoOffsetZ, 0.02f, 1.0f, 1.0f);

        // Window on right wall (frame and glass, treating as solid for collision)
        // The window's local Z offset (0.02f or 0.04f) becomes a positive X offset after -90 deg Y rotation
        // Right window frame
        addBox((2.05f + 0.02f) + jaimitoOffsetX, 1.8f + jaimitoOffsetY, 0.5f + jaimitoOffsetZ, 0.05f, 1.1f, 1.1f);
        // Right window glass
        addBox((2.05f + 0.04f) + jaimitoOffsetX, 1.8f + jaimitoOffsetY, 0.5f + jaimitoOffsetZ, 0.02f, 1.0f, 1.0f);

        // Floor/base
        addBox(0.0f + jaimitoOffsetX, -0.1f + jaimitoOffsetY, 0.0f + jaimitoOffsetZ, 4.2f, 0.2f, 3.2f);
        
        // Flat roof
        // The drawRoof function adjusts the Y center to 3.0f + (0.2f - 0.2f / 2.0f) = 3.1f
        addBox(0.0f + jaimitoOffsetX, 3.1f + jaimitoOffsetY, 0.0f + jaimitoOffsetZ, 4.2f, 0.2f, 3.2f);

        // Small decorative details (pot and plant)
        // Pot (glutSolidCube is centered, so use addBox directly)
        addBox(-1.8f + jaimitoOffsetX, 0.3f + jaimitoOffsetY, 1.6f + jaimitoOffsetZ, 0.3f, 0.3f, 0.3f);
        // Plant (glutSolidSphere, approximate with cube, use addBox directly)
        addBox(-1.8f + jaimitoOffsetX, 0.6f + jaimitoOffsetY, 1.6f + jaimitoOffsetZ, 0.3f, 0.3f, 0.3f);

        // --- Casa Clotilde ---
        // glTranslatef(20.0f, 0.0f, -20.0f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        float clotildeOffsetX = 20.0f;
        float clotildeOffsetY = 0.0f;
        float clotildeOffsetZ = -20.0f;
        
        // For CasaClotilde, since it's rotated -90 degrees around Y,
        // its local X becomes world Z, and local Z becomes world -X.
        // I'll assume CasaClotilde has similar dimensions to CasaJaimito for simplicity.
        // You'll need to adapt these based on CasaClotilde's actual drawing code.
        addBox(clotildeOffsetX, 1.5f + clotildeOffsetY, clotildeOffsetZ + 0.0f, 3.0f, 3.0f, 0.1f); // Back wall (local front)
        addBox(clotildeOffsetX + 1.5f, 1.5f + clotildeOffsetY, clotildeOffsetZ + 0.0f, 0.1f, 3.0f, 3.0f); // Right wall (local left)
        addBox(clotildeOffsetX - 1.5f, 1.5f + clotildeOffsetY, clotildeOffsetZ + 0.0f, 0.1f, 3.0f, 3.0f); // Left wall (local right)
        addBox(clotildeOffsetX, 1.5f + clotildeOffsetY, clotildeOffsetZ + 1.5f, 3.0f, 3.0f, 0.1f); // Front wall (local back)

        addBox(clotildeOffsetX, -0.1f + clotildeOffsetY, clotildeOffsetZ, 3.2f, 0.2f, 4.2f); // Floor
        addBox(clotildeOffsetX, 3.1f + clotildeOffsetY, clotildeOffsetZ, 3.2f, 0.2f, 4.2f); // Roof

        // --- Casa Florinda ---
        // glTranslatef(10.0f, 1.69f, -20.0f); glScalef(0.6f, 0.6f, 0.6f);
        float florindaOffsetX = 10.0f;
        float florindaOffsetY = 1.69f;
        float florindaOffsetZ = -20.0f;
        float florindaScale = 0.6f;

        // Assuming FlorindaCasa is similar to CasaJaimito but scaled
        addBox(0.0f * florindaScale + florindaOffsetX, (1.5f * florindaScale) + florindaOffsetY, (1.5f * florindaScale) + florindaOffsetZ, 4.0f * florindaScale, 3.0f * florindaScale, 0.1f * florindaScale);
        addBox((-2.0f * florindaScale) + florindaOffsetX, (1.5f * florindaScale) + florindaOffsetY, (0.0f * florindaScale) + florindaOffsetZ, 0.1f * florindaScale, 3.0f * florindaScale, 3.0f * florindaScale);
        addBox((2.0f * florindaScale) + florindaOffsetX, (1.5f * florindaScale) + florindaOffsetY, (0.0f * florindaScale) + florindaOffsetZ, 0.1f * florindaScale, 3.0f * florindaScale, 3.0f * florindaScale);
        addBox((0.0f * florindaScale) + florindaOffsetX, (1.5f * florindaScale) + florindaOffsetY, (-1.5f * florindaScale) + florindaOffsetZ, 4.0f * florindaScale, 3.0f * florindaScale, 0.1f * florindaScale);
        addBox((0.0f * florindaScale) + florindaOffsetX, (-0.1f * florindaScale) + florindaOffsetY, (0.0f * florindaScale) + florindaOffsetZ, 4.2f * florindaScale, 0.2f * florindaScale, 3.2f * florindaScale);
        addBox((0.0f * florindaScale) + florindaOffsetX, (3.1f * florindaScale) + florindaOffsetY, (0.0f * florindaScale) + florindaOffsetZ, 4.2f * florindaScale, 0.2f * florindaScale, 3.2f * florindaScale);

        // --- Casa Ramon ---
        // glTranslatef(20.0f, 0.0f, 0.0f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); glScalef(0.6f, 0.6f, 0.6f);
        float ramonOffsetX = 20.0f;
        float ramonOffsetY = 0.0f;
        float ramonOffsetZ = 0.0f;
        float ramonScale = 0.6f;

        // Assuming CasaRamon is similar to CasaJaimito, rotated and scaled
        addBox(ramonOffsetX, (1.5f * ramonScale) + ramonOffsetY, ramonOffsetZ + (0.0f * ramonScale), 3.0f * ramonScale, 3.0f * ramonScale, 0.1f * ramonScale); // Back wall (local front)
        addBox(ramonOffsetX + (1.5f * ramonScale), (1.5f * ramonScale) + ramonOffsetY, ramonOffsetZ + (0.0f * ramonScale), 0.1f * ramonScale, 3.0f * ramonScale, 3.0f * ramonScale); // Right wall (local left)
        addBox(ramonOffsetX - (1.5f * ramonScale), (1.5f * ramonScale) + ramonOffsetY, ramonOffsetZ + (0.0f * ramonScale), 0.1f * ramonScale, 3.0f * ramonScale, 3.0f * ramonScale); // Left wall (local right)
        addBox(ramonOffsetX, (1.5f * ramonScale) + ramonOffsetY, ramonOffsetZ + (1.5f * ramonScale), 3.0f * ramonScale, 3.0f * ramonScale, 0.1f * ramonScale); // Front wall (local back)

        addBox(ramonOffsetX, (-0.1f * ramonScale) + ramonOffsetY, ramonOffsetZ, 3.2f * ramonScale, 0.2f * ramonScale, 4.2f * ramonScale); // Floor
        addBox(ramonOffsetX, (3.1f * ramonScale) + ramonOffsetY, ramonOffsetZ, 3.2f * ramonScale, 0.2f * ramonScale, 4.2f * ramonScale); // Roof

        // --- Barril ---
        // glTranslatef(5.0f, 0.5f, 0.0f); glScalef(0.54f, 0.54f, 0.54f);
        float barrilOffsetX = 10.0f;
        float barrilOffsetY = 0.5f; // This is the center Y of the model, assuming the base is at 0.0f
        float barrilOffsetZ = 2.0f;
        float barrilScale = 0.64f;
        
        // Approximate the barrel with a cylinder or a box. A box is simpler.
        // Assuming a barrel is roughly a cylinder with height and diameter.
        // Let's assume its unscaled dimensions are diameter ~2.0f, height ~2.0f.
        addBox(barrilOffsetX, barrilOffsetY, barrilOffsetZ, 2.0f * barrilScale, 2.0f * barrilScale, 2.0f * barrilScale);

        // --- Pelota ---
        // glTranslatef(6.0f, 0.5f, 0.0f); glScalef(0.34f, 0.34f, 0.34f);
        float pelotaOffsetX = 14.0f;
        float pelotaOffsetY = 0.3f; // Center Y of the sphere
        float pelotaOffsetZ = 0.0f;
        float pelotaScale = 0.34f;
        
        // A sphere is best approximated by its radius for collision, but we use a box for simplicity here.
        // Assuming unscaled radius of 1.0f (so diameter 2.0f).
        addBox(pelotaOffsetX, pelotaOffsetY, pelotaOffsetZ, 2.0f * pelotaScale, 2.0f * pelotaScale, 2.0f * pelotaScale);

        // --- Escalera (Stairs) ---
        // glTranslatef(8.0f, 0.0f, 4.0f); glScalef(0.74f, 0.74f, 0.74f);
        float escaleraOffsetX = 8.0f;
        float escaleraOffsetY = 0.0f;
        float escaleraOffsetZ = 4.0f;
        float escaleraScale = 0.74f;

        // USAR DATOS REALES DE ESCALERA.H
        addEscaleraCollisions(escaleraOffsetX, escaleraOffsetY, escaleraOffsetZ, escaleraScale);

        // --- Pileta ---
        // glTranslatef(34.0f, 0.3f, -2.0f); glScalef(1.34f, 1.34f, 1.34f);
        float piletaOffsetX = 40.0f;
        float piletaOffsetY = 0.3f; // Center Y of the pileta
        float piletaOffsetZ = 0.0f;
        float piletaScale = 1.34f;
        

        // Assuming the pileta is a simple, box-like structure for collision purposes.
        // You'll need to define its unscaled dimensions based on your Pileta.h.
        // Let's assume it's roughly 3 units wide, 1 unit high, 3 units deep (unscaled).
        addBox(piletaOffsetX, piletaOffsetY, piletaOffsetZ,
               3.0f * piletaScale, 1.0f * piletaScale, 3.0f * piletaScale);



    }
};

#endif // COLLISION_H
