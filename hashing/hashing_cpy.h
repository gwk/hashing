// Dedicated to the public domain under CC0: https://creativecommons.org/publicdomain/zero/1.0/.

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "aquahash.h"


const Py_ssize_t GIL_MIN_SIZE = 4096;

#define min(a, b) \
  ({__typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })


typedef struct {
  PyObject_HEAD;
  AquaHash state;
  PyThread_type_lock lock;
} Aquahash;


// `class Aquahash`.

inline static void Aquahash_dealloc(Aquahash *self) {
  if (self->lock) {
    PyThread_free_lock(self->lock);
    self->lock = NULL;
  }
}


// `def __new__(cls, salt: Union[str, ByteString] = Ellipsis) -> 'Aquahash'`.
inline static Aquahash *Aquahash___new__(PyTypeObject *cls, Py_buffer salt) {
  Aquahash *self = (Aquahash *) cls->tp_alloc(cls, 0);
  if (!self) return NULL;

  __m128i salt_val = {0, 0};
  if (salt.len) memcpy(&salt_val, salt.buf, min(salt.len, (long)sizeof(salt_val)));
  self->state = AquaHash(salt_val);
  self->lock = NULL;
  return self;
}


// `def update(self, data: Union[str, ByteString]) -> None`.
inline static void Aquahash_update(Aquahash *self, Py_buffer data) {
  // Release the GIL for large inputs.
  // This strategy is copied from the standard library blake2 implementation.
  bool should_allow_threads = (data.len >= GIL_MIN_SIZE);
  if (should_allow_threads && !self->lock) {
    self->lock = PyThread_allocate_lock(); // Can fail, resulting in NULL.
  }
  if (should_allow_threads && self->lock) {
    Py_BEGIN_ALLOW_THREADS
    PyThread_acquire_lock(self->lock, 1); // Protect this hasher from concurrent mutation.
    self->state.Update((uint8_t*)data.buf, data.len);
    PyThread_release_lock(self->lock);
    Py_END_ALLOW_THREADS
  } else { // Either too small to be worthwhile, or else could not allocate the lock.
    self->state.Update((uint8_t*)data.buf, data.len);
  }
}


// `def digest(self) -> bytes`.
inline static PyBytesObject *Aquahash_digest(Aquahash *self) {
  AquaHash copied_state = self->state;
  __m128i digest = copied_state.Finalize();
  return (PyBytesObject *)Py_BuildValue("y#", &digest, sizeof(digest));
}


// `def block_size(self) -> int`.
inline static PyObject *Aquahash_block_size(Aquahash *self) {
  return Py_BuildValue("i", 64);
}


// `def digest_size(self) -> int`.
inline static PyObject *Aquahash_digest_size(Aquahash *self) {
  return Py_BuildValue("i", 16);
}



// `def aquahash(data: Union[str, ByteString], salt: Union[str, ByteString] = Ellipsis) -> bytes`.
inline static PyBytesObject *aquahash(Py_buffer data, Py_buffer salt) {

  __m128i salt_val = {0, 0};
  if (salt.len) memcpy(&salt_val, salt.buf, min(salt.len, (long)sizeof(salt_val)));

  __m128i digest;
  if (data.len >= GIL_MIN_SIZE) { // Unlike update(), we do not need a lock here because all state is private.
    Py_BEGIN_ALLOW_THREADS
    digest = AquaHash::Hash((uint8_t*)data.buf, data.len, salt_val);
    Py_END_ALLOW_THREADS
  } else {
    digest = AquaHash::Hash((uint8_t*)data.buf, data.len, salt_val);
  }

  return (PyBytesObject *)Py_BuildValue("y#", &digest, sizeof(digest));
}
