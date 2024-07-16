# tANS_py
Tabled Asymmetric Numeral Systems Implementation in Python. 

**Asymmetric Numeral Systems (ANS)** is a Entropy Coding compression technique created by Jarek Duda. This repository contains a Python implementation a version of the techinque that uses lookup table to store the state transitions (called **tANS**). It also contains code testing **tANS** on neural network traces, using 2 different methods of encoding the data (APack vs 256).

This implementation is based on the following resources: 

* The original paper [Asymmetric Numeral Systems](https://arxiv.org/abs/1311.2540) by Jarek Duda
* [Slides](https://ww2.ii.uj.edu.pl/~smieja/teaching/ti/3a.pdf) from a course taught by Duda (see slide 38)
* The following [medium article](https://medium.com/@bredelet/understanding-ans-coding-through-examples-d1bebfc7e076)
* This python implementation of [tANS](https://github.com/GarethCa/Py-tANS/tree/master?tab=readme-ov-file)
    * My implementation is very similar to this code, but is written to be more readable and fixes some of the small bugs in the original code
* This [blog post](https://kedartatwawadi.github.io/post--ANS/) explaining ANS

# Usage

## `Coder`

This is the main class that is used to encode and decode data. It is initialized with the table length (`L`), the list of symbols (`s_list`) and the list of frequencies (`L_s`). 

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

## 'Decoder'

