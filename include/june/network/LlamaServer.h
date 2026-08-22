#pragma once

#include <expected>
#include <unistd.h>

namespace june
{
    enum class LlamaServerError
    {
        PipeFailed,
        ForkFailed,
        ExecFailed,
        KillFailed,
    };

    class LlamaServer
    {
        public:
            // Create the forked llama server process and boot it up.
            std::expected<void, LlamaServerError> start();
            // Kill the llama server process and clean up.
            std::expected<void, LlamaServerError> stop();

        private:
            // Process ID of the llama server.
            pid_t m_pid = -1;
    };
}