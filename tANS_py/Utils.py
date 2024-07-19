# Generate random string
import random
import numpy as np
import math


def next_power_of_2(x):
    # helper function to find the next power of 2
    return 1 if x == 0 else 2**((x - 1).bit_length())

def next_lower_power_of_2(x):   
    # helper function to find the next lower power of 2
    return 2**(x - 1).bit_length()

def generate_random_list_pow2(l, n):
    # Generates a list of random numbers with a length of `l` and a sum that is a power of 2.
    
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

def rescale_list_to_power_of_2(input_list, max_sum, max_sum_var = "max_sum", depth_limit=1000):
    """Rescales a list of integers to have a sum that is the nearest power of 2 less than or equal to a specified value.

    Args:
        input_list (list): input list of integers to be rescaled
        max_sum (int): the maximum sum of the rescaled list
        max_sum_var (str, optional): the name of the variable corresponding to max_sum. Defaults to "max_sum". Helps in error messages.
        depth_limit (int, optional): the maximum number of iterations to adjust the list. Defaults to 1000. If the depth limit is reached, an error is raised.

    Raises:
        ValueError: if the sum of the input list is zero
        ValueError: if the max_sum is less than 1

    Returns:
        list: the rescaled list of integers with the sum being the nearest power of 2 less than or equal to max_sum
    """
    current_sum = sum(input_list)
    if current_sum == 0:
        raise ValueError("The sum of the list elements is zero, cannot rescale.")
    
    if max_sum < len(list(set(input_list))):
        raise ValueError(f"{max_sum_var} must be greater than number of symbols.")
    
    # Find the nearest power of 2 less than or equal to max_sum
    nearest_power_of_2 = next_lower_power_of_2(max_sum)
    
    # Calculate the scaling factor
    scaling_factor = nearest_power_of_2 / current_sum
    
    # Scale the list elements and round them to integers, ensuring no zero values
    scaled_list = [max(1, int(round(x * scaling_factor))) for x in input_list]
    
    # Adjust the sum to be exactly the nearest power of 2
    difference = nearest_power_of_2 - sum(scaled_list)
    
    # set the depth limit to avoid infinite loop
    depth = 0
    # Adjust the elements to ensure the sum is correct and no value is zero
    while difference != 0 and depth < depth_limit:
        depth += 1
        if difference > 0:
            # Find the index to increment
            index_to_adjust = scaled_list.index(min(scaled_list))
            scaled_list[index_to_adjust] += 1
            difference -= 1
        elif difference < 0:
            # Find the index to decrement (but ensure no value goes below 1)
            index_to_adjust = scaled_list.index(max(scaled_list))
            if scaled_list[index_to_adjust] > 1:
                scaled_list[index_to_adjust] -= 1
                difference += 1
            else:
                # If we can't decrement, we need to increment another value to balance
                index_to_adjust = scaled_list.index(min(scaled_list))
                scaled_list[index_to_adjust] += 1
                difference -= 1
    
    if depth == depth_limit:
        raise ValueError(f"Reached depth limit. Could not rescale the list. Please increase {max_sum_var}")
    
    return scaled_list

def generate_random_string(alphabet, frequencies):
    # Generates a random string based on the given alphabet and frequencies. Useful for testing.
    population = []
    
    # Create the population list based on frequencies
    for symbol, freq in zip(alphabet, frequencies):
        population.extend([symbol] * freq)
    
    # Shuffle the population to ensure randomness
    random.shuffle(population)
    
    # Join the list into a string
    random_string = ''.join(population)
    
    return random_string
