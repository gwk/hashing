#!/usr/bin/env python3

from utest import *
from hashing import Aquahash, aquahash


h = Aquahash()
d_0 = h.digest()
h.update('a')
d_a = h.digest()
h.update('b')
d_ab = h.digest()

utest_val_ne(d_0, d_a, 'incremental updates produce different values.')
utest_val_ne(d_a, d_ab, 'incremental updates produce different values.')

utest(d_a, aquahash, 'a')
utest(d_ab, aquahash, 'ab')

for i in range(0, 18): # Any combination of zero bytes is equivalent to no salt.
  utest(d_a, aquahash, 'a', salt='\x00'*i)

utest(d_a, aquahash, 'a', salt='\x00'*16 + '\x01') # Salt bytes beyond the 16-byte limit are ignored.

utest_val_ne(d_a, aquahash('a', salt='\x01'), 'Different salts produce different values.')

# Supports str and bytelike inputs.
utest(d_a, aquahash, b'a')
utest(d_0, aquahash, bytearray(b''))
utest(d_a, aquahash, bytearray(b'a'))
utest(d_ab, aquahash, bytearray(b'ab'))

# Test whole and incremental hashing for a variety of sizes.
for i in range(0, 1<<14): # 1<<12 is the threshold for releasing the GIL; important to test both code paths.
  b = '\x00' * i # Half of the input; fed to incremental update twice.
  h = Aquahash()
  h.update(b)
  h.update(b)
  d_inc = h.digest()
  d_one = aquahash(b + b)
  utest_val(d_inc, d_one)
