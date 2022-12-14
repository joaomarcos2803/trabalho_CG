/**
 * @file trabalho.cpp
 * Applies the Phong method.
 * 
 * @author Adriano Lucas Ferreira
 * @author Carlos Eduardo da Cruz Brito
 * @author Matheus Martins Batista
 * @author João Marcos Cucovia
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
int win_width  = 900;
/** Window height. */
int win_height = 700;

/** Program variable. */
int program;
/** Vertex array object. */
unsigned int VAO;
/** Vertex buffer object. */
unsigned int VBO;

/* cores dos cubos */
float r[10], g[10], b[10];

//controla a animação de mudar as cores dos cubos
int i = 0;

//0 - sem animação, 1 - DFS, 2 - BFS
int animation = 0;

/** zoom camera */
float zoom = 0;

/** light in cube*/
float light = 0;

/** move cube*/
float inY = 0;
float inX = -1;

/** cube and line scale*/
float scale = 0;

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
    glLineWidth(10);
	glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(1.0f,0.0f,0.0f));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(65.0f), glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1 + scale, 1 + scale, 1.0));
	glm::mat4 model = S*Rx*Ry;
	unsigned int loc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 projection = glm::perspective(glm::radians(zoom + 60.0f), (win_width/(float)win_height), 0.1f, 120.0f);
 	loc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

    // Light color.
    unsigned int locLight = glGetUniformLocation(program, "lightColor");
    glUniform3f(locLight, 1.0, 1.0, 1.0);

    //Light position.
    loc = glGetUniformLocation(program, "lightPosition");
    glUniform3f(loc, light + 1.0, light + 3.0, light + 2.0);
    	
    // Camera position.
    loc = glGetUniformLocation(program, "cameraPosition");
    glUniform3f(loc, 0.0, 0.0, 0.0);

    // Object color.
    unsigned int locColor = glGetUniformLocation(program, "objectColor");

    unsigned int locView = glGetUniformLocation(program, "view");

    //indices dos cubos para fazer a animação de cada algoritmo
    int BFS[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int DFS[10] = {0, 1, 4, 7, 2, 5, 8, 3, 6, 9};

    //DFS
    if (animation == 1)
    {
        for (int j = 0; j <= i; j++)
        {
            r[DFS[j]] = 0.0f;
            g[DFS[j]] = 1.0f;
            b[DFS[j]] = 0.0f;
        }
    }

    //BFS
    else if (animation == 2)
    {
        for (int j = 0; j <= i; j++)
        {
            r[BFS[j]] = 0.0f;
            g[BFS[j]] = 1.0f;
            b[BFS[j]] = 0.0f;
        }   
    }
    else 
    {
        for (int j = 0; j < 10; j++)
        {
            r[j] = 0.0f;
            g[j] = 0.0f;
            b[j] = 1.0f;
        }
    }
    
    //Primeiro cubo que irá começar a BFS/DFS
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + -1.5f, inY + 0.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[0], g[0], b[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    

    //Primeira coluna de cubos
    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 0.0f, inY + 2.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[1], g[1], b[1]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 37, 2);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 0.0f, inY + 0.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[2], g[2], b[2]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 41, 2);


    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 0.0f, inY + -2.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[3], g[3], b[3]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 39, 2);


    //Segunda coluna de cubos
    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 1.5f, inY + 2.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[4], g[4], b[4]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 41, 2);


    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 1.5f, inY + 0.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[5], g[5], b[5]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 41, 2);


    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 1.5f, inY + -2.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[6], g[6], b[6]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 41, 2);


    //Terceira coluna de cubos
    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 3.0f, inY + 2.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[7], g[7], b[7]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 41, 2);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 3.0f, inY + 0.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[8], g[8], b[8]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 41, 2);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(inX + 3.0f, inY + -2.0f, -5.0f));
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(locColor, r[9], g[9], b[9]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_LINES, 41, 2);
    
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
            exit(0);

        case 'q':
        case 'Q':
            exit(0);

        //DFS
        case 'n':
        case 'N':
            animation = 1;
            break;
        
        //BFS
        case 'm':
        case 'M':
            animation = 2;
            break;

        //Zoom camera
        case 'z':
        case 'Z':
            zoom -= 2;
            break;

        case 'x':
        case 'X':
            zoom += 2;
            break;

        //Seta a posição inicial dos cubos
        case 'p':
        case 'P':
            zoom = 0;
            light = 0; 
            inX = -1.0;
            inY = 0;
            break;

        //Movimentar os cubos na tela
        case 'w':
        case 'W':
            inY += 0.5;
            break;

        case 's':
        case 'S':
            inY -= 0.5;
            break;

        case 'a':
        case 'A':
            inX -= 0.5;
            break;

        case 'd':
        case 'D':
            inX += 0.5;
            break;

        //Light control
        case 'l':
        case 'L':
            light += 0.5;
            break;

        case 'k':
        case 'K':
            light -= 0.5;
            break;
        
    }
    glutPostRedisplay();   
}


void idle()
{   
    if (animation && i < 10)
    {   
        scale = 0.115;
        i++;
        sleep(1);
    }
    if (i > 9)
    {
        animation = 0;
        i = 0;
        scale = 0;
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
        0.25f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,

        //lines
        0.25f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,
        -0.75f, -1.8f,  -1.5f,  0.0f, -1.0f,  0.0f,

        0.25f, 0.25f, -0.25f,  0.0f, -1.0f,  0.0f,
        -0.525f, 1.8f,  -1.5f,  0.0f, -1.0f,  0.0f,

        0.25f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,
        -0.75f, 0.0f,  -1.5f,  0.0f, -1.0f,  0.0f,
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
	glutCreateWindow("Trabalho Final - CIC270");
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