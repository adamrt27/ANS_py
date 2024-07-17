import math
import pandas as pd

class Encoder:
    def __init__(self, L, s_list, L_s, spread):
        """Initializes the encoder

        Args:
            L (int): the sum of frequencies, also the length of the table
            s_list (list): list of symbols in the message
            L_s (list): list of frequencies of the symbols
            spread (list): spread of the symbols
        """
        if L > 0 and (L & (L - 1)) != 0:
            raise ValueError("L must be a power of 2")
        
        self.L = L
        self.s_list = s_list
        self.L_s = L_s

        # setup the table        
        self.table = [0 for i in range(L)]
        
        # spread symbols using the spread function
        self.symbol_spread = spread
        
        # setup other variables
        self.k = None
        self.nb = None
        self.start = None
        
        # setup the encoder
        self.setup()
        
    def display_table(self):
        """Generates a pandas dataframe to display the encoding table

        Returns:
            pd.DataFrame: a pandas dataframe with the encoding table
        """
        table = []
        for i in range(self.L):
            table.append(self.table[i])
            
        df = pd.DataFrame({"Next": table,"State": [i for i in range(self.L, self.L*2)]})
        df.set_index("State", inplace=True)
        return df.T
    
    @staticmethod
    def use_bits(num, n_bits):
        """Uses the first n_bits LSB from num, and returns the bits and the remaining number after removing the bits.

        Args:
            num (int): the number to extract bits from
            n_bits (int): number of bits to extract

        Returns:
            (list, int): returns (bits_list, num) where bits_list is the list of bits extracted, and num is the remaining 
            number after removing the bits
        """
        # get the first n_bits (LSB) from num
        if n_bits == 0:
            return [], num
        bits = num & ((1 << n_bits) - 1)
        num >>= n_bits
        # convert bits to a list of 0s and 1s
        bits_list = [int(bit) for bit in bin(bits)[2:].zfill(n_bits)]
        return bits_list, num
     
    def setup(self):
        """Sets up the encoding table, calculates k, nb, start, and next values

        Returns:
            list: returns the encoding table list
        """
        
        # calculate R = log2(L), the number of bits needed to represent L
        R = int(math.log2(self.L))
        # calculate r = R + 1
        r = R + 1
        
        # define k, which is the number of bits needed to represent each symbol
        self.k = [R - math.floor(math.log2(self.L_s[i])) for i in range(len(self.L_s))]
        
        # define nb
        self.nb = [(self.k[i] << r) - (self.L_s[i] << self.k[i]) for i in range(len(self.L_s))]
        
        # define start
        self.start = [0 for i in range(len(self.L_s))]
        for i in range(len(self.L_s)):
            self.start[i] = -self.L_s[i] + sum([self.L_s[j] for j in range(i)])
        
        # define next
        next = self.L_s.copy()
        
        # generate the encoding table
        for i in range(self.L):
            s = self.symbol_spread[i]
            self.table[self.start[s] + next[s]] = i + self.L
            next[s] += 1
            
        return self.table
    
    def encode_step(self, state, s):
        """A single step in the encoding process, encodes a single symbol

        Args:
            state (int): the current state of the encoding process, should be in the range [L,2L)
            s (void): the symbol to encode, can be of any type, but must be in the s_list

        Returns:
            (list, int): returns (bitstream, next state) where bitstream is the list of bits encoded, and next state is the next state
        """
        
        r = int(math.log2(self.L * 2))
        
        nbBits = (state + self.nb[s]) >> r
                
        bitstream, state = Encoder.use_bits(state, nbBits)
        
        state = self.table[self.start[s] + (state)]
        
        return bitstream, state
    
    def encode(self, data):
        """Encodes the entire data, and returns the bitstream

        Args:
            data (list): a list of symbols to encode, all symbols should be in the s_list

        Returns:
            (list, int): returns (bitstream, state) where bitstream is the list of bits encoded, and 
            state is the final state in [L,2L)
        """
        bitstream = []
        state = 0
        
        # initialize the state
        bits, state = self.encode_step(state, self.s_list.index(data[0]))
        
        # save original state
        state_orig = state
        
        for s in data:
            s_orig = s
            s = self.s_list.index(s)
            bits, state = self.encode_step(state, s)
            bitstream.extend(bits)
            
        # encode the final state
        bitstream.extend(Encoder.use_bits(state - self.L, int(math.log2(self.L)))[0])
        
        # encode the original state
        bitstream.extend(Encoder.use_bits(state_orig - self.L, int(math.log2(self.L)))[0])
            
        return bitstream