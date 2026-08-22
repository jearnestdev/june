#pragma once

#include <expected>

#include "june/network/LlamaClient.h"
#include "june/network/LlamaServer.h"

namespace june
{
    enum class LlamaBackendError
    {
        ServerStartFailed,
        ServerStopFailed,
    };

    enum class LlamaBackendState
    {
        Error,
        NotRunning,
        Starting,
        Stopped,
    };

    const char* toString(LlamaBackendState state);

    class LlamaBackend
    {
        public:
            std::expected<void, LlamaBackendError> start();
            std::expected<void, LlamaBackendError> stop();

            LlamaBackendState getState() const;
        private:
            LlamaServer m_server;
            LlamaClient m_client;

            LlamaBackendState m_state = LlamaBackendState::NotRunning;
    };
}