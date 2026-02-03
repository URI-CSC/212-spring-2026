# Homework Assignment 1 (due Feb 9th 11:59pm)

The goal of this assignment is to refamiliarize yourself with basic C/C++ programming and refine your problem-solving skills. For each problem, you will formulate an algorithm, write the corresponding source code, and thoroughly test it before submission. Be sure to carefully read each problem, as every detail plays a role in the solution. You are expected to learn how to compile, link, and run C/C++ programs as part of this assignment.

This assignment must be completed individually. While you may discuss problem understanding and algorithms with other students, you are not allowed to share or acquire source code from anyone else in the class, current or previous. Start early to ensure you have adequate time to seek help from TAs, the instructor, or through [Ed Discussion](https://edstem.org/). You may post general questions and clarifications using the appropriate folder tags, but do not post your source code. 

The assignment consists of 11 problems, each worth 10 points, for a total of 110 points. However, only a maximum of 100 points will be counted towards your final grade. This means you can afford to miss up to 10 points without penalty.

## Methodology 

Create a source file named `functions.cpp` that contains solutions to all problems below. For each problem, implement a function using the provided function header exactly as specified. You may add helper functions as needed, but **do not modify the given function headers**.

You are encouraged to create an additional file (e.g., main.cpp) for testing your functions with various edge and corner cases. This testing file does not need to be submitted. Do not look up solutions via LLMs or online resources, doing so is detrimental to your learning and success in this class.

> [!IMPORTANT]
> We strongly encourage to **DISABLE** any LLM-based code completion or generation tools in your IDE while working on this assignment. This assignment is meant to help you practice your problem-solving and coding skills, and using such tools will defeat the purpose of the exercise.

## Problems

### Problem 1
In the hexadecimal number system, numbers are represented using $16$ different digits: $\{0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F\}$. The hexadecimal number `AF` in decimal equals: $10\cdot16^{1}+15\cdot16^{0}=175$. Write a function named `hex2dec` that returns the decimal number corresponding to the input hexadecimal string `hex`. You cannot use any built-in conversion functions. Input strings are case insensitive (i.e., `A` and `a` both represent 10).
```c++
unsigned long int hex2dec(const std::string& hex);
```

### Problem 2
Create a function `string_hash` that takes a string `str` as input and returns a hash value between $0$ and $250$, inclusive. You cannot use any built-in hash functions. The hash value is computed using the $3x+y$ rule: starting with a hash value of $0$, for each character in the string (from left to right), multiply the current hash value by $3$ and add the ASCII value of the character, then take modulo $251$. For example, for the string "`abc`":
- Start with hash = $0$
- Process 'a' (ASCII 97): hash = $(3 \times 0 + 97) \mod 251 = 97$
- Process 'b' (ASCII 98): hash = $(3 \times 97 + 98) \mod 251 = 389 \mod 251 = 138$
- Process 'c' (ASCII 99): hash = $(3 \times 138 + 99) \mod 251 = 513 \mod 251 = 11$
- Final hash value = $11$
```c++
unsigned char string_hash(const std::string& str);
```

### Problem 3
Write a function `dec2hex` that takes a positive integer $d$ (where $0\leq d< 2^{32}$) and returns the hexadecimal representation of the decimal number $d$. The returned string should have the prefix `0x` and be padded with zeros to 8 hex digits (32 bits). For example, for $d=342576$, your function should return `0x00053a30`. You cannot use any built-in conversion functions. To convert a decimal number to hexadecimal, repeatedly divide the number by $16$ and record the remainders. The hexadecimal digits are formed by the remainders read in reverse order.
```c++
std::string dec2hex(unsigned int d);
```

### Problem 4
Create a function `armstrong` that checks whether an integer $n$ (where $0\leq n < 2^{16}$) is an Armstrong number. An Armstrong number equals the sum of its own digits each raised to the power of the number of digits. For example, $371$ is an Armstrong number since $3^3 + 7^3 + 1^3 = 371$. Return `true` if $n$ is an Armstrong number and `false` otherwise.
```c++
bool armstrong(unsigned short int n);
```

### Problem 5
Write a function `str_search` that takes two strings, `pattern` and `text`, and returns the number of occurrences of `pattern` in `text`. For example, if `pattern == "ana"` and `text == "ana ate the banana"`, the result should be $3$. The length of both strings will not exceed $2^8-1$ characters. You cannot use any built-in string searching functions.
```c++
unsigned char str_search(const std::string& pattern, const std::string& text);
```

### Problem 6
Create a function `l_mode` that finds the index of the value that occurs most frequently in an array. The input is an array `A` with $n$ integers, where $0 < n \leq 10^3$. If `v` is the most frequent element, return the index of its first (leftmost) occurrence in `A`. You can assume $-500 \leq A[i] \leq 500$ for all elements. Ties must be broken by choosing the smallest element value. You cannot use any built-in sorting functions or data structures such as maps or sets.
```c++
unsigned int l_mode(const std::vector<int>& A);
```

### Problem 7
Create a function `diameter_2d` that, given a finite set `P` of $n$ 2D points (where $1 < n < 10^3$), calculates the floor of the largest Euclidean distance between any two points in `P`. The input consists of two arrays: one storing the $x$ coordinates and one storing the $y$ coordinates.
```c++
unsigned long diameter_2d(const std::vector<double>& x, const std::vector<double>& y);
```

### Problem 8
Create a function `max_sub_sum` that takes an array `A` of integers and two output parameters `start` and `end`. Find the contiguous subarray in `A` with the largest sum and return that sum. Set `start` and `end` to the starting and ending indices (zero-based) of the leftmost occurrence of such a subarray. For example, if `A = {1, -2, 3, 4, -1, 2, 1, -5, 4}`, the contiguous subarray with the largest sum is `{3, 4, -1, 2, 1}` with sum $9$. The function should return $9$ and set `start = 2` and `end = 6`. Assume the array contains at least one positive number.
```c++
int max_sub_sum(const std::vector<int>& A, unsigned int& start, unsigned int& end);
```

### Problem 9
Create a function `unique_letters` that returns the number of unique letters in a string `str`. The function should be case-insensitive (i.e., `A` and `a` are the same). Non-letter characters should be ignored. For example, for the input "`Hello, World!`", the function should return $7$ (unique letters: `h`, `e`, `l`, `o`, `w`, `r`, `d`).
```c++
unsigned int unique_letters(const std::string& str);
```

### Problem 10
Create a function `same_gmail` that checks whether two email addresses belong to the same Gmail account. Return `true` if both addresses correspond to the same account, `false` otherwise. In Gmail, the comparison rules are:
- Both the username and domain parts are **case-insensitive**
- Dots (`.`) in the username are ignored
- Any characters after a plus sign (`+`) in the username are ignored

For example, `"csc...2.1.2@gmail.com"`, `"csc212@gmail.com"`, `"CSC.212.@gmail.com"`, `"CSC212@GMail.Com"`, `"csc212+announce@gmail.com"`, and `"csc.212+spring-2017@gmail.com"` all correspond to the same Gmail account. See [Official Gmail blog post](https://gmail.googleblog.com/2008/03/2-hidden-ways-to-get-more-from-your.html).
```c++
bool same_gmail(const std::string& email1, const std::string& email2);
```

### Problem 11
Create a function `to_hostname` that extracts the hostname from a URL string. The hostname is the domain name portion of the URL. URLs follow the format `protocol://hostname/path?query`, where the protocol (e.g., `http`, `https`, `ftp`, `file`) is followed by `://`, then the hostname, then optionally a path and query string. Your function should extract the hostname from a URL and return it in lowercase. For example, for the input URL "`https://WWW.Example.com/path/to/page?query=123`", the function should return "`www.example.com`".  You can assume the input URL always contains `://` to separate the protocol from the hostname.
```c++
std::string to_hostname(const std::string& url);
```

## Submission and Grading
This assignment relies on automated evaluation. Once you are finished, you **must** submit the files listed below via [Gradescope](https://www.gradescope.com/) to record your grade. Use the exact filenames provided here:

- `functions.cpp`
- `llm-usage.txt`

In `functions.cpp`, include all your function implementations for the problems above. You must use the exact function headers provided. Any deviation will cause autograding tests to fail, resulting in no credit for that problem. Use proper coding style, indentation, and comments in all functions; points may be deducted otherwise.

The `llm-usage.txt` file should contain: the name of the LLM, if you used one, and a copy of the prompts you entered and the 
responses you received.  If you did not use an LLM, simply write "No LLM used".

> [!CAUTION]
> Remember, academic integrity is of utmost importance.  Any attempts at cheating or plagiarism will result in a forfeiture of credit.  Potential further actions include failing the class or referring the case for disciplinary measures.
