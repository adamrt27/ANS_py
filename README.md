# tANS
Tabled Asymmetric Numeral Systems Implementation in Python. 

**Asymmetric Numeral Systems (ANS)** is a Entropy Coding compression technique created by Jarek Duda. This repository contains a Python implementation a version of the techinque that uses lookup table to store the state transitions (called **tANS**). It also contains code testing **tANS** on neural network traces, using 2 different methods of encoding the data (APack vs 256).

This implementation is based on the following resources: 

* The original paper [Asymmetric Numeral Systems](https://arxiv.org/abs/1311.2540) by Jarek Duda
* [Slides](https://ww2.ii.uj.edu.pl/~smieja/teaching/ti/3a.pdf) from a course taught by Duda (see slide 38)
* The following [medium article](https://medium.com/@bredelet/understanding-ans-coding-through-examples-d1bebfc7e076)
* This python implementation of [tANS](https://github.com/GarethCa/Py-tANS/tree/master?tab=readme-ov-file)
    * My implementation is very similar to this code, but is written to be more readable and fixes some of the small bugs in the original code
* This [blog post](https://kedartatwawadi.github.io/post--ANS/) explaining ANS

## tANS

The main code can be found in the [`tANS.ipynb`](https://github.com/adamrt27/tANS/blob/main/tANS.ipynb). This notebook contains the implementation of the tANS algorithm, as well as some examples of how to use it. It also contains some benchmarking with random data.

A package version of [`tANS.ipynb`](https://github.com/adamrt27/tANS/blob/main/tANS.ipynb) can be found in [`Functions`](https://github.com/adamrt27/tANS/blob/main/Functions).

## tANS for Neural Compression

### Generating Traces

Traces are generated in [`pytorch_trace.ipynb`](https://github.com/adamrt27/tANS/blob/main/pytorch_trace.ipynb). 

They are then processed using [`atalanta`](https://github.com/adamrt27/tANS/blob/main/alatanta), which was taken from the [atalanta repository](https://github.com/moshovos/Atalanta/tree/main). This is required for compression using the APack method.

### APack

As outlined in the [APack paper](https://arxiv.org/abs/2201.08830), we seperate traces into `(symbol, offset)` pairs for compression. This can be found in [`tANS_apack.ipynb`](https://github.com/adamrt27/tANS/blob/main/tANS_apack.ipynb).

### 256

As a baseline to test against, we also just use the traces as they are, using 256 symbols with 8-bit quantized models. This can be found in [`tANS_256.ipynb`](https://github.com/adamrt27/tANS/blob/main/tANS_256.ipynb).