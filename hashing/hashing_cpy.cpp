// Dedicated to the public domain under CC0: https://creativecommons.org/publicdomain/zero/1.0/.

#include "hashing_cpy.h"


PyDoc_STRVAR(hashing_cpy_doc,
"Hashing C++ extension."
);


// `class Aquahash`.

static void _Aquahash_dealloc(Aquahash *self) {
  Aquahash_dealloc(self);
  PyObject_Del(self);
}


PyDoc_STRVAR(Aquahash_doc,
"Aquahash."
);


// `def __new__(cls, salt: Union[str, ByteString] = Ellipsis) -> 'Aquahash'`.

PyDoc_STRVAR(Aquahash___new___doc,
"Create a new AquaHash hashing object with an optional salt."
);


static PyObject *_Aquahash___new__(PyTypeObject *cls, PyObject *args, PyObject *kwargs) {
  Aquahash *_ret = NULL;
  PyObject *ret = NULL;
  Py_buffer salt = {.buf=NULL, .obj=NULL, .len=0};

  static const char * const _keywords[] = {"salt", NULL};
  static _PyArg_Parser _parser = {"|$s*:Aquahash", _keywords, 0};
  if (!_PyArg_ParseTupleAndKeywordsFast(args, kwargs, &_parser, &salt)) goto cleanup;

  _ret = Aquahash___new__(cls, salt);
  ret = (PyObject*)(_ret);

  cleanup:
  if (salt.obj) PyBuffer_Release(&salt);
  return ret;
}


// `def update(self, data: Union[str, ByteString]) -> None`.

PyDoc_STRVAR(Aquahash_update_doc,
"Update the hash object with bytes from `data`."
);


static PyObject *_Aquahash_update(Aquahash *self, PyObject *args, PyObject *kwargs) {
  PyObject *ret = NULL;
  Py_buffer data = {.buf=NULL, .obj=NULL, .len=0};

  static const char * const _keywords[] = {"data", NULL};
  static _PyArg_Parser _parser = {"s*:update", _keywords, 0};
  if (!_PyArg_ParseTupleAndKeywordsFast(args, kwargs, &_parser, &data)) goto cleanup;

  Aquahash_update(self, data);
  ret = Py_None;
  Py_INCREF(ret);

  cleanup:
  if (data.obj) PyBuffer_Release(&data);
  return ret;
}


// `def digest(self) -> bytes`.

PyDoc_STRVAR(Aquahash_digest_doc,
"Produce a digest of the bytes hashed so far."
);


static PyObject *_Aquahash_digest(Aquahash *self, PyObject *noargs) {
  PyBytesObject *_ret = NULL;
  PyObject *ret = NULL;
  _ret = Aquahash_digest(self);
  ret = (PyObject *)(_ret);
  return ret;
}


// `def block_size(self) -> int`.

PyDoc_STRVAR(Aquahash_block_size_doc,
"The size in bytes of the AquaHash algorithm blocks."
);


static PyObject *_Aquahash_block_size(Aquahash *self, PyObject *noargs) {
  PyObject *_ret = NULL;
  PyObject *ret = NULL;
  _ret = Aquahash_block_size(self);
  ret = (PyObject*)(_ret);
  return ret;
}


// `def digest_size(self) -> int`.

PyDoc_STRVAR(Aquahash_digest_size_doc,
"The size in bytes of the AquaHash algorithm digest."
);


static PyObject *_Aquahash_digest_size(Aquahash *self, PyObject *noargs) {
  PyObject *_ret = NULL;
  PyObject *ret = NULL;
  _ret = Aquahash_digest_size(self);
  ret = (PyObject*)(_ret);
  return ret;
}


static struct PyMethodDef Aquahash_methods[] = {
  {"update", (PyCFunction)_Aquahash_update, METH_VARARGS|METH_KEYWORDS, Aquahash_update_doc},
  {"digest", (PyCFunction)_Aquahash_digest, METH_NOARGS, Aquahash_digest_doc},
  {"block_size", (PyCFunction)_Aquahash_block_size, METH_NOARGS, Aquahash_block_size_doc},
  {"digest_size", (PyCFunction)_Aquahash_digest_size, METH_NOARGS, Aquahash_digest_size_doc},
  {NULL, NULL}
};

static PyTypeObject Aquahash_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Aquahash",
    .tp_basicsize = sizeof(Aquahash),
    .tp_doc = Aquahash_doc,
    .tp_dealloc = (destructor)_Aquahash_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_methods = Aquahash_methods,
    .tp_new = _Aquahash___new__,
};


// `def aquahash(data: Union[str, ByteString], salt: Union[str, ByteString] = Ellipsis) -> bytes`.

PyDoc_STRVAR(aquahash_doc,
"Hash a `str` or bytes-like value using the AquaHash function."
);


static PyObject *_aquahash(PyObject *module, PyObject *args, PyObject *kwargs) {
  PyBytesObject *_ret = NULL;
  PyObject *ret = NULL;
  Py_buffer data = {.buf=NULL, .obj=NULL, .len=0};
  Py_buffer salt = {.buf=NULL, .obj=NULL, .len=0};

  static const char * const _keywords[] = {"data", "salt", NULL};
  static _PyArg_Parser _parser = {"s*|$s*:aquahash", _keywords, 0};
  if (!_PyArg_ParseTupleAndKeywordsFast(args, kwargs, &_parser, &data, &salt)) goto cleanup;

  _ret = aquahash(data, salt);
  ret = (PyObject *)(_ret);

  cleanup:
  if (data.obj) PyBuffer_Release(&data);
  if (salt.obj) PyBuffer_Release(&salt);
  return ret;
}


static struct PyMethodDef hashing_cpy_methods[] = {
  {"aquahash", (PyCFunction)_aquahash, METH_VARARGS|METH_KEYWORDS, aquahash_doc},
  {NULL, NULL}
};


static struct PyModuleDef module_def = {
  PyModuleDef_HEAD_INIT,
  .m_name = "hashing_cpy",
  .m_doc = hashing_cpy_doc,
  .m_size = 0,
  .m_methods = hashing_cpy_methods,
  .m_slots = NULL, // Single-phase initialization.
};


PyMODINIT_FUNC
PyInit_hashing_cpy(void) {

  PyObject *module = PyModule_Create(&module_def);
  if (!module) return NULL;

  if (PyType_Ready(&Aquahash_type) < 0) return NULL;
  Py_INCREF(&Aquahash_type);
  PyModule_AddObject(module, Aquahash_type.tp_name, (PyObject *)&Aquahash_type);

  return module;
}
