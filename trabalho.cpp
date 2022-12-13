/**
 * @file phong.cpp
 * Applies the Phong method.
 * 
 * @author Ricardo Dutra da Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "./lib/utils.h"
#include <unistd.h>

/* Globals */
/** Window width. */
int win_width  = 600;
/** Window height. */
int win_height = 600;

/** Program variable. */
int program;
/** Vertex array object. */
unsigned int VAO;
/** Vertex buffer object. */
unsigned int VBO;

/* variaveis para as cores dos cubos */
float r[7], g[7], b[7];
int i = 0;
int animation = 0;

/** Vertex shader. */
const char *vertex_code = "\n"
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 normal;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"out vec3 vNormal;\n"
"out vec3 fragPosition;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(position, 1.0);\n"
"    vNormal = mat3(transpose(inverse(model)))*normal;;\n"
"    fragPosition = vec3(model * vec4(position, 1.0));\n"
"}\0";

/** Fragment shader. */
const char *fragment_code = "\n"
"#version 330 core\n"
"\n"
"in vec3 vNormal;\n"
"in vec3 fragPosition;\n"
"\n"
"out vec4 fragColor;\n"
"\n"
"uniform vec3 objectColor;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 lightPosition;\n"
"uniform vec3 cameraPosition;\n"
"\n"
"void main()\n"
"{\n"
"    float ka = 0.5;\n"
"    vec3 ambient = ka * lightColor;\n"
"\n"
"    float kd = 0.8;\n"
"    vec3 n = normalize(vNormal);\n"
"    vec3 l = normalize(lightPosition - fragPosition);\n"
"\n"
"    float diff = max(dot(n,l), 0.0);\n"
"    vec3 diffuse = kd * diff * lightColor;\n"
"\n"
"    float ks = 1.0;\n"
"    vec3 v = normalize(cameraPosition - fragPosition);\n"
"    vec3 r = reflect(-l, n);\n"
"\n"
"    float spec = pow(max(dot(v, r), 0.0), 3.0);\n"
"    vec3 specular = ks * spec * lightColor;\n"
"\n"
"    vec3 light = (ambient + diffuse + specular) * objectColor;\n"
"    fragColor = vec4(light, 1.0);\n"
"}\0";

/* Functions. */
void display(void);
void reshape(int, int);
void keyboard(unsigned char, int, int);
void initData(void);
void initShaders(void);

/** 
 * Drawing function.
 *
 * Draws primitive.
 */
void display()
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(VAO);

	glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(1.0f,0.0f,0.0f));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(-30.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 model = Rx*Ry;
	unsigned int loc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (win_width/(float)win_height), 0.1f, 100.0f);
 	loc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

    // Light color.
    unsigned int locLight = glGetUniformLocation(program, "lightColor");
    glUniform3f(locLight, 1.0, 1.0, 1.0);

    //Light position.
    loc = glGetUniformLocation(program, "lightPosition");
    glUniform3f(loc, 1.0, 3.0, 2.0);
    	
    // Camera position.
    loc = glGetUniformLocation(program, "cameraPosition");
    glUniform3f(loc, 0.0, 0.0, 0.0);

    //cor original
    //glUniform3f(locColor, 0.1, 0.1, 0.5);

    if (animation == 0)
    {
        for (int j = 0; j < 7; j++)
        {
            r[j] = 0.0f;
            g[j] = 0.0f;
            b[j] = 1.0f;
        }
    }
    else
    {
        for (int j = 0; j <= i; j++)
        {
            r[j] = 0.0f;
            g[j] = 1.0f;
            b[j] = 0.0f;
        }
    }
    
    // Object color.
    unsigned int locColor = glGetUniformLocation(program, "objectColor");

    unsigned int locView = glGetUniformLocation(program, "view");

    //primeiro cubo que irá começar a BFS
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[0], g[0], b[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //cubos que são vizinhos do primeiro cubo
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[1], g[1], b[1]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[2], g[2], b[2]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[3], g[3], b[3]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //cubos que são vizinhos dos vizinhos do primeiro cubo
    view = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 1.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[4], g[4], b[4]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[5], g[5], b[5]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -1.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[6], g[6], b[6]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glutSwapBuffers();
}   

