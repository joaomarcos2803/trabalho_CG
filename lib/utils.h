/**
 * @file utils.h
 * Util functions.
 *
 * Defines a set of util frequently used functions.
 *
 * @author Ricardo Dutra da Silva
 */

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>


/** 
 * Create program.
 *
 * Creates a program from given shader codes.
 *
 * @param vertex_code String with code for vertex shader.
 * @param fragment_code String with code for fragment shader.
 * @return Compiled program.
 */
int createShaderProgram(const char *, const char *);

