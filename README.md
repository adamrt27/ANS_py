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

## Main Workflow

The main workflow of this implementation can be found in [tANS.py](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/tANS.py). This file contains functions which encode and decode messages using the `Coder` class. The benefit of using this file is that it allows for a more streamlined workflow, removing the need for the user to interact with the `Coder` class directly and define symbols and their frequencies.

### Encoding and Decoding

```python
# Importing the tANS module and testing it with a simple message
from tANS_py import tANS

# takes a string or list of symbols as input
msg = "Hello World! This is a test message to see how well the tANS algorithm works. It should be able to compress this message quite well, as it has a lot of repeated characters. Let's see how well it does!"
msg_list = list(msg)

# Using the tANS module to encode and decode the message as a string

# L determines the table size, the larger the table, the more efficient the encoding (default is 1024)
# fast determines whether to use the fast or slow version of the spread, slow is more efficient but slower (default is False)
bits, c = tANS.encode(msg, L = 128, fast= False)  

# Note: the output of encode, c, must be passed to decode, as it contains the data necessary to decode the message   
res = tANS.decode(bits, c)              

# returns a list of characters, so we need to join them into a string
res = "".join(res) 

print(msg[:11])
print("String Works:",res == msg)

# Using the tANS module to encode and decode the message as a list
bits, c = tANS.encode(msg_list)
res = tANS.decode(bits, c)

print(msg_list[:11])
print("List Works:",res == msg_list)
```
Output:
```output
Hello World
String Works: True
['H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd']
List Works: True
```

### `encode_decode_test` for Testing
If you wish to test how well ANS works on a particular dataset, you can use the `encode_decode` function in `tANS.py`. This function takes a message, encodes and decodes it, and returns the **compression ratio**, along with the encoded message and decoded result. **Compression ratio** = (size of original message) / (size of compressed message), thus a higher compression ratio is better.

This can be useful if you are comparing ANS to other compression algorithms, or if you are trying to determine the best table size for your data.

```python
# Can also test the encode_decode function to test the compression ratio, good for evaluating the algorithm
msg = "Hello World! This is a test message to see how well the tANS algorithm works. It should be able to compress this message quite well, as it has a lot of repeated characters. Let's see how well it does!"
msg2 = "Hello World! This message will compress worse"

res = tANS.encode_decode_test(msg, L = 1024, fast = False)
res2 = tANS.encode_decode_test(msg2, L = 1024, fast = False)

print("Message 1")
print("\tBitstream","".join([str(i) for i in res[0]]))
print("\tWorking:",res[1] == msg)
print("\tComp Ratio:",res[2])

print("Message 2")
print("\tBitstream","".join([str(i) for i in res2[0]]))
print("\tWorking:",res2[1] == msg2)
print("\tComp Ratio:",res2[2])
```
Output:
```output
Message 1
	Bitstream 1100011110111001001101110000000001010101101000010111111010110100100110001101101001101010110001001110110011000000001111000001101111110101110000100001110000001100100101101110111100000011110100101100010011010101011110110001100101110010110111100010110001110110110101010101101001000100001001111001000101110101000101000000100110010101110111010101111110101000000100110010101101111000010010100001111000101001111111110100010010000100001100001100010010111011100010111010000010100011011110110111101101011100001101001000110011010100001000000011011011011111101000111111110101101101101100101101111000110001100000101011101100101100110001011001111001100011101010101101001101111001000010011110100110010100011011100011011011110110110001100100111001110000111010110110001011010011011010111010101011101101001100101011011101010001000101111000100100111000001100011
	Working: True
	Comp Ratio: 1.426872770511296
Message 2
	Bitstream 10100111001110110111110111000101011100000111110001010110001001001101100100110010010100010001011011001010011001101000111100011101001000110010110000000000010011011000101000111000100001110000010100
	Working: True
	Comp Ratio: 1.1597938144329898

```

## Submodules

tANS_py contains several submodules that can be used to interact with the tANS algorithm. These submodules are:

* [Coder](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/Coder.py) - The main class that is used to encode and decode messages
* [Decoder](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/Decoder.py) - Contains the functions necessary to decode a message
* [Encoder](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/Encoder.py) - Contains the functions necessary to encode a message
* [SpreadFunction](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/SpreadFunction.py) - Contains the functions necessary to build the encoder and decoder tables
* [Utils](https://github.com/adamrt27/ANS_py/blob/main/tANS_py/Utils.py) - Contains utility functions that are mainly used to generate frequency counts given a probability distribution

You can find examples of how to use these submodules in [example.ipynb](https://github.com/adamrt27/ANS_py/blob/main/example.ipynb).

# About

This implementation was created by Adam Taback as part of a research project at the University of Toronto, aiming to use ANS to compress neural network traces.

If you have any questions, reach out to me at adamrtaback@gmail.com.