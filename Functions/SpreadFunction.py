class SpreadFunction:
    @staticmethod
    def generate_N_s_i(p_s, i):
        """Generates the value of N_s_i according to the original paper.
           N_s_i = 1/(2*p_s) + i/p_s

        Args:
            p_s (float): the probability of a specific symbol
            i (int): the index of the N_s value we want to get

        Returns:
            float: returns the N_s_i value
        """
        return 1/(2*p_s) + i/p_s

    @staticmethod
    def generate_N_s(p_s, l):
        """Generates N_s for a specific symbol with a given length.
           N_s = [N_s_0, N_s_1, ..., N_s

        Args:
            p_s (float): the probability of a specific symbol
            l (int): length of the N_s we want to generate

        Returns:
            list: list of N_s values
        """
        return [SpreadFunction.generate_N_s_i(p_s, i) for i in range(l)]

    @staticmethod
    def generate_N(prob, l):
        """Generates N for a given set of symbols, given their probabilities and the length of the N_s values.

        Args:
            prob (list): list of probabilities of the symbols
            l (int): length of the N_s values we want to generate

        Returns:
            list of lists: list of N_s values for each symbol
        """
        return [SpreadFunction.generate_N_s(p, l) for p in prob]

    @staticmethod
    def generate_N_iter(prob, l):
        """Generates a list of tuples with the N values and their indexes, sorted in ascending order.
           This is used to iterate over the N values in the order they should be used, as defined in the original paper.

        Args:
            prob (list): list of probabilities of the symbols
            l (int): length of the N_s values we want to generate

        Returns:
            list of lists: list of tuples with the N values and their indexes, sorted in ascending order with respect to the N values
        """
        N = SpreadFunction.generate_N(prob, l)
        N_iter = [[N[i][j], i] for i in range(len(N)) for j in range(len(N[i]))]
        N_iter.sort()
        return N_iter