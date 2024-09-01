#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/wait.h>
#include <poll.h>



#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>

int main() {
    // Example environment variables for CGI
    char *envp[] = {
        (char *)"CONTENT_LENGTH=13",  // Example content length
        (char *)"CONTENT_TYPE=application/x-www-form-urlencoded",
        (char *)"REQUEST_METHOD=POST",
        nullptr
    };

    // Example POST data
    const char *post_data = "name=John&age=30";
    size_t post_data_len = strlen(post_data);

    // Create a pipe to pass the POST data to the child process
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create a pipe to capture the output of the child process
    int output_pipefd[2];
    if (pipe(output_pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        // Close the write end of the input pipe
        close(pipefd[1]);

        // Redirect stdin to the read end of the input pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        // Close the read end of the output pipe
        close(output_pipefd[0]);

        // Redirect stdout to the write end of the output pipe
        dup2(output_pipefd[1], STDOUT_FILENO);
        close(output_pipefd[1]);

        // Arguments for execve
        char *argv[] = {
            (char *)"/usr/bin/python3",
            (char *)"./cgi-bin/python/hello.py",
            nullptr
        };

        // Execute the script
        execve("./cgi-bin/python/hello.py", argv, envp);

        // If execve fails
        perror("execve");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        // Close the read end of the input pipe
        close(pipefd[0]);

        // Write the POST data to the write end of the input pipe
        write(pipefd[1], post_data, post_data_len);
        close(pipefd[1]);

        // Close the write end of the output pipe
        close(output_pipefd[1]);

        // Read the output from the read end of the output pipe
        std::vector<char> buffer(1024);
        std::string result;
        ssize_t bytes_read;
        while ((bytes_read = read(output_pipefd[0], buffer.data(), buffer.size())) > 0) {
            result.append(buffer.data(), bytes_read);
        }
        close(output_pipefd[0]);

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        // Print the captured output
        std::cout << "Captured output: " << result << std::endl;
    }

    return 0;
}