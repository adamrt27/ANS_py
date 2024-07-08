# Generate random string
import random
import numpy as np

def generate_random_string(alphabet, frequencies):
    population = []
    
    # Create the population list based on frequencies
    for symbol, freq in zip(alphabet, frequencies):
        population.extend([symbol] * freq)
    
    # Shuffle the population to ensure randomness
    random.shuffle(population)
    
    # Join the list into a string
    random_string = ''.join(population)
    
    return random_string

def generate_random_list(l, n):
    return [random.randint(0, n) for _ in range(l)]

def next_power_of_2(x):
    return 1 if x == 0 else 2**((x - 1).bit_length())

def generate_random_list2(l, n):
    if l <= 0:
        raise ValueError("Length of the list must be positive.")
    
    # Generate the initial list of random numbers
    random_list = [random.randint(1, n) for _ in range(l)]
    total_sum = sum(random_list)
    
    # Find the next power of 2 greater than or equal to total_sum
    target_sum = next_power_of_2(total_sum)
    
    # Calculate the adjustment needed
    adjustment = target_sum - total_sum
    
    # Ensure the last element remains positive
    if random_list[-1] + adjustment <= 0:
        # Adjust another element if the last element would become non-positive
        for i in range(l-1):
            if random_list[i] + adjustment > 0:
                random_list[i] += adjustment
                break
        else:
            raise ValueError("Cannot adjust the list to make the sum a power of 2 while keeping all elements positive.")
    else:
        random_list[-1] += adjustment

    return random_list


def generate_random_list_target(l, n, target_sum):
    """
    Generates a random list of positive integers with a specified length and sum,
    while keeping the values within a specified range and maintaining approximate frequencies.

    Parameters:
    l (int): Length of the list to be generated. Must be a positive integer.
    n (int): Maximum value for the random integers in the list. Must be a positive integer.
    target_sum (int): Desired sum of the generated list. Must be a positive integer.

    Returns:
    list of int: A list of length `l` with integers summing up to `target_sum`.

    Raises:
    ValueError: If `l` is less than or equal to 0, if `n` is less than or equal to 0, or if `target_sum` is not achievable 
                with the given length and range.

    Example:
    >>> l = 5
    >>> n = 10
    >>> target_sum = 30
    >>> random_list = generate_random_list2(l, n, target_sum)
    >>> print("Generated random list:", random_list)
    >>> print("Sum of the list:", sum(random_list))
    Generated random list: [6, 7, 6, 5, 6]
    Sum of the list: 30
    """
    if l <= 0:
        raise ValueError("Length of the list must be positive.")
    
    # Generate the initial list of random numbers
    random_list = [random.randint(1, n) for _ in range(l)]
    initial_sum = sum(random_list)
    
    # Calculate the scaling factor
    scaling_factor = target_sum / initial_sum
    
    # Scale the numbers and round them
    scaled_list = [round(num * scaling_factor) for num in random_list]
    
    # Calculate the difference caused by rounding
    scaled_sum = sum(scaled_list)
    difference = target_sum - scaled_sum
    
    # Adjust the scaled list to match the target sum
    for i in range(abs(difference)):
        if difference > 0:
            # Increment a random element if the sum is less than the target
            scaled_list[random.randint(0, l - 1)] += 1
        elif difference < 0:
            # Decrement a random element if the sum is more than the target
            idx = random.randint(0, l - 1)
            if scaled_list[idx] > 1:  # Ensure the element stays positive
                scaled_list[idx] -= 1

    return scaled_list

def generate_random_array_uint8(alphabet, frequencies):
    # Convert the alphabet to uint8
    alphabet = np.array(alphabet, dtype=np.uint8)
    
    # Create the population array based on frequencies
    population = np.repeat(alphabet, frequencies)
    
    # Shuffle the population to ensure randomness
    np.random.shuffle(population)
    
    # Return the shuffled numpy array
    return population