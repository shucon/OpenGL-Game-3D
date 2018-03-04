#include "main.h"
#include "timer.h"
#include "cube.h"
#include "pool.h"
#include "rock.h"
#include "aim.h"
#include "sail.h"
#include "monster.h"
#include "barrel.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Cube boat;
Rock rocks[100];
Pool pool;
Rock cannon,blast;
Aim aim;
Sail sail;
Monster monster[100];
Rock gift;
Barrel barrel[100];
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0 , gravity = 0.5 ,level = 3,pi=3.141;
float camera_x=0,camera_y=90,camera_z=100,target_x=0,target_y=90,target_z=0,health = 3;
int rockCount = 100,monsterCount = 100,barrelCount=40,view = 1, time_cnt = 0, time_blast = 0,time_gift=0;
Timer t60(1.0 / 60);
int escape = 1;
void change_camera();
void speed_camera();
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 0, -5, 2 );
    glm::vec3 eye(camera_x, camera_y, camera_z);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (target_x, target_y, target_z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 0, 1);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    pool.draw(VP);
    for(int i = 0 ; i < rockCount ; i++){
        rocks[i].draw(VP);
    }  
    for(int i = 0 ; i < rockCount ; i++){
        monster[i].draw(VP);
    }    
    boat.draw(VP);
    cannon.draw(VP);
    blast.draw(VP);
    sail.draw(VP);
    aim.draw(VP);
    gift.draw(VP);
    for(int i = 0 ; i < rockCount ; i++){
        barrel[i].draw(VP);        
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int jump = glfwGetKey(window, GLFW_KEY_SPACE);
    int fire = glfwGetKey(window, GLFW_KEY_F);
    int camera = glfwGetKey(window, GLFW_KEY_C);

    if (left) {
        int flag = 0;
        for (int i = 0 ; i < rockCount ; i++) {
            if (detect_collision(boat.bounding_box(),rocks[i].bounding_box()) && rocks[i].position.z != -1000) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            boat.rotation += 5;
            cannon.rotation += 5;
            aim.rotation_z += 5;
        }
        // pool.position.x = boat.position.x;
    }
    if (right) {
        int flag = 0;
        for (int i = 0 ; i < rockCount ; i++) {
            if (detect_collision(boat.bounding_box(),rocks[i].bounding_box()) && rocks[i].position.z != -1000) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            boat.rotation -= 5;
            cannon.rotation -= 5;
            aim.rotation_z -= 5;
        }
        // pool.position.x = boat.position.x;        
    }
    if (up) {
        boat.position.y += boat.speed*cos(boat.rotation*M_PI/180.0f);
        boat.position.x -= boat.speed*sin(boat.rotation*M_PI/180.0f);
        int flag = 0;
        for (int i = 0 ; i < rockCount ; i++) {
            if (detect_collision(boat.bounding_box(),rocks[i].bounding_box()) && rocks[i].position.z != -1000) {
                if (escape == 1) {
                    health -= 0.5;
                    escape = 0;
                }
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            pool.position.y = boat.position.y;
            if (cannon.position.z == boat.position.z) {
                cannon.position.x = boat.position.x;        
                cannon.position.y = boat.position.y;        
            }
            aim.position.y = boat.position.y;        
            aim.position.x = boat.position.x;
            sail.position.y = boat.position.y;        
            sail.position.x = boat.position.x;
            escape = 1;
        }
        else {
            boat.position.y -= boat.speed*cos(boat.rotation*M_PI/180.0f);
            boat.position.x += boat.speed*sin(boat.rotation*M_PI/180.0f);
            escape = 0;
        }

    }
    if (down) {
        boat.position.y -= boat.speed*cos(boat.rotation*M_PI/180.0f);
        boat.position.x += boat.speed*sin(boat.rotation*M_PI/180.0f);
        int flag = 0;
        for (int i = 0 ; i < rockCount ; i++) {
            if (detect_collision(boat.bounding_box(),rocks[i].bounding_box()) && rocks[i].position.z != -1000) {
                if (escape == 1) {
                    health -= 0.5;
                    escape = 0;
                }
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            pool.position.y = boat.position.y;
            if (cannon.position.z == boat.position.z) {
                cannon.position.x = boat.position.x;        
                cannon.position.y = boat.position.y;        
            }
            aim.position.y = boat.position.y;        
            aim.position.x = boat.position.x;
            sail.position.y = boat.position.y;        
            sail.position.x = boat.position.x;
            escape = 1;
        }
        else {
            boat.position.y += boat.speed*cos(boat.rotation*M_PI/180.0f);
            boat.position.x -= boat.speed*sin(boat.rotation*M_PI/180.0f);
            escape = 0;
        }       
}
    if (jump && boat.position.z == level) {
        boat.launch_speed = 6;
        cannon.launch_speed =  6;
        aim.launch_speed = 6;
    }
    if (fire && cannon.position.z == boat.position.z && aim.rotation_h > 3 && time_blast == 0) {
        cannon.launch_speed = 10 * sin(aim.rotation_h*M_PI/180);
        cannon.launch_speed_x = -10 * cos(aim.rotation_h*M_PI/180) * sin(aim.rotation_z*M_PI/180);
        cannon.launch_speed_y = 10 * cos(aim.rotation_h*M_PI/180) * cos(aim.rotation_z*M_PI/180);
    }
    if (camera && time_cnt > 30) {
        change_camera();
        time_cnt = 0;
    }
}

void tick_elements() {
    boat.tick();
    // printf("%lf  %lf  %lf",boat.position.x,boat.position.y,boat.position.z);
    // camera_rotation_angle += 1;
    boat.position.z += boat.launch_speed;
    cannon.position.z += cannon.launch_speed;
    cannon.position.x += cannon.launch_speed_x;
    cannon.position.y += cannon.launch_speed_y;
    aim.position.z += aim.launch_speed;
    if (boat.position.z > level || cannon.position.z > level) {
        boat.launch_speed -= gravity;
        cannon.launch_speed -= gravity;
        aim.launch_speed -= gravity;
    }

    if (boat.position.z <= level) {
        boat.launch_speed = 0;
        boat.position.z = level;
        aim.launch_speed = 0;
        aim.position.z = level;
}

    if (cannon.position.z <= level) {
        cannon.launch_speed = 0;
        cannon.launch_speed_x = 0;
        cannon.launch_speed_y = 0;
        if (cannon.position.x != boat.position.x && cannon.position.y != boat.position.y) {
            blast.position.y = cannon.position.y;
            blast.position.x = cannon.position.x;
            time_blast = 1;
        }
        cannon.position.z = level;
        cannon.position.x = boat.position.x;
        cannon.position.y = boat.position.y;
    }
    speed_camera();
    time_cnt ++;
    sail.position.z = boat.position.z;
    if (time_blast > 60) {
        time_blast = 0;
        blast.size = 25;
        blast.position.x = -1000;
        blast.position.y = -1000;
    }
    for (int i = 0 ; i < rockCount ; i++) {
        if(detect_collision(blast.bounding_box(),rocks[i].bounding_box())) {
            rocks[i].position.z = -1000;
        }
    }
    for (int i = 0 ; i < monsterCount ; i++) {
        if(detect_collision(blast.bounding_box(),monster[i].bounding_box())) {
            monster[i].position.z = -1000;
            if (i%10 == 0) {
                //mega-monster
                gift.position.x = monster[i].position.x;
                gift.position.y = monster[i].position.y;
            }
        }
    }
    if(detect_collision(boat.bounding_box(),gift.bounding_box())) {
        gift.position.x = -1000;
        gift.position.y = -1000;
        time_gift++;
        boat.speed *= 5;
    }
    if (time_gift) {
        time_gift++;
    }
    if (time_gift >= 600) {
        time_gift = 0;
        boat.speed /= 5;
    }
    if (time_blast > 0)  {
        time_blast ++;
        blast.size -= 0.35;
    }
    // printf("Health: %f\n",health);
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat = Cube(0, 0, COLOR_RED);
    boat.position.z += level;
    pool = Pool(0,0, COLOR_BLUE);
    pool.position.z -= 32767;
    for (int i = 0 ; i < rockCount ; i++) {
        float x = ((((i+1)*rand()+i*584)%10000)/10) + 30;  
        float y = ((((i+1)*rand()+i*784)%10000)/10) + 30;
        rocks[i] = Rock(x , y, COLOR_BLACK);
        rocks[i].position.z += 1;
        rocks[i].size = i%5 * 5;
    }
    
    for (int i = 0 ; i < monsterCount ; i++) {
        float x = ((((i+1)*rand()+i*584)%10000)/10) + 50;  
        float y = ((((i+1)*rand()+i*784)%10000)/10) + 50;
        if (i%3 == 0)
            monster[i] = Monster(x , y, COLOR_CUBE1);
        if (i%3 == 1)
            monster[i] = Monster(x , y, COLOR_CUBE3);
        if (i%3 == 2)
            monster[i] = Monster(x , y, COLOR_CUBE4);
        monster[i].position.z += 1;
        monster[i].size = 10;
        if (i%10 == 0) {
            monster[i] = Monster(x , y, COLOR_CUBE8);
            monster[i].size = 30;
        }
    }
    aim = Aim(0, 0, COLOR_RED);
    aim.position.z += level;
    cannon = Rock(boat.position.x,boat.position.y,COLOR_RED);
    blast = Rock(-1000,-1000,COLOR_BLAST);
    gift = Rock(-10000,-10000,COLOR_CUBE1);
    blast.size = 25;
    gift.size = 10;
    cannon.position.z += level;
    gift.position.z += level;
    blast.position.z -= blast.size/2;
    sail = Sail(boat.position.x,boat.position.y,COLOR_BACKGROUND);
    sail.position.z += level;
    for (int i = 0 ; i < monsterCount ; i++) {
        float x = ((((i+1)*rand()+i*584)%10000)/10) + 50;  
        float y = ((((i+1)*rand()+i*784)%10000)/10) + 50;
        barrel[i] = Barrel(x ,y , COLOR_BROWN);
        barrel[i].position.z += level;
    }
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2)) < (a.width+b.width));
}

