# Dedicated to the public domain under CC0: https://creativecommons.org/publicdomain/zero/1.0/.

from typing import ByteString, Union

_Data = Union[str,ByteString]

class Aquahash():
  def __new__(cls, salt:_Data=...) -> 'Aquahash': ...

  def update(self, data:_Data) -> None: ...

  def digest(self) -> bytes: ...

def aquahash(self, data:_Data, salt:_Data=...) -> bytes: ...
