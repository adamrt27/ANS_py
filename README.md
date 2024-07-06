# tANS
Tabled Asymmetric Numeral Systems Implementation in Python. 

**Asymmetric Numeral Systems (ANS)** is a Entropy Coding compression technique created by Jarek Duda. This repository contains a Python implementation a version of the techinque that uses lookup table to store the state transitions (called **tANS**). 

This implementation is based on the following resources: 

* The original paper [Asymmetric Numeral Systems](https://arxiv.org/abs/1311.2540) by Jarek Duda
* [Slides](https://ww2.ii.uj.edu.pl/~smieja/teaching/ti/3a.pdf) from a course taught by Duda (see slide 38)
* The following [medium article](https://medium.com/@bredelet/understanding-ans-coding-through-examples-d1bebfc7e076)
* This python implementation of [tANS](https://github.com/GarethCa/Py-tANS/tree/master?tab=readme-ov-file)
    * My implementation is very similar to this code, but is written to be more readable and fixes some of the small bugs in the original code
## Main Code

The main code can be found in the [`tANS.ipynb`](https://github.com/adamrt27/tANS/blob/main/tANS.ipynb).

## Next Steps

The aim of this project is to apply **tANS** to Neural Compression, similar to the work done in [this paper](https://arxiv.org/abs/2201.08830), which was for Arithmetic Coding. Arithmetic Coding is another Entropy Coding algorithm that achieves similar compression to ANS, but is more computationally expensive.