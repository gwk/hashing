// Dedicated to the public domain under CC0: https://creativecommons.org/publicdomain/zero/1.0/.

#include "hashing_cpy.h"


PyDoc_STRVAR(hashing_doc, "Various hashing functions.");


static void _Aquahash_dealloc(Aquahash *self) {
  if (self->lock) {
    PyThread_free_lock(self->lock);
    self->lock = NULL;
  }
  PyObject_Del(self);
}


static PyObject *_Aquahash_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {

  Aquahash *self = NULL;
  Py_buffer salt = {.buf=NULL, .obj=NULL, .len=0};

  static const char * const _keywords[] = {"salt", NULL};
  static _PyArg_Parser _parser = {"|$s*:Aquahash", _keywords, 0};
  if (!_PyArg_ParseTupleAndKeywordsFast(args, kwargs, &_parser, &salt)) goto cleanup;

  self = Aquahash_new(type, salt);

  cleanup:
  if (salt.obj) PyBuffer_Release(&salt);
  return (PyObject*)self;
}


static PyObject *_Aquahash_update(Aquahash *self, PyObject *args) {
  PyObject *res = NULL;
  Py_buffer data = {.buf=NULL, .obj=NULL, .len=0};
  if (!PyArg_ParseTuple(args, "s*:update", &data)) goto cleanup;

  Aquahash_update(self, data);
  res = Py_None;
  Py_INCREF(res);

  cleanup:
  if (data.obj) PyBuffer_Release(&data);
  return res;
}

static PyObject *_Aquahash_digest(Aquahash *self, PyObject *noargs) {
  return Aquahash_digest(self);
}

static PyObject *_Aquahash_digest_size(Aquahash *self, PyObject *noargs) {
  return Aquahash_digest_size(self);
}


static PyObject *_Aquahash_block_size(Aquahash *self, PyObject *noargs) {
  return Aquahash_block_size(self);
}


static struct PyMethodDef Aquahash_methods[] = {
  {"update", (PyCFunction)_Aquahash_update, METH_VARARGS},
  {"digest", (PyCFunction)_Aquahash_digest, METH_NOARGS},
  {"digest_size", (PyCFunction)_Aquahash_digest_size, METH_NOARGS},
  {"block_size", (PyCFunction)_Aquahash_block_size, METH_NOARGS},
  {NULL, NULL}
};

static PyTypeObject Aquahash_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Aquahash",
    .tp_basicsize = sizeof(Aquahash),
    .tp_doc = "Various hash functions.",
    .tp_dealloc = (destructor)_Aquahash_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_methods = Aquahash_methods,
    .tp_new = _Aquahash_new
};


PyDoc_STRVAR(aquahash_doc, "Hash a `str` or bytes-like value using the AquaHash function.");


static PyObject *_hashing_aquahash(PyObject *module, PyObject *args, PyObject *kwargs) {
  Py_buffer data = {.buf=NULL, .obj=NULL, .len=0};
  Py_buffer salt = {.buf=NULL, .obj=NULL, .len=0};
  PyObject *ret = NULL;

  static const char * const _keywords[] = {"data", "salt", NULL};
  static _PyArg_Parser _parser = {"s*|$s*:aquahash", _keywords, 0};
  if (!_PyArg_ParseTupleAndKeywordsFast(args, kwargs, &_parser, &data, &salt)) goto cleanup;

  ret = hashing_aquahash(data, salt);

  cleanup:
  if (data.obj) PyBuffer_Release(&data);
  if (salt.obj) PyBuffer_Release(&salt);
  return ret;
}


static struct PyMethodDef module_functions[] = {
  {"aquahash", (PyCFunction)_hashing_aquahash, METH_VARARGS|METH_KEYWORDS, aquahash_doc },
  {NULL, NULL}
};


static struct PyModuleDef module_def = {
  PyModuleDef_HEAD_INIT,
  .m_name = "hashing_cpy",
  .m_doc = hashing_doc,
  .m_size = 0,
  .m_methods = module_functions,
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
