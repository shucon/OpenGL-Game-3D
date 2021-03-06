#include "main.h"

#ifndef CUBE_H
#define CUBE_H


class Cube {
public:
    Cube() {}
    Cube(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float launch_speed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    float angular_speed;
    float speed_y;
    float level_angle;
    float acceleration_y;
    void tick();
    void shm();
    bounding_box_t bounding_box();
    double speed;
private:
    VAO *object;
    VAO *object1;
    VAO *object2;
    VAO *object3;
    VAO *object4;
    VAO *object5;
    VAO *object6;
    VAO *object7;
    VAO *object8;
    VAO *object9;
};

#endif // CUBE_H
