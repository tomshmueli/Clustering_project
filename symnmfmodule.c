# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include "symnmf.h"

MATRIX *create_cMat_from_pyList(PyObject *py_lst)
{
    int col, row, i, j;
    PyObject *item;
    MATRIX *cMat;
 
    cMat = (MATRIX *)malloc(sizeof(MATRIX));
    col = PyList_Size(py_lst);
    row = PyList_Size(PyList_GetItem(py_lst, 0));
    if (cMat == NULL || init_matrix(cMat, col, row) == 1) {
        return NULL;
    }
    for (i = 0; i < cMat->col; i++) {
        item = PyList_GetItem(py_lst, i);
        for (j = 0; j < cMat->row; j++) {
            cMat->cord[i][j] = PyFloat_AsDouble(PyList_GetItem(item, j));
        }
    }
    return cMat;
}


PyObject *create_pyList_from_cMat(MATRIX *result) 
{
    int i, j, col, row;
    PyObject *pMat, *python_row, *python_float;

    col = result->col;
    row = result->row;
    pMat = PyList_New(col);
    for (i = 0; i < col; i++) {
        python_row = PyList_New(row);
        for (j = 0; j < row; j++) {
            python_float = Py_BuildValue("f", result->cord[i][j]);
            PyList_SetItem(python_row, j, python_float);
        }
        PyList_SetItem(pMat, i, python_row);
    }
    return pMat;
} 

/**
 * Wrapper function for sym.
*/
static PyObject *sym_wrapper(PyObject *self, PyObject *args) 
{
    MATRIX *X, *A;
    PyObject *dp_list, *pMat;
    pMat = NULL;

    if (!PyArg_ParseTuple(args, "O", &dp_list)) {
        return NULL;
    }

    X = create_cMat_from_pyList(dp_list);
    A = (MATRIX *)malloc(sizeof(MATRIX));
    if (X == NULL || init_matrix(A, X->col, X->col) == 1) {
        printf("An Error Has Occurred");
        free_matrix(X);
        free_matrix(A);
        return NULL;
    }
    sym(X, A);
    pMat = create_pyList_from_cMat(A);

    return pMat;
}

/**
 * Wrapper function for ddg.
*/
static PyObject *ddg_wrapper(PyObject *self, PyObject *args) 
{
    MATRIX *X, *D;
    PyObject *dp_list, *pMat;
    pMat = NULL;

    if (!PyArg_ParseTuple(args, "O", &dp_list)) {
        return NULL;
    }

    X = create_cMat_from_pyList(dp_list);
    D = (MATRIX *)malloc(sizeof(MATRIX));
    if (X == NULL || init_matrix(D, X->col, X->col) == 1) {
        printf("An Error Has Occurred");
        free_matrix(X);
        free_matrix(D);
        return NULL;
    }
    ddg(X, D);
    pMat = create_pyList_from_cMat(D);

    return pMat;
}

/**
 * Wrapper function for norm.
*/
static PyObject *norm_wrapper(PyObject *self, PyObject *args) 
{
    MATRIX *X, *W;
    PyObject *dp_list, *pMat;
    pMat = NULL;

    if (!PyArg_ParseTuple(args, "O", &dp_list)) {
        return NULL;
    }

    X = create_cMat_from_pyList(dp_list);
    W = (MATRIX *)malloc(sizeof(MATRIX));
    if (X == NULL || init_matrix(W, X->col, X->col) == 1) {
        printf("An Error Has Occurred");
        free_matrix(X);
        free_matrix(W);
        return NULL;
    }
    norm(X, W);
    pMat = create_pyList_from_cMat(W);

    return pMat;
}

/**
 * Wrapper function for symnmf.
*/
static PyObject *symnmf_wrapper(PyObject *self, PyObject *args) 
{
    MATRIX *W, *H;
    PyObject *W_lst, *H_lst, *pMat;
    pMat = NULL;

    if (!PyArg_ParseTuple(args, "OO", &W_lst, &H_lst)) {
        return NULL;
    }
    
    W = create_cMat_from_pyList(W_lst);
    H = create_cMat_from_pyList(H_lst);
    if (W == NULL || H == NULL) {
        printf("An Error Has Occurred");
        free_matrix(W);
        free_matrix(H);
        return NULL;
    }
    symnmf(W, H);
    pMat = create_pyList_from_cMat(H);

    return pMat;
}

/**
 * Module's function table.
*/
static PyMethodDef symnmf_FunctionsTable[] = {
    {
        "sym",
        (PyCFunction) sym_wrapper, /* the C-function that implements the Python sym function and returns static PyObject*  */
        METH_VARARGS,
        PyDoc_STR("Returns the similarity matrix.")
    }, {
        "ddg",
        (PyCFunction) ddg_wrapper, /* the C-function that implements the Python ddg function and returns static PyObject*  */
        METH_VARARGS,
        PyDoc_STR("Returns the diagonal degree matrix.")
    }, {
        "norm",
        (PyCFunction) norm_wrapper, /* the C-function that implements the Python norm function and returns static PyObject*  */
        METH_VARARGS,
        PyDoc_STR("Returns the normalized similarity matrix.")
    }, {
        "symnmf",
        (PyCFunction) symnmf_wrapper, /* the C-function that implements the Python symnmf function and returns static PyObject*  */
        METH_VARARGS,
        PyDoc_STR("Returns the H after convergence.")
    }, {
        NULL, NULL, 0, NULL
    }
};


/** 
 * Module's definition.
*/
static struct PyModuleDef symnmfAPI = {
    PyModuleDef_HEAD_INIT,
    "symnmfAPI",
    "Python wrapper for custom C extension library.",
    -1,
    symnmf_FunctionsTable
};

/**
 * Module's init.
*/
PyMODINIT_FUNC PyInit_symnmfAPI(void) {
    PyObject *col;
    col = PyModule_Create(&symnmfAPI);
    if (!col) {
        return NULL;
    }
    return col;
};
