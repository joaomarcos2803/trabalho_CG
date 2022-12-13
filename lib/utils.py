## @file utils.py
# Util functions.
#
# Defines a set of frequently used functions.
#
# @author Ricardo Dutra da Silva

import os
import sys
import math
import numpy as np
import OpenGL.GL as gl

## Read shader.
#
# Read a shader from a file.
#
# @param File name.
# @return String with shader code.
def readShaderFile(shader_file):

    # Check if file exists.
    if not os.path.isfile(shader_file):
        print("Could not open shader file: " + shader_file)
        sys.exit()

    # Read file.
    shader_code = None
    with open(shader_file, 'r') as f:
        shader_code = f.read()

    return shader_code


## Create program.
#
# Creates a program from given shader codes.
#
# @param vertex_code String with code for vertex shader.
# @param fragment_code String with code for fragment shader.
# @return Compiled program.
def createShaderProgram(vertex_code, fragment_code):
    
    # Request a program and shader slots from GPU
    program  = gl.glCreateProgram()
    vertex   = gl.glCreateShader(gl.GL_VERTEX_SHADER)
    fragment = gl.glCreateShader(gl.GL_FRAGMENT_SHADER)
    
    # Set shaders source
    gl.glShaderSource(vertex, vertex_code)
    gl.glShaderSource(fragment, fragment_code)

    # Compile shaders
    gl.glCompileShader(vertex)
    if not gl.glGetShaderiv(vertex, gl.GL_COMPILE_STATUS):
        error = gl.glGetShaderInfoLog(vertex).decode()
        print(error)
        raise RuntimeError("Shader compilation error")
                
    gl.glCompileShader(fragment)
    if not gl.glGetShaderiv(fragment, gl.GL_COMPILE_STATUS):
        error = gl.glGetShaderInfoLog(fragment).decode()
        print(error)
        raise RuntimeError("Shader compilation error")                

    # Attach shader objects to the program
    gl.glAttachShader(program, vertex)
    gl.glAttachShader(program, fragment)

    # Build program
    gl.glLinkProgram(program)
    if not gl.glGetProgramiv(program, gl.GL_LINK_STATUS):
        print(gl.glGetProgramInfoLog(program))
        raise RuntimeError('Linking error')

    # Get rid of shaders (not needed anymore)
    gl.glDetachShader(program, vertex)
    gl.glDetachShader(program, fragment)
    gl.glDeleteShader(vertex)
    gl.glDeleteShader(fragment)

    return program

## Identity matrix.
#
# Creates a identity matrix.
#
# @return The matrix.
def matIdentity():
    I = np.identity(4, dtype='float32')
    
    return I

## Translation matrix.
#
# Creates a translation matrix with x, y, and z displacements.
#
# @param x Displacement along x-axis. 
# @param y Displacement along y-axis. 
# @param z Displacement along z-axis. 
# @return The matrix.
def matTranslate(x, y, z):
    T =  np.identity(4, dtype='float32')

    T[0,3] = x
    T[1,3] = y
    T[2,3] = z

    return T

## Scale matrix.
#
# Creates a scale matrix with x, y, and z factors.
#
# @param x Scale factor along x-axis. 
# @param y Scale factor along y-axis. 
# @param z Scale factor along z-axis. 
# @return The matrix.
def matScale(x, y, z):
    T =  np.identity(4, dtype='float32')

    T[0,0] = x
    T[1,1] = y
    T[2,2] = z

    return T

## X Rotation matrix.
#
# Creates a rotation matrix for the x-axis.
#
# @param angle Rotation angle in radians. 
# @return The matrix.
def matRotateX(angle):
    R =  np.identity(4, dtype='float32')

    acos = math.cos(angle)
    asin = math.sin(angle)

    R[1,1] =  acos
    R[1,2] = -asin
    R[2,1] =  asin
    R[2,2] =  acos

    return R

## Y Rotation matrix.
#
# Creates a rotation matrix for the y-axis.
#
# @param angle Rotation angle in radians. 
# @return The matrix.
def matRotateY(angle):
    R =  np.identity(4, dtype='float32')

    acos = math.cos(angle)
    asin = math.sin(angle)

    R[0,0] =  acos
    R[0,2] =  asin
    R[2,0] = -asin
    R[2,2] =  acos

    return R

## Z Rotation matrix.
#
# Creates a rotation matrix for the z-axis.
#
# @param angle Rotation angle in radians. 
# @return The matrix.
def matRotateZ(angle):
    R =  np.identity(4, dtype='float32')

    acos = math.cos(angle)
    asin = math.sin(angle)

    R[0,0] =  acos
    R[0,1] = -asin
    R[1,0] =  asin
    R[1,1] =  acos

    return R

## Perspective matrix.
#
# Creates a perspective matrix.
#
# @param fovy y field of view. 
# @param aspect aspect. 
# @param n near plane. 
# @param f far plane. 
# @return The matrix.
def matPerspective(fovy, aspect, n, f):

    P = np.zeros((4,4), dtype='float32')
    
    rad = fovy
    tan = math.tan(rad/2.0)

    P[0,0] = 1.0/(aspect*tan)
    P[1,1] = 1.0/tan
    P[2,2] = -(f+n)/(f-n);
    P[2,3] = - (2.0*f*n)/(f-n);
    P[3,2] = -1.0;

    return P

## Perspective matrix.
#
# Creates a perspective matrix.
#
# @param l left. 
# @param r right. 
# @param b bottom. 
# @param u top. 
# @param n near plane. 
# @param f far plane. 
# @return The matrix.
def matFrustum(l, r, b, t, n, f):
    
    F = np.zeros((4,4), dtype='float32')

    F[0,0] = (2.0*n)/(r-l)
    F[0,2] = (r+l)/(r-l)
    F[1,1] = (2.0*n)/(t-b)
    F[1,2] = (t+b)/(t-b)
    F[2,2] = (-(f+n))/(f-n)
    F[2,3] = (-2.0*f*n)/(f-n)
    F[3,2] = -1.0

    return F

## Orthographic matrix.
#
# Creates an orthographic matrix.
#
# @param l left. 
# @param r right. 
# @param b bottom. 
# @param u top. 
# @param n near plane. 
# @param f far plane. 
# @return The matrix.
def matOrtho(l, r, b, t, n, f):

    F = np.zeros((4,4), dtype='float32')

    F[0,0] = 2.0/(r-l)
    F[0,3] = -(r+l)/(r-l)
    F[1,1] = 2.0/(t-b)
    F[1,3] = -(t+b)/(t-b)
    F[2,2] = -2.0/(f-n)
    F[2,3] = -(f+n)/(f-n)
    F[3,3] = 1.0

    return F