/**
 * Reshape function.
 *
 * Called when window is resized.
 *
 * @param width New window width.
 * @param height New window height.
 */
void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}


/** 
 * Keyboard function.
 *
 * Called to treat pressed keys.
 *
 * @param key Pressed key.
 * @param x Mouse x coordinate when key pressed.
 * @param y Mouse y coordinate when key pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            glutLeaveMainLoop();
        case 'q':
        case 'Q':
            glutLeaveMainLoop();
        case 'j':
        case 'J':
            animation = 1;
        
    }
    glutPostRedisplay();   
}


void idle()
{   
    if (animation && i < 7)
    {
        i++;
        sleep(3);
    }
    if (i > 6)
    {
        animation = 0;
        i = 0;
    }
    glutPostRedisplay();
}

/**
 * Init vertex data.
 *
 * Defines the coordinates for vertices, creates the arrays for OpenGL.
 */
void initData()
{
    // Set cube vertices.
    float vertices[] = {
        // coordinate      // normal
        
        //left
	    -0.25f, -0.25f,  0.25f,  0.0f,  0.0f,  1.0f,
        0.25f, -0.25f,  0.25f,  0.0f,  0.0f,  1.0f,
        0.25f,  0.25f,  0.25f,  0.0f,  0.0f,  1.0f,

        -0.25f, -0.25f,  0.25f,  0.0f,  0.0f,  1.0f,
        0.25f,  0.25f,  0.25f,  0.0f,  0.0f,  1.0f,
        -0.25f,  0.25f,  0.25f,  0.0f,  0.0f,  1.0f,

        //front
        0.25f, -0.25f,  0.25f,  1.0f,  0.0f,  0.0f, 
        0.25f, -0.25f, -0.25f,  1.0f,  0.0f,  0.0f,
        0.25f,  0.25f, -0.25f,  1.0f,  0.0f,  0.0f,

        0.25f, -0.25f,  0.25f,  1.0f,  0.0f,  0.0f,
        0.25f,  0.25f, -0.25f,  1.0f,  0.0f,  0.0f,
        0.25f,  0.25f,  0.25f,  1.0f,  0.0f,  0.0f,

        //right
        0.25f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
        -0.25f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
        -0.25f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,

        0.25f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
        -0.25f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
        0.25f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,


        //back
        -0.25f, -0.25f, -0.25f, -1.0f,  0.0f,  0.0f,
        -0.25f, -0.25f,  0.25f, -1.0f,  0.0f,  0.0f,
        -0.25f,  0.25f,  0.25f, -1.0f,  0.0f,  0.0f,

        -0.25f, -0.25f, -0.25f, -1.0f,  0.0f,  0.0f,
        -0.25f,  0.25f,  0.25f, -1.0f,  0.0f,  0.0f,
        -0.25f,  0.25f, -0.25f, -1.0f,  0.0f,  0.0f,

        //top
        -0.25f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,
        0.25f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,
        0.25f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,

        -0.25f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,
        0.25f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,
        -0.25f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,

        //bottom
        -0.25f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,
        -0.25f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,
        0.25f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,

        -0.25f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,
        0.25f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,
        0.25f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f
    };
    
    // Vertex array.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Set attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind Vertex Array Object.
    glBindVertexArray(0);
    
    
    glEnable(GL_DEPTH_TEST);
}

/** Create program (shaders).
 * 
 * Compile shaders and create the program.
 */
void initShaders()
{
    // Request a program and shader slots from GPU
    program = createShaderProgram(vertex_code, fragment_code);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(win_width,win_height);
	glutCreateWindow(argv[0]);
	glewInit();

    // Init vertex data for the triangle.
    initData();
    
    // Create shaders.
    initShaders();
	
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

	glutMainLoop();
}