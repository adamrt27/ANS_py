from tANS_py import tANS
import tANS_py.Utils
import numpy as np

def test_tANS_string():
    # Testing with different messages to see how well it compresses
    freq = [tANS_py.Utils.generate_random_list_target(26, 100, 1024) for i in range(100)]
    msg = [tANS_py.Utils.generate_random_string("ABCDEFGHIJKLMNOPQRSTUVWXYZ", freq[i]) for i in range(100)]
    for m in msg:    
        res = tANS.encode_decode_test(m, dtype = "str")
        assert m == res[1], "Error in encoding and decoding"
        
def test_tANS_list():
    # Testing with different messages to see how well it compresses
    freq = [tANS_py.Utils.generate_random_list_target(26, 100, 1024) for i in range(100)]
    msg = [tANS_py.Utils.generate_random_string("ABCDEFGHIJKLMNOPQRSTUVWXYZ", freq[i]) for i in range(100)]
    for m in msg:    
        res = tANS.encode_decode_test(list(m), dtype = "list")
        assert list(m) == res[1], "Error in encoding and decoding"
        
def test_tANS_numpy():
    # Testing with different messages to see how well it compresses
    msg = [np.random.randint(0, 256, 200, dtype=np.uint8) for i in range(100)]
    for m in msg:    
        res = tANS.encode_decode_test(m, dtype = "np")
        assert np.array_equal(m, res[1]), "Error in encoding and decoding"