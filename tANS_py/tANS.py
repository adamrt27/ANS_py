# this file contains functions for encoding and decoding using tANS, without having to make the objects
from . import Utils, Coder

def encode(data, L = 1024, fast = False):
    # get symbols
    s = list(set(data))
    
    # get frequencies
    freq = [data.count(i) for i in s]
    
    # rescale frequencies to power of 2
    freq = Utils.rescale_list_to_power_of_2(freq, L, max_sum_var = "L")
    
    # define the coder
    c = Coder.Coder(sum(freq), s, freq, fast = fast)
    
    # encode the data and return the bitstream and the coder (needed to decode)
    return c.encode(data), c

def decode(bitstream, coder):
    # decode the bitstream using the coder
    if not isinstance(coder, Coder.Coder):
        raise ValueError("coder must be an instance of Coder")
    return coder.decode(bitstream)

def encode_decode_test(data, L = 1024, fast=False):
    # get symbols
    s = list(set(data))
    
    # get frequencies
    freq = [data.count(i) for i in s]
    
    # rescale frequencies to power of 2
    freq = Utils.rescale_list_to_power_of_2(freq, L, max_sum_var = "L")
    
    # define the coder
    c = Coder.Coder(sum(freq), s, freq, fast = fast)
    
    # encode the data
    bitstream = c.encode(data)
    
    # decode the data
    out = c.decode(bitstream)
    
    # calculate the compression ratio
    comp_ratio = (len(data) * Coder.Coder.calculate_bits(len(s))) / len(bitstream) 
    
    # return the bitstream and the decoded data and the compression ratio
    return bitstream, "".join(out), comp_ratio