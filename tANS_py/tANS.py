# this file contains functions for encoding and decoding using tANS, without having to make the objects
from . import Utils, Coder
import numpy as np

def encode(data, L = 1024, fast = False, dtype = 'np'):
    # get symbols
    s = list(set(data))
    
    # get frequencies
    if dtype != "np":
        freq = [data.count(i) for i in s]
    else:
        freq = np.sum(data == i for i in s)
    
    # rescale frequencies to power of 2
    freq = Utils.rescale_list_to_power_of_2(freq, L, max_sum_var = "L")
    
    # define the coder
    c = Coder.Coder(sum(freq), s, freq, fast = fast)
    
    # encode the data and return the bitstream and the coder (needed to decode)
    return c.encode(data), c

def decode(bitstream, coder, dtype = "np"):
    # decode the bitstream using the coder
    if not isinstance(coder, Coder.Coder):
        raise ValueError("coder must be an instance of Coder")
    if dtype == "np":
        return np.asarray(coder.decode(bitstream))
    return coder.decode(bitstream)

def encode_decode_test(data, L = 1024, fast=False, dtype = "np"):
    
    # get symbols
    s = list(set(data))
    
    if dtype != "np":
        
        # get frequencies
        freq = [data.count(i) for i in s]
        
    else:
        freq = [np.sum(data == i) for i in s]
        
    # rescale frequencies to power of 2
    freq = Utils.rescale_list_to_power_of_2(freq, L, max_sum_var = "L")
    
    # define the coder
    c = Coder.Coder(sum(freq), s, freq, fast = fast)
    
    # encode the data
    bitstream = c.encode(data)
    
    # decode the data
    out = c.decode(bitstream)
    
    # calculate the compression ratio
    if dtype == "np":
        orig_size = data.nbytes
    else:
        orig_size = Coder.Coder.calculate_bits(data)
    
    comp_ratio = orig_size / (len(bitstream) / 8)
    
    if dtype == "np":
        out = np.asarray(out)
    elif dtype == "list":
        out = list(out)
    else:
        out = "".join([str(i) for i in out])
    
    # return the bitstream and the decoded data and the compression ratio
    return bitstream, out, comp_ratio        
