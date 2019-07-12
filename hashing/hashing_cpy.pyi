# Dedicated to the public domain under CC0: https://creativecommons.org/publicdomain/zero/1.0/.

'''
Hashing C++ extension.
'''


from typing import ByteString, Union

_Data = Union[str,ByteString]


class Aquahash():
  'Aquahash.'

  @classmethod
  def __new__(cls, salt:_Data=...) -> 'Aquahash':
    'Create a new AquaHash hashing object with an optional salt.'

  def update(self, data:_Data) -> None:
    'Update the hash object with bytes from `data`.'

  def digest(self) -> bytes:
    'Produce a digest of the bytes hashed so far.'

  def block_size(self) -> int:
    'The size in bytes of the AquaHash algorithm blocks.'

  def digest_size(self) -> int:
    'The size in bytes of the AquaHash algorithm digest.'


def aquahash(data:_Data, salt:_Data=...) -> bytes:
  'Hash a `str` or bytes-like value using the AquaHash function.'
