#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ_END 0   // Define pipe read end
#define WRITE_END 1  // Define pipe write end

// Function to send an integer through the pipe
void pipe_send(int pipe_fd, int data) {
    write(pipe_fd, &data, sizeof(int));
}

// Function to receive an integer from the pipe
int pipe_receive(int pipe_fd) {
    int data;
    read(pipe_fd, &data, sizeof(int));
    return data;
}

int main(int argc, char *argv[]) {
    // Check if exactly two command line arguments (4-digit numbers) are provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <4-digit-number> <4-digit-number>\n", argv[0]);
        return 1;
    }

    // Convert the input arguments to integers
    int first_input = atoi(argv[1]);
    int second_input = atoi(argv[2]);

    // Ensure both numbers are 4-digit integers
    if (first_input < 1000 || first_input > 9999 || second_input < 1000 || second_input > 9999) {
        fprintf(stderr, "Both numbers must be 4-digit integers (1000-9999)\n");
        return 1;
    }

    printf("Your integers are %d %d\n", first_input, second_input);

    // Create two pipes: one for parent-to-child communication, one for child-to-parent
    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];

    // Initialize pipes
    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("pipe");
        exit(1);
    }

    // Create child process
    pid_t process_id = fork();

    if (process_id == -1) {
        perror("fork");
        exit(1);
    }

    if (process_id == 0) {
        // Child process
        close(pipe_parent_to_child[WRITE_END]);
        close(pipe_child_to_parent[READ_END]);

        while (1) {
            // Receive two numbers from the parent
            int child_num1 = pipe_receive(pipe_parent_to_child[READ_END]);
            int child_num2 = pipe_receive(pipe_parent_to_child[READ_END]);

            if (child_num1 == -1 || child_num2 == -1) break;  // Exit signal from parent

            printf("Child (PID %d): Received %d from parent\n", getpid(), child_num1);
            printf("Child (PID %d): Received %d from parent\n", getpid(), child_num2);

            // Multiply the numbers
            int product = child_num1 * child_num2;
            printf("Child (PID %d): Sending %d to parent\n", getpid(), product);

            // Send result back to parent
            pipe_send(pipe_child_to_parent[WRITE_END], product);
        }

        // Close pipes in the child process
        close(pipe_parent_to_child[READ_END]);
        close(pipe_child_to_parent[WRITE_END]);
        exit(0);
    } else {
        // Parent process
        close(pipe_parent_to_child[READ_END]);
        close(pipe_child_to_parent[WRITE_END]);

        printf("Parent (PID %d): Created child (PID %d)\n\n", getpid(), process_id);

        // Split the 4-digit numbers into 2-digit pairs
        int first_pair1 = first_input / 100;
        int first_pair2 = first_input % 100;
        int second_pair1 = second_input / 100;
        int second_pair2 = second_input % 100;

        // Calculating the X term
        printf("###\n# Calculating X\n###\n");
        printf("Parent (PID %d): Sending %d to child\n", getpid(), first_pair1);
        pipe_send(pipe_parent_to_child[WRITE_END], first_pair1);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), second_pair1);
        pipe_send(pipe_parent_to_child[WRITE_END], second_pair1);
        int result_A = pipe_receive(pipe_child_to_parent[READ_END]);
        printf("Parent (PID %d): Received %d from child\n\n", getpid(), result_A);

        // Calculating the Y term
        printf("###\n# Calculating Y\n###\n");
        printf("Parent (PID %d): Sending %d to child\n", getpid(), first_pair1);
        pipe_send(pipe_parent_to_child[WRITE_END], first_pair1);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), second_pair2);
        pipe_send(pipe_parent_to_child[WRITE_END], second_pair2);
        int result_B = pipe_receive(pipe_child_to_parent[READ_END]);
        printf("Parent (PID %d): Received %d from child\n", getpid(), result_B);

        printf("Parent (PID %d): Sending %d to child\n", getpid(), first_pair2);
        pipe_send(pipe_parent_to_child[WRITE_END], first_pair2);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), second_pair1);
        pipe_send(pipe_parent_to_child[WRITE_END], second_pair1);
        int result_C = pipe_receive(pipe_child_to_parent[READ_END]);
        printf("Parent (PID %d): Received %d from child\n\n", getpid(), result_C);

        // Calculating the Z term
        printf("###\n# Calculating Z\n###\n");
        printf("Parent (PID %d): Sending %d to child\n", getpid(), first_pair2);
        pipe_send(pipe_parent_to_child[WRITE_END], first_pair2);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), second_pair2);
        pipe_send(pipe_parent_to_child[WRITE_END], second_pair2);
        int result_D = pipe_receive(pipe_child_to_parent[READ_END]);
        printf("Parent (PID %d): Received %d from child\n\n", getpid(), result_D);

        // Sending exit signal to child
        pipe_send(pipe_parent_to_child[WRITE_END], -1);
        pipe_send(pipe_parent_to_child[WRITE_END], -1);

        // Final result calculation
        long long term_X = (long long)result_A * 10000;
        long long term_Y = ((long long)result_B + result_C) * 100;
        long long term_Z = result_D;
        long long final_result = term_X + term_Y + term_Z;

        printf("%d * %d == %lld + %lld + %lld == %lld\n", first_input, second_input, term_X, term_Y, term_Z, final_result);

        // Close pipes in the parent process
        close(pipe_parent_to_child[WRITE_END]);
        close(pipe_child_to_parent[READ_END]);
    }

    return 0;
}
