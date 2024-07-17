import numpy as np 
import math
import pandas as pd
from . import SpreadFunction

class DecodeTableColumn:
    def __init__(self, x, sym, nbBits, newX):
        """Initializes the decoding table column

        Args:
            x (int): the state of the column
            sym (void): the symbol for that column, can be of any type
            nbBits (int): number of bits to read from bitstream
            newX (int): new state after reading nbBits
        """
        self.x = x
        self.sym = sym
        self.nbBits = nbBits
        self.newX = newX
        
    def __str__(self):
        return "State: %d, Symbol: %s, nbBits: %d, newX: %d" % (self.x, self.sym, self.nbBits, self.newX)
    
    def __list__(self):
        return [self.x, self.sym, self.nbBits, self.newX]
        
class DecodeTable:
    def __init__(self, L, s_list, L_s, fast = False):
        """Initializes the decoding table

        Args:
            L (int): the length of the table, equal to the culminative frequency of the symbols
            s_list (list): list of symbols in the message, for example (A,B,C)
            L_s (list): the frequency of each symbol in the message, for example (2,3,4)
        """
        if L > 0 and (L & (L - 1)) != 0:
            raise ValueError("L must be a power of 2")
        
        self.L = L
        self.s_list  = s_list
        self.L_s = L_s
        
        # initialize an empty table, with L empty columns
        self.table = [DecodeTableColumn(0,0,0,0) for i in range(L)]
        
        # spread symbols using the spread function
        self.symbol_spread = None
        if fast:
            self.fast_spread(X = 0, step = int((5/8) * self.L + 3))
        else:
            self.spread_function()
        
        # generate the decoding table
        self.get_decoding_table()
        
        
    def __str__(self):
        return "Decoding table with length: %d, symbols: %s, frequencies: %s" % (self.L, self.s_list, self.L_s)
    
    def display_table(self):
        """Generates a pandas dataframe to display the decoding table

        Returns:
            pd.DataFrame: a pandas dataframe with the decoding table
        """
        
        # make a list of lists for the table
        table = []
        for i in range(self.L):
            table.append(self.table[i].__list__())
            
        # convert to pandas dataframe
        df = pd.DataFrame(table, columns=["State", "Symbol", "nbBits", "newX"])
        df.set_index("State", inplace=True)
        return df.T

    def spread_function(self):
        """Uses the heuristic spread function to spread the symbols in the decoding table

        Returns:
            list: a list of symbols spread according to the spread function
        """
        
        symbol = [0 for i in range(self.L)]
        spread = SpreadFunction.SpreadFunction.generate_N_iter([i/self.L for i in self.L_s], self.L)
        
        for X, e in enumerate(spread):
            if X < self.L:
                symbol[X] = e[1]
            else:
                break
                
        # update in place
        self.symbol_spread = symbol
        return symbol
    
    def fast_spread(self, X, step):
        symbol = [0 for i in range(self.L)]
        for s in range(len(self.s_list)):
            for i in range(1, self.L_s[s]+1):
                symbol[X] = s
                X = (X + step) % self.L
                
        self.symbol_spread = symbol
        return symbol
    
    @staticmethod
    def read_bit(bitstream, nbits):
        """Read nbits from bitstream (a list of bits), and return the bits read and the remaining bitstream.
           Reads from right to left.

        Args:
            bitstream (list): the binary bitstream to read from (list of 0s and 1s)
            nbits (int): number of bits to read

        Returns:
            (list, list): returns (bits, bitstream) where bits is the list of bits read, and bitstream is the remaining bitstream
        """
        # check if nbits == 0
        if nbits == 0:
            return [], bitstream
        # read nbits from bitstream
        bits = bitstream[-nbits:]  # get the last nbits
        bitstream = bitstream[:-nbits]  # remove the last nbits from the bitstream
        return bits, bitstream

    def get_decoding_table(self):
        """Generates the decoding table, and returns. Also updates table in place.

        Returns:
            list of lists: returns the decoding table as a list of lists
        """
        
        # next holds the next x_tmp for each symbol and is incremented after each use
        # is initialized to the frequency of each symbol
        next = [e for e in self.L_s] 
        
        # calculate R, the number of bits needed to represent L
        R = math.log2(self.L) # L = 2^R
        
        # generate the decoding table
        for X in range(self.L):
            # set the x value
            self.table[X].x = X
            
            # set the symbol, given by the spread function
            self.table[X].sym = self.symbol_spread[X]

            # calculate x_tmp
            x_tmp = next[self.symbol_spread[X]]
            
            # increment next at the index of the symbol
            next[self.symbol_spread[X]] += 1
            
            # calculate the number of bits needed to represent x_tmp
            self.table[X].nbBits = int(R - math.floor(math.log2(x_tmp)))
            
            # calculate the new x value
            self.table[X].newX = (x_tmp << self.table[X].nbBits) - self.L
        
        return self.table
    
    def decode_step(self, state, bitstream):
        """A single step in the decoding process, decodes a single symbol from the bitstream

        Args:
            state (int): the current state of the decoding process, in the range [0, L)
            bitstream (list): A bitstream to decode, should be a list of 0s and 1s

        Returns:
            (void, int, list): returns (decoded symbol, next state, remaining bitstream)
        """
        
        # get the symbol
        s_decode = self.table[state].sym
        
        # read into bits from the bitstream, and update the stream back into bitstream
        # bits is the bits read from the bitstream
        # bitstream is the remaining bitstream after reading bits
        bits, bitstream = DecodeTable.read_bit(bitstream, self.table[state].nbBits)
        if len(bits) == 0:
            bits = 0
        else:
            bits = int("".join(str(i) for i in bits), 2) # convert bits to int
        
        # calculate the next state
        next_state = self.table[state].newX + bits
        
        return s_decode, next_state, bitstream
    
    def decode(self, bitstream):
        """decode the entire bitstream, given the initial state
           NOTE: the bitstream is decoded right to left

        Args:
            bitstream (list): the bitstream to decode

        Returns:
            list: list of decoded symbols, in the order they were decoded
        """
        
        # initialize empty list to store decoded symbols
        decoded = []
        
        # get the original state, by reading log2(L) bits from the bitstream
        state_orig, bitstream = self.read_bit(bitstream, int(math.log2(self.L)))
        state_orig = int("".join(str(i) for i in state_orig), 2) # convert bits to int
        
        # get the initial state, by reading log2(L) bits from the bitstream
        state, bitstream = self.read_bit(bitstream, int(math.log2(self.L)))
        state = int("".join(str(i) for i in state), 2) # convert bits to int
                
        # iterate over the bitstream, decoding each symbol
        # stops when the bitstream is empty and the state is equal to the original state
        # note: we do state_orig - self.L because the encoding table is from L to 2L
        # and we decoding is from 0 to L
        while len(bitstream) > 0 or state != (state_orig):
            # decode a single symbol
            s_decode, state, bitstream = self.decode_step(state, bitstream)
            # append the decoded symbol to the list
            decoded.append(self.s_list[s_decode])
            #print(state)
            
        # return the list of decoded symbols
        return decoded