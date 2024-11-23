import ctypes
from . import _astroimsum_dir

class astroimsum_handle(ctypes.Structure):
    pass


class astroimsum_frame_handle(ctypes.Structure):
    pass

_astroimsum = ctypes.CDLL(_astroimsum_dir)
_astroimsum.astroimsum_init.argtypes = (
    ctypes.POINTER(ctypes.POINTER(astroimsum_handle)),
)
_astroimsum.astroimsum_destroy.argtypes = (ctypes.POINTER(astroimsum_handle),)
_astroimsum.astroimsum_create_frame.argtypes = (
    ctypes.POINTER(astroimsum_handle),
    ctypes.POINTER(ctypes.POINTER(astroimsum_frame_handle)),
    ctypes.c_char_p,
)
_astroimsum.astroimsum_access_frame.argtypes = (
    ctypes.POINTER(astroimsum_handle),
    ctypes.POINTER(astroimsum_frame_handle),
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.POINTER(ctypes.c_float)),
)
_astroimsum.astroimsum_destroy_frame.argtypes = (
    ctypes.POINTER(astroimsum_handle),
    ctypes.POINTER(astroimsum_frame_handle),
)
_astroimsum.astroimsum_write_frame.argtypes = (
    ctypes.POINTER(astroimsum_handle),
    ctypes.POINTER(astroimsum_frame_handle),
    ctypes.POINTER(ctypes.c_char),
)
_astroimsum.astroimsum_set_base_frame.argtypes = (
    ctypes.POINTER(astroimsum_handle),
    ctypes.POINTER(astroimsum_frame_handle),
)
_astroimsum.astroimsum_add_frame.argtypes = (
    ctypes.POINTER(astroimsum_handle),
    ctypes.POINTER(astroimsum_frame_handle),
)

class astroimsum:
    def __init__(self) -> None:
        self.handle = ctypes.POINTER(astroimsum_handle)()
        _astroimsum.astroimsum_init(ctypes.byref(self.handle))

    def load_frame(self, name: str):
        return frame(self, name)

    def set_base_frame(self, frame):
        _astroimsum.astroimsum_set_base_frame(self.handle, frame.handle)

    def add_frame(self, frame):
        _astroimsum.astroimsum_add_frame(self.handle, frame.handle)

    def __del__(self) -> None:
        _astroimsum.astroimsum_destroy(self.handle)


class frame:
    def __init__(self, lib: astroimsum, name: str) -> None:
        self.handle = ctypes.POINTER(astroimsum_frame_handle)()
        self.lib = lib

        name_str = ctypes.c_char_p(name.encode())
        _astroimsum.astroimsum_create_frame(
            self.lib.handle, ctypes.byref(self.handle), name_str
        )

    def write(self, name: str) -> None:
        name_str = ctypes.c_char_p(name.encode())
        _astroimsum.astroimsum_write_frame(self.lib.handle, self.handle, name_str)

    def __del__(self) -> None:
        _astroimsum.astroimsum_destroy_frame(self.lib.handle, self.handle)
