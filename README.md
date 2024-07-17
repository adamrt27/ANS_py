# tANS_py
Tabled Asymmetric Numeral Systems Implementation in Python. This code is available as a package on PyPI [here](https://pypi.org/project/tANS-py/).

**Asymmetric Numeral Systems (ANS)** is a Entropy Coding compression technique created by Jarek Duda. This repository contains a Python implementation a version of the techinque that uses lookup table to store the state transitions (called **tANS**).

This implementation is based on the following resources: 

* The original paper [Asymmetric Numeral Systems](https://arxiv.org/abs/1311.2540) by Jarek Duda
* [Slides](https://ww2.ii.uj.edu.pl/~smieja/teaching/ti/3a.pdf) from a course taught by Duda (see slide 38)
* The following [medium article](https://medium.com/@bredelet/understanding-ans-coding-through-examples-d1bebfc7e076)
* This python implementation of [tANS](https://github.com/GarethCa/Py-tANS/tree/master?tab=readme-ov-file)
    * My implementation is very similar to this code, but is written to be more readable and fixes some of the small bugs in the original code
* This [blog post](https://kedartatwawadi.github.io/post--ANS/) explaining ANS

# Limitations

This implementation is not optimized for speed. It is meant to be a simple implementation that is easy to understand. 

The main limitation of this implementation is that `L`, the table size, must be a power of 2.

# Usage
See [example.ipynb](https://github.com/adamrt27/ANS_py/blob/main/example.ipynb) for examples of all the things discussed below.

## `Coder`

This is the main class that is used to encode and decode data. It is initialized with the table length (`L`), the list of symbols (`s_list`) and the list of frequencies (`L_s`). See implemention in [Coder.py](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/Coder.py).

Example Usage:

```python
# importing the Coder class as well as the Utils module, which helps with generating random data for testing
import tANS_py.Coder, tANS_py.Utils
import numpy as np

# Set up the alphabet
s = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"]
nbits = 5 # 5 bits per symbol as there are 26 symbols in the alphabet

# Run this multiple times to see how it performs on average
comp_ratios = []
for i in range(50):
    # Set up random frequencies
    # This specifically generates a list of len(s) numbers randomly chosen between 1 and 100
    freq = tANS_py.Utils.generate_random_list2(len(s), 100)

    # Create the Coder object
    c = tANS_py.Coder.Coder(sum(freq), s, freq, fast = False) # specifies fast = False to use slower, but more effecient spread function

    # Create a message
    # Specifically generates a random string using symbols from s with frequencies from freq
    msg = tANS_py.Utils.generate_random_string(s, freq)

    # Encode and decode the message and get the number of bits of the encoded message
    # Note: you must pass in message as a list of symbols
    out, bits = c.encode_decode(list(msg))

    # Check if the decoding worked
    if "".join(out) != msg:
        # If the decoding failed, print a message
        print("Coding failed")
    else:
        # If the decoding worked, save the compression ratio
        comp_ratios.append(len(msg) * nbits / bits)
    
print("Comp Ratio:", np.mean(comp_ratios))
```
Output:
```output
Comp Ratio: 1.359817660857606
```

## Submodules of `Coder`

### `DecodeTable`

This class is used to decode an encoded message. It is initialized with the table length (`L`), the list of symbols (`s_list`) and the list of frequencies (`L_s`). See implemention in [Decoder.py](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/Decoder.py)

### `Encoder`

This class is used to encode a message. It is initialized with the table length (`L`), the list of symbols (`s_list`) and the list of frequencies (`L_s`). See implemention in [Encoder.py](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/Encoder.py)

Example Usage of Submodules:

```python
# Testing code 
import tANS_py.Decoder
import tANS_py.Encoder

# Define the alphabet and the frequency of each symbol
s = ["A","B","C"]
freq = [6, 2, 24] # note that the sum of freq must be a power of 2 (in this case 32)

# Create the encoder and decoder
t = tANS_py.Decoder.DecodeTable(sum(freq), s, freq, fast = False)
g = tANS_py.Encoder.Encoder(sum(freq), s,freq,t.symbol_spread)

# Create message
msg = "CAACACCCCCCCCBCCCACCCACCCACCCBCC"
msg_temp = list(msg)

# Encode message
bit = g.encode(msg_temp)

# Decode message
out = t.decode(bit)
out.reverse() # reverse the list to get the original message, as the decoding function returns the message in reverse order
print("Coding worked:", "".join(out) == msg)
```

Output:
```output
Coding worked: True
```

## `SpreadFunction.py`

This module contains the spread function defined in [the original paper](https://arxiv.org/abs/1311.2540). It is automatically called by `Coder` and its submodules. See implemention in [SpreadFunction.py](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/SpreadFunction.py).

## `Utils.py`

Contains utility functions for generating random data or rescaling frequencies for the coder. See implemention in [Utils.py](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/Utils.py).

Functions:
```python
from tANS_py import Utils
# generates a list of length numbers that sum to a power of 2, with each number being randomly chosen between 1 and n
Utils.generate_random_list_pow2(length, n) 

# generates a list of length numbers that sum to a target sum, with each number being randomly chosen between 1 and n
Utils.generate_random_list_target(length, n, target_sum)

# rescales a list of numbers to sum to a power of 2 that is less than or equal to max sum
Utils.rescale_list_to_power_of_2(input_list, max_sum)

# generates a random string of length n using symbols from s with frequencies from freq
Utils.generate_random_string(s, freq)
```

# About

This implementation was created by Adam Taback as part of a research project at the University of Toronto, aiming to use ANS to compress neural network traces.

If you have any questions, reach out to me at adamrtaback@gmail.com.