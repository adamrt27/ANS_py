import ctypes
import os

# Set up the path to the shared library
lib_path = os.path.join(os.path.dirname(__file__), 'libtANS.so')

# Load the shared library
lib = ctypes.CDLL(lib_path)

# Define Structs
class encodeTable(ctypes.Structure):
    _fields_ = [
        ("L", ctypes.c_int),
        ("s_list", ctypes.POINTER(ctypes.c_uint8)),
        ("L_s", ctypes.POINTER(ctypes.c_uint8)),
        ("table", ctypes.POINTER(ctypes.c_int)),
        ("n_sym", ctypes.c_uint8),
        ("k", ctypes.POINTER(ctypes.c_uint8)),
        ("nb", ctypes.POINTER(ctypes.c_int)),
        ("start", ctypes.POINTER(ctypes.c_int))
    ]

class encoder(ctypes.Structure):
    _fields_ = [ 
        ("state", ctypes.c_int),
        ("bitstream", ctypes.POINTER(ctypes.c_uint8)),
        ("bitstream_capacity", ctypes.c_long),
        ("l_bitstream", ctypes.c_long),
        ("msg", ctypes.POINTER(ctypes.c_uint8)),
        ("l_msg", ctypes.c_int),
        ("ind_msg", ctypes.c_int)
    ]

class decodeTableColumn(ctypes.Structure):
    _fields_ = [
        ("x", ctypes.c_uint8),
        ("sym", ctypes.c_uint8),
        ("nb", ctypes.c_uint8),
        ("newX", ctypes.c_uint8)
    ]

class decodeTable(ctypes.Structure):
    _fields_ = [
        ("table", ctypes.POINTER(decodeTableColumn)),
        ("L", ctypes.c_int),
        ("s_list", ctypes.POINTER(ctypes.c_uint8)),
        ("L_s", ctypes.POINTER(ctypes.c_uint8)),
        ("n_sym", ctypes.c_uint8)
    ]

class decoder(ctypes.Structure):
    _fields_ = [ 
        ("state", ctypes.c_uint8),
        ("bitstream", ctypes.POINTER(ctypes.c_uint8)),
        ("l_bitstream", ctypes.c_long),
        ("msg", ctypes.POINTER(ctypes.c_uint8)),
        ("l_msg", ctypes.c_int)
    ]

class coder(ctypes.Structure):    
    _fields_ = [ 
        ("L", ctypes.c_int),
        ("s_list", ctypes.POINTER(ctypes.c_uint8)),
        ("L_s", ctypes.POINTER(ctypes.c_uint8)),
        ("n_sym", ctypes.c_uint8),
        ("e", encoder),
        ("e_table", encodeTable),
        ("d", decoder),
        ("d_table", decodeTable)
    ]

# Define Functions

def initCoder(L: int, s_list: list, L_s: list):
    func = lib.initCoder
    func.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint8]
    func.restype = ctypes.POINTER(coder)
    
    s_list_arr = (ctypes.c_uint8 * len(s_list))(*s_list)
    L_s_arr = (ctypes.c_uint8 * len(L_s))(*L_s)
    
    return func(L, s_list_arr, L_s_arr, len(s_list))

def encodeCoder(c: ctypes.POINTER(coder), msg: list):
    func = lib.encodeCoder
    func.argtypes = [ctypes.POINTER(coder), ctypes.POINTER(ctypes.c_uint8), ctypes.c_int]
    func.restype = None
    
    msg_arr = (ctypes.c_uint8 * len(msg))(*msg)
    
    return func(c, msg_arr, len(msg))

def decodeCoder(c: ctypes.POINTER(coder)):
    func = lib.decodeCoder
    func.argtypes = [ctypes.POINTER(coder)]
    func.restype = None
    
    return func(c)

def encodeDecode(c: ctypes.POINTER(coder), msg: list):
    func = lib.encodeDecode
    func.argtypes = [ctypes.POINTER(coder), ctypes.POINTER(ctypes.c_uint8), ctypes.c_int]
    func.restype = ctypes.c_int
    
    msg_arr = (ctypes.c_uint8 * len(msg))(*msg)
    
    return func(c, msg_arr, len(msg))

def encodeDecodeWithInit(L: int, s_list: list, L_s: list, msg: list):
    func = lib.encodeDecodeWithInit
    func.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint8, ctypes.POINTER(ctypes.c_uint8), ctypes.c_int]
    func.restype = ctypes.c_int
    
    s_list_arr = (ctypes.c_uint8 * len(s_list))(*s_list)
    L_s_arr = (ctypes.c_uint8 * len(L_s))(*L_s)
    msg_arr = (ctypes.c_uint8 * len(msg))(*msg)
    
    return func(L, s_list_arr, L_s_arr, len(s_list), msg_arr, len(msg))

if __name__ == '__main__':
    L = 256
    s_list = [i for i in range(16)]
    L_s = [19, 4, 8, 33, 8, 18, 4, 10, 19, 8, 30, 4, 8, 38, 26, 19]
    msg = [0,3,2,3,2,1,2,3,4,7,3,1,2,3,4,5,6,7, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0]
    
    res = encodeDecodeWithInit(L, s_list, L_s, msg)
    
    print(f"Compression ratio: {len(msg) * 4 / res}")
    
    # track time taken
    import time
    
    start = time.time()
    
    for i in range(1000):
        encodeDecodeWithInit(L, s_list, L_s, msg)
        
    end = time.time()
    
    print(f"Time taken (uninitialized): {(end - start) * 1e3} miliseconds")
    
    c = initCoder(L, s_list, L_s)
    
    start1 = time.time()
    
    for i in range(1000):
        encodeDecode(c, msg)
        
    end1 = time.time()
    
    print(f"Time taken (preinitialized): {(end1 - start1) * 1e3} miliseconds")

    print("It is {}x faster to preinitialize".format(round((end - start) / (end1 - start1), 2)))