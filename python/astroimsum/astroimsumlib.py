import ctypes


class astroimsum_handle(ctypes.Structure):
    pass


class astroimsum_frame_handle(ctypes.Structure):
    pass


_astroimsum = ctypes.CDLL("astroimsum_c.so")
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
_astroimsum.astroimsum_add_frames.argtypes = (
    ctypes.POINTER(astroimsum_handle),
    ctypes.POINTER(astroimsum_frame_handle),
    ctypes.POINTER(astroimsum_frame_handle),
)

_astroimsum_handle = ctypes.POINTER(astroimsum_handle)
_astroimsum.astroimsum_init(ctypes.POINTER(_astroimsum_handle))


class frame:
    def __init__(self, name: str) -> None:
        self.handle = ctypes.POINTER(astroimsum_frame_handle)
        name_str = ctypes.c_char_p(name.encode())
        _astroimsum.astroimsum_create_frame(
            _astroimsum_handle, ctypes.POINTER(self.handle), name_str
        )

    def __del__(self) -> None:
        _astroimsum.astroimsum_destroy_frame(_astroimsum_handle, self.handle)


def sum_frames(a: frame, b: frame) -> frame:
    _astroimsum.astroimsum_add_frames(_astroimsum_handle, a.handle, b.handle)
    return a
