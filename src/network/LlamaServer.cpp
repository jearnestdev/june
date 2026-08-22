#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "june/network/LlamaServer.h"

namespace june
{
    std::expected<void, LlamaServerError> LlamaServer::start()
    {
        int exec_pipe[2];

        // Attempt pipe.
        if (pipe2(exec_pipe, O_CLOEXEC) == -1)
        {
            return std::unexpected(LlamaServerError::PipeFailed);
        }

        // Fork process.
        m_pid = fork();

        if (m_pid == -1)
        {
            // Fork failed.
            close(exec_pipe[0]);
            close(exec_pipe[1]);

            return std::unexpected(LlamaServerError::ForkFailed);
        }
        else if (m_pid == 0)
        {
            // Child process.
            // Start server.
            execl(
                "./external/llama.cpp/build/bin/llama-server",
                "llama-server",
                "-m", "./models/qwen3-14b-q4_k_m.gguf",
                "-ngl", "999",
                "-c", "8192",
                "--host", "127.0.0.1",
                "--port", "8080",
                nullptr
            );
            // If we are here, the execl failed.
            int exec_error = errno;

            // Tell parent what errno was.
            write(exec_pipe[1], &exec_error, sizeof(exec_error));

            // Exit only child process.
            _exit(1);
        }

        // Parent process.

        // Parent doesn't write to the pipe. 
        close(exec_pipe[1]);

        int exec_error = 0;

        // Read the errno from the pipe.
        ssize_t bytes_read = read(exec_pipe[0], &exec_error, sizeof(exec_error));

        close(exec_pipe[0]);

        if (bytes_read > 0)
        {
            // Child process reported that execl() failed.
            // Since exec failed, child process already knows to terminate so wait for it.
            waitpid(m_pid, nullptr, 0);
            m_pid = -1;

            return std::unexpected(LlamaServerError::ExecFailed);
        }

        // Return success.
        return {};
    }

    std::expected<void, LlamaServerError> LlamaServer::stop()
    {
        // Kill process.
        if (m_pid > 0)
        {
            // Send kill signal to proces via "SIGTERM."
            kill(m_pid, SIGTERM);

            // Wait until process is actually terminated.
            waitpid(m_pid, nullptr, 0);

            // Set process ID back to -1.
            m_pid = -1;
        }
        else
        {
            return std::unexpected(LlamaServerError::KillFailed);
        }
        // Return success.
        return {};
    }
}
