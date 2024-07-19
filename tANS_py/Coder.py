from . import Decoder, Encoder

class Coder:
    def __init__(self, L, s_list, L_s, fast = False):
        """Initializes the coder

        Args:
            L (int): the sum of frequencies, also the length of the table
            s_list (list): list of symbols in the message
            L_s (list): list of frequencies of the symbols
            fast (bool, optional): whether to use the fast spread function. Defaults to False.
        """
        if L > 0 and (L & (L - 1)) != 0:
            raise ValueError("L must be a power of 2")
        
        self.s_list = s_list
        self.L_s = L_s
        self.L = L
                        
        # initialize the decoding table
        self.decoding_table = Decoder.DecodeTable(self.L, s_list, L_s, fast= fast)
        
        # spread symbols using the spread function
        self.spread = self.decoding_table.symbol_spread
        
        # initialize the encoder
        self.encoder = Encoder.Encoder(self.L, s_list, L_s, self.spread)
        
    def encode(self, data) -> list:
        """Encodes the data using the encoder

        Args:
            data (list): a list of symbols to encode

        Returns:
            list: returns the bitstream
        """
        return self.encoder.encode(data)
    
    def decode(self, bitstream):
        """Decodes the bitstream using the decoding table

        Args:
            bitstream (list): the bitstream to decode

        Returns:
            list: returns the decoded symbols
        """
        # note we reverse the decoded symbols since the bitstream is decoded in opposite order
        res = self.decoding_table.decode(bitstream)
        res.reverse()
        return res
    
    @staticmethod
    def calculate_bits(number):
        if number == 0:
            return 1  # Special case for zero
        num_bits = 0
        while number:
            number >>= 1  # Right shift by 1 bit
            num_bits += 1
        return num_bits
    
    def encode_decode(self, data, verbose = False):
        """Encodes and decodes the data, and returns the decoded data

        Args:
            data (list): a list of symbols to encode and decode

        Returns:
            list: returns the decoded symbols
            int: returns the number of bits in the bitstream
        """
        
        bitstream = self.encode(data)
        res = self.decode(bitstream)
        # if there is an error 
        if res != data:
            print("Error in encoding and decoding")
            
        # compute how many bits saved
        orig_bits = len(data) * Coder.calculate_bits(len(self.s_list))
        comp_bits = len(bitstream)
        
        if verbose:
            print("Original bits: %d, Compressed bits: %d, Saved: %d" % (orig_bits, comp_bits, orig_bits - comp_bits))
        
        return res, comp_bits
    
    def encode_decode_string(self, data):
        """Encodes and decodes the string data, and returns the decoded data

        Args:
            data (str): a string to encode and decode

        Returns:
            list: returns the decoded symbols
            int: returns the number of bits in the bitstream
        """
        data_t = list(data)
        res = self.encode_decode(data_t)
        return "".join(res[0]), res[1]
    
    def encode_string(self, data):
        """Encodes the string data, and returns the bitstream

        Args:
            data (str): a string to encode

        Returns:
            list: returns the bitstream
        """
        data_t = list(data)
        return self.encode(data_t)
    
    def decode_string(self, data):
        """Decodes the bitstream, and returns the decoded string

        Args:
            data (list): the bitstream to decode

        Returns:
            str: returns the decoded string
        """
        res = self.decode(data)
        return "".join(res)