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

Example Usage:

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

```output
Hello World
String Works: True
['H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd']
List Works: True
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