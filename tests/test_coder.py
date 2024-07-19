# Start by just testing Coder
import tANS_py.Coder, tANS_py.Utils
import pytest

def test_checkCoder():

    # Set up the alphabet
    s = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"]
    nbits = 5 # 5 bits per symbol as there are 26 symbols in the alphabet

    # Run this multiple times to see how it performs on average
    comp_ratios = []
    for i in range(50):
        # Set up random frequencies
        # This specifically generates a list of len(s) numbers randomly chosen between 1 and 100
        freq = tANS_py.Utils.generate_random_list_target(len(s), 100, 1024)

        # Create the Coder object
        c = tANS_py.Coder.Coder(sum(freq), s, freq, fast = False) # specifies fast = False to use slower, but more effecient spread function

        # Create a message
        # Specifically generates a random string using symbols from s with frequencies from freq
        msg = tANS_py.Utils.generate_random_string(s, freq)

        # Encode and decode the message and get the number of bits of the encoded message
        # Note: you must pass in message as a list of symbols
        out, bits = c.encode_decode(list(msg))

        # Check if the decoding worked
        assert "".join(out) == msg, "Coding failed"
        
def test_checkCoderL_err():
    
    s = ["1","2","3","4","5"]
    freq = [1,2,3,4,5] # sum is 15 which is not a power of 2
    
    with pytest.raises(ValueError):
        c = tANS_py.Coder.Coder(sum(freq), s, freq, fast = False)
        
        msg = tANS_py.Utils.generate_random_string(s, freq)
        out, bits = c.encode_decode(list(msg))
