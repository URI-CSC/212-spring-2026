# Lab 01 - Benchmarking algorithms

## Overview

In this lab, you will review fundamental C++ programming concepts and practice using development tools essential for the course. You'll start by setting up and testing your C++ development environment with a simple console program. Then, you'll implement and analyze two algorithms for the classic two sum problem: a brute-force approach and an optimized two-pointer technique. Through empirical benchmarking, you'll observe how algorithm efficiency impacts execution time as input sizes scale. This hands-on experience will reinforce your understanding of C++ syntax, algorithmic complexity, and performance measurement techniques.

> [!CAUTION]
> For this lab, **avoid** using large language models (LLMs) to generate code or solutions for the exercises. In particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE while working on this lab.  You may ask specific questions to LLMs about concepts or language features/syntax, but we strongly discourage using LLMs to write code for you.

## Task 1: Testing Your Development Environment

Before starting the exercises, ensure that your development environment is correctly set up to compile and run C++ programs.

1. Open VSCode and create a new file named `hello.cpp`. Write a simple program that reads an integer value $n$ from the user and outputs a pyramid pattern of $n$ rows to the console, using asterisks. Use a loop to control the rows and conditionals to determine spacing and star placement. For example, if the user inputs `5`, the output should be:
```
    *
   ***
  *****
 *******
*********
```
2. Compile the program using the terminal window in VSCode with the command:
```bash
$ g++ hello.cpp -o hello
```
3. If no errors occur during compilation, you might want to compile your program with additional flags to tell the compiler to report all warnings and errors. Use the command:
```bash
$ g++ -std=c++11 -Wall -Werror hello.cpp -o hello
```
4. If no errors are reported during compilation, run the program with the command below. Otherwise, fix any errors before compiling again.
```bash
$ ./hello
```
5. Enter a number (e.g., `5`) when prompted. If you see the correct pyramid pattern printed in the terminal, your environment is set up correctly! You may also test with other input values to ensure the program behaves as expected.

## Task 2: Two Sum Problem

In this task you will implement a simple benchmarking tool to measure the execution time of different algorithms that solve the two sum problem (2SUM). The two sum problem is defined as follows: given an array of integers and a target integer, determine if there are two integers in the array that sum up to the target.

### 2SUM (brute force)

You will complete the `two_sum_brute_force` function below, which returns `true` if there are two integers in the array that sum up to the target, and `false` otherwise. For this function, you should implement a simple brute-force algorithm. It should check **all pairs of integers** in the array to see if any pair sums to the target.

```cpp
bool two_sum_brute_force(const std::vector<int>& array, int target) {
    // TODO: Implement the brute-force algorithm to solve 2SUM
    // ...
}
```

### 2SUM (two-pointer technique)

Now you will implement a faster approach, which first sorts the array and then uses the two-pointer technique to find if there are two integers that sum to the target. The two-pointer technique involves maintaining two indices `left` and `right`, which are initialized to the indices of the first and last elements of the sorted array, respectively. A loop is used to check the sum of the elements at these indices. If the sum is equal to the target, the function returns `true`. If the sum is less than the target, the `left` index is incremented to increase the sum. If the sum is greater than the target, the `right` index is decremented to decrease the sum. This process continues until the `left` index is no longer less than the `right` index. You should first trace a few examples of this algorithm on paper to understand how it works and then proceed to complete the `two_sum_two_pointer` function below:

```cpp
bool two_sum_two_pointer(const std::vector<int>& array, int target) {
    // store the number of elements in the array
    const size_t n = array.size();
    // if the array has less than 2 elements, return false
    if (n < 2) {
        return false;
    }
    // make a copy of the array to sort
    std::vector<int> copy = array;
    // sort the copy of the array
    std::sort(copy.begin(), copy.end());
    // TODO: Implement the two-pointer technique to solve 2SUM
    // ...
}
```

### Testing Correctness

Before benchmarking the two algorithms, you should ensure that both implementations are correct. You can do this by writing unit tests that cover various scenarios, including edge cases. Here are some example test cases you can use:

