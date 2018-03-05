

#include "barrel.h"
#include "main.h"

Barrel::Barrel(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->size = 10;
    int pos=0,i,k,n=100;
    float j=0;

    GLfloat vertex_buffer_data[180*n+18];
    GLfloat g_vertex_buffer_data[180*n];
    float pi = 3.14159, angle = 0, theta=(2*pi)/n;
    for(j=0;j<2;j+=0.1){
        for(i=0;i<n;i++){
            g_vertex_buffer_data[pos++]= 10*j;
            g_vertex_buffer_data[pos++]= 0.0f;
            g_vertex_buffer_data[pos++]= 0.0f;
            for(k=0;k<2;k++){
                g_vertex_buffer_data[pos++]= 10*j;
                g_vertex_buffer_data[pos++]= this->size*cos(angle);
                g_vertex_buffer_data[pos++]= this->size*sin(angle);
                angle += theta;
            }
            angle-=theta;
            //j+=0.001;
     }
    }

    static const GLfloat l_vertex_buffer_data[] = {
        0.0,0.0,-1*this->size, 
       19.0,0.0,   this->size, 
       19.0,0.0,-1*this->size, 

        0.0,0.0,-1*this->size,
       19.0,0.0,   this->size,
        0.0,0.0,   this->size,
    };

    for(i=0;i<180*n+18;i++){
        if(i<180*n)
            vertex_buffer_data[i] = g_vertex_buffer_data[i];
        else
            vertex_buffer_data[i] = l_vertex_buffer_data[i-180*n];
    }

    this->object = create3DObject(GL_TRIANGLES, 60*n+6, vertex_buffer_data, color, GL_FILL);
}

void Barrel::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Barrel::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Barrel::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 9, 9};
    return bbox;
}