void reset_screen() {
    float top    = screen_center_y + 15 / screen_zoom;
    float bottom = screen_center_y - 15 / screen_zoom;
    float left   = screen_center_x - 15 / screen_zoom;
    float right  = screen_center_x + 15 / screen_zoom;
    Matrices.projection = glm::perspective(45.0f,1.0f,50.0f, 500.0f);
}
void change_camera(){
    view = (view+1)%5;
}

void speed_camera(){
    float theta = (boat.rotation)*(pi/180);
    if(view == 1){ //follow-cam
        camera_x = boat.position.x+100*sin(theta);
        camera_y = boat.position.y-100*cos(theta);
        camera_z = 50;

        target_x = boat.position.x;
        target_y = boat.position.y;
        target_z = boat.position.z;
    }
    else if(view == 0) { //helicopter-view
        camera_z = 75;
    }
    else if(view==3){ // boat view
        camera_x = boat.position.x;
        camera_y = boat.position.y;
        camera_z = 25;

        target_x = boat.position.x-40*sin(theta);
        target_y = boat.position.y+40*cos(theta);
        target_z = boat.position.z+25;
    }
    else if(view==4){ //tower-view
        camera_x = boat.position.x+100;
        camera_y = boat.position.y;
        camera_z = 50;

        target_x = boat.position.x;
        target_y = boat.position.y;
        target_z = boat.position.z;
    }
    else if(view==2){ //top-view
        camera_x = boat.position.x+1;
        camera_y = boat.position.y;
        camera_z = boat.position.z+300;

        target_x = boat.position.x;
        target_y = boat.position.y;
        target_z = boat.position.z;
    }
}

void heli_camera(float x, float y){
    if(view == 0) {
        target_y = boat.position.x+x/3-100;
        target_x = boat.position.y+y/3-100;
    }
}

void cannon_pos(float x, float y) {
    if (view != 0) {
        aim.rotation_z = boat.rotation-((3*x/5) - 180);
        aim.rotation_h = -(3*y/20) + 90;
    }
}
// void zoom_camera(int type){
//     if(view==2){
//         float l = target_x-camera_x;
//         float m = target_y-camera_y;
//         float n = target_z-camera_z;
//         if(type==1){
//             if(camera_z-10>target_z)
//                 camera_z-=10;
//         }
//         else if(type==-1){
//             camera_z+=10;
//         }
//         camera_x = l*(camera_z-target_z)/n+target_x;
//         camera_y = m*(camera_z-target_z)/n+target_y;
//     }
// }