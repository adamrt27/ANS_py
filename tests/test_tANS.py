from tANS_py import tANS
import tANS_py.Utils

def test_tANS():
    # Testing with different messages to see how well it compresses
    freq = [tANS_py.Utils.generate_random_list_target(26, 100, 1024) for i in range(100)]
    msg = [tANS_py.Utils.generate_random_string("ABCDEFGHIJKLMNOPQRSTUVWXYZ", freq[i]) for i in range(100)]
    for m in msg:    
        res = tANS.encode_decode_test(m)
        assert m == res[1], "Error in encoding and decoding"