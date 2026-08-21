#include "june/network/LlamaServer.h"

namespace june
{
    std::expected<void, LlamaServerError> LlamaServer::init()
    {
        // Fork process.
        m_pid = fork();

        if (m_pid == -1)
        {
            // Fork failed.
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
            perror("excel");
            // Exit only child process.
            _exit(1);
        }

        // Parent process.
        std::println("Started llama server process (hopefully) with PID = {}", m_pid);

        // Return success.
        return {};
    }

    std::expected<void, LlamaServerError> LlamaServer::shutdown()
    {
        // Kill process.
        if (m_pid > 0)
        {
            // Send kill signal to proces via "SIGTERM."
            kill(m_pid, SIGTERM);
            m_state = LlamaServerState::Stopping;

            // Wait until process is actually terminated.
            waitpid(m_pid, nullptr, 0);
            m_state = LlamaServerState::Stopped;
            std::println("Stopped llama server process (hopefully) with PID = {}", m_pid);

            // Set process ID back to -1.
            m_pid = -1;
        }
        // Return success.
        return {};
    }

    june::LlamaServerState LlamaServer::getState() const
    {
        return m_state;
    }
}
