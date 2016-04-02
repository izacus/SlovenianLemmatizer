// Windows compilation needs exported initLibLemmagen or the compilation will fail.
// Insert this file on Windows builds to make it build properly.

#ifdef _WIN32

#include <Python.h>
#define _WIN32_WINNT 0x400

PyMODINIT_FUNC
__declspec(dllexport)  initlibLemmagen(void) {
    Py_InitModule("libLemmagen", NULL);
}

#endif