```cpp
void test_two_sum_functions() {
    std::vector<int> array1 = {-2, 7, 11, 0};
    std::vector<int> array2 = {-10, -5, 0, 5, 10, -4, -3, -2, -1, 0};
    // the assert statements below should all pass 
    // if your implementations are correct, and terminate
    // the program with an error otherwise
    assert(two_sum_brute_force(array1, 9) == true);
    assert(two_sum_two_pointer(array1, 9) == true);
    assert(two_sum_brute_force(array1, -2) == true);
    assert(two_sum_two_pointer(array1, -2) == true);
    assert(two_sum_brute_force(array1, -1) == false);
    assert(two_sum_two_pointer(array1, -1) == false);
    assert(two_sum_brute_force(array1, 0) == false);
    assert(two_sum_two_pointer(array1, 0) == false);

    assert(two_sum_brute_force(array2, -15) == true);
    assert(two_sum_two_pointer(array2, -15) == true);
    assert(two_sum_brute_force(array2, 0) == true);
    assert(two_sum_two_pointer(array2, 0) == true);
    assert(two_sum_brute_force(array2, 500) == false);
    assert(two_sum_two_pointer(array2, 500) == false);
    assert(two_sum_brute_force(array2, 17) == false);
    assert(two_sum_two_pointer(array2, 17) == false);
}
```

At this point, you can compile and run your code to ensure that both implementations pass all the tests. Create a file named `two_sum.cpp` that includes the functions `two_sum_brute_force`, `two_sum_two_pointer`, `test_two_sum_functions`, and a `main` function that calls `test_two_sum_functions`.
```bash
$ g++ -std=c++11 -Wall -Werror two_sum.cpp -o two_sum 
$ ./two_sum
```

### Empirical Analysis

After verifying the correctness of both implementations, you can proceed to benchmark their execution times. You can use the `<chrono>` library in C++ to measure the time taken by each function to execute on large input arrays. Here is an example of how you can measure the elapsed execution time for a piece of code:

```cpp
auto start = std::chrono::steady_clock::now();
// statements to be timed
auto end = std::chrono::steady_clock::now();
std::chrono::duration<double> duration = end - start;
std::cout << "Time: " << duration.count() << " seconds" << std::endl;
```

With the hints provided above, your task is now to modify `two_sum.cpp` to output a small table comparing the execution times of both algorithms on arrays of increasing sizes: (1000; 5000; 10000; 50000; 100000  elements). Each column of the table should correspond to one of the algorithms, and each row should correspond to one of the input sizes. Each entry in that table should be the average elapsed time of 5 runs of the algorithm using the same input vector and target value. You can generate random integer arrays using the `<random>` library in C++. Here is function you can use to generate a random integer array of size `n` and values between -1000 and 1000:

```cpp
std::vector<int> generate_random_array(size_t n) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(-1000, 1000);
    std::vector<int> arr(n);
    for (auto& x : arr) x = dist(gen);
    return arr;
}
```

For benchmarking, use a target value that is **unlikely to exist** in the array, such as `target = 99999`. This ensures both algorithms examine a similar number of elements before returning `false`, providing a fair comparison of their worst-case performance.


After completing the benchmarking code, compile and run your program again. You should see a table printed to the console showing the execution times for both algorithms on the specified input sizes. See the example output below:

```
Input Size     Brute Force     Two-Pointer
1000           0.0023          0.0001
5000           0.0587          0.0005
10000          0.2345          0.0010
50000          5.6789          0.0050
100000         22.3456         0.0100
```

> [!WARNING]
> All values in the table above are merely illustrative. Your actual execution times may vary based on your machine's performance and the randomness of the input arrays.

## Submission

After you have completed all the tasks, submit the files below to Gradescope. Your code should be well-formatted and easy to read. Make sure to test your code thoroughly before submitting it. Full credit for this lab requires **attendance**, **collaboration with your group**, and **active participation**. No remote submissions will be accepted. If you have any questions, please ask the instructor or the TA for help. 

- `hello.cpp`
- `two_sum.cpp`



