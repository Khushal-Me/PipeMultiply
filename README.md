# PipeMultiply

PipeMultiply is a C program that demonstrates inter-process communication using pipes to perform large integer multiplication. It breaks down the multiplication of two 4-digit numbers into smaller operations, utilizing a parent-child process model to distribute the computation.

## Features

- Accepts two 4-digit integers as command-line arguments
- Uses pipes for bidirectional communication between parent and child processes
- Breaks down large integer multiplication into smaller, manageable calculations
- Provides detailed output of the communication between processes

## Requirements

- GCC compiler
- UNIX-like operating system (Linux, macOS, etc.)

## Compilation

To compile the program, use the following command in the terminal:

```
gcc -o pipemultiply pipemultiply.c
```

## Usage

Run the compiled program with two 4-digit integers as arguments:

```
./pipemultiply <first_4digit_number> <second_4digit_number>
```

For example:

```
./pipemultiply 1234 5678
```

## How It Works

1. The program splits each 4-digit number into two 2-digit numbers.
2. It creates a child process and establishes two-way communication using pipes.
3. The parent process sends pairs of 2-digit numbers to the child process.
4. The child process multiplies these numbers and sends the results back to the parent.
5. The parent process combines these results to calculate the final product.

The multiplication is broken down as follows:

```
1234 * 5678 = (12 * 56) * 10000 + (12 * 78 + 34 * 56) * 100 + (34 * 78)
```

## Output

The program provides detailed output showing the communication between the parent and child processes, including:

- The numbers being sent from parent to child
- The results being sent from child to parent
- The final calculation of the product

## Error Handling

- The program checks for the correct number of command-line arguments.
- It verifies that both input numbers are 4-digit integers.
- It handles potential errors in pipe creation and process forking.

## License

This project is open source and available under the [MIT License](https://opensource.org/licenses/MIT).
