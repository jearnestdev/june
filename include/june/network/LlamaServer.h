#pragma once

#include <cstdio>
#include <expected>
#include <print>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

namespace june
{
    enum class LlamaServerError
    {
        ForkFailed,
        //ExecFailed,
        //KillFailed
    };

    enum class LlamaServerState
    {
        NotRunning,
        Starting,
        Running,
        Stopping,
        Stopped,
        Error
    };

    class LlamaServer
    {
        public:
            // Create the forked llama server process and boot it up.
            std::expected<void, LlamaServerError> init();
            // Kill the llama server process and clean up.
            std::expected<void, LlamaServerError> shutdown();

            // Get current state of the llama server.
            LlamaServerState getState() const;
        private:
            // Process ID of the llama server.
            pid_t m_pid = -1;
            // State the server is currently in.
            LlamaServerState m_state = LlamaServerState::NotRunning;
    };
}