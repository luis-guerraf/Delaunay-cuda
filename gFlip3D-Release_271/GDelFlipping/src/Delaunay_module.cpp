/*
Author: Cao Thanh Tung, Ashwin Nanjappa
Date:   05-Aug-2014

===============================================================================

Copyright (c) 2011, School of Computing, National University of Singapore. 
All rights reserved.

Project homepage: http://www.comp.nus.edu.sg/~tants/gdel3d.html

If you use gDel3D and you like it or have comments on its usefulness etc., we 
would love to hear from you at <tants@comp.nus.edu.sg>. You may share with us
your experience and any possibilities that we may improve the work/code.

===============================================================================

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the National University of University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission from the National University of Singapore. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/
#include "gDel3D/GpuDelaunay.h"
#include <Python.h>
#include <numpy/arrayobject.h>
#include <iomanip>
#include <unistd.h>
#include <iostream>
#define REAL_TYPE_FP32

GDelOutput   output;

// Our Python binding to our C function
// This will take one and only one non-keyword argument
static PyObject* triangulation(PyObject* self, PyObject* args) {
    PyArrayObject* input;

    if (!PyArg_ParseTuple(args, "O!", &PyArray_Type, &input))
        return NULL;

    // Data type
//    printf("Type_num: %d, %d\n", input->descr->type_num, PyArray_FLOAT);

    // Copy the arrays into the Point3 structs (coulnd't find an efficient way)
    float *data_array = (float *)PyArray_DATA((PyArrayObject *)input);
    npy_intp *shape = PyArray_DIMS((PyArrayObject *)input);
    uint N = shape[0];
    Point3 point_array[N];
    uint i, j;
    for(i=0; i < N; ++i){
        for(j=0; j < 3; ++j){
            point_array[i]._p[j] = data_array[j+i*3];
//            std::cout << i << "\t" << j << "\t" << data_array[i+j*3] << std::endl;
        }
    }

    // Create vector of points
//    Point3HVec pointVec(point_array, point_array + sizeof point_array / 3); // Efficient way but has a bug
    Point3HVec   pointVec;

    // Sanity check
    for(uint i=0; i < N; ++i){
        pointVec.push_back(point_array[i]);
//        pointVec[i].printOut();
    }

    GpuDel triangulator;

    triangulator.compute(pointVec, &output);

    npy_intp dims[2];
    dims[0] = output.tetVec.size();
    dims[1] = 4;
    return PyArray_SimpleNewFromData(2, dims, PyArray_INT, &output.tetVec[0]);
}

static PyObject* helloworld(PyObject* self, PyObject* args) {
    printf("Hello World\n");
    return Py_None;
}

static PyMethodDef myMethods[] = {
    {"helloworld", helloworld, METH_NOARGS, "Prints Hello World"},
    {"triangulation", triangulation, METH_VARARGS, "Computes the Delaunay triangulation"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef myModule = {
    PyModuleDef_HEAD_INIT, "delaunay_triangulation",
    "delaunay_triangulation", -1, myMethods
};

PyMODINIT_FUNC PyInit_Delaunay_module(void) {
    import_array();
    return PyModule_Create(&myModule);
}

