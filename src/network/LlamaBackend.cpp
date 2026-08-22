#include <print>

#include "june/network/LlamaBackend.h"

namespace june
{
    const char* toString(LlamaBackendState state)
    {
        switch (state)
        {
            case LlamaBackendState::Error:
            {
                return "Error";
                break;
            }
            case LlamaBackendState::NotRunning:
            {
                return "Not Running";
                break;
            }
            case LlamaBackendState::Starting:
            {
                return "Starting...";
                break;
            }
            case LlamaBackendState::Stopped:
            {
                return "Stopped";
                break;
            }
            default:
            {
                return "Unknown State";
                break;
            }
        }
    }

    std::expected<void, LlamaBackendError> LlamaBackend::start()
    {
        // Initiate server start up.
        auto server_status = m_server.start();

        // No errors. Server is now attempting to start.
        if (server_status)
        {
            m_state = LlamaBackendState::Starting;
            std::println("Server is now starting.");
        }
        else 
        {
            m_state = LlamaBackendState::Error;
            // Error logging.
            switch (server_status.error())
            {
                case LlamaServerError::PipeFailed:
                {
                    std::println("Server failed to start due to pipe failure.");
                    break;
                }
                case LlamaServerError::ForkFailed:
                {
                    std::println("Server failed to start since its process could not be forked.");
                    break;
                }
                case LlamaServerError::ExecFailed:
                {
                    std::println("Server failed to start due to invalid execl().");
                    std::println("   | Check server configuration.");
                    break;
                }
                default:
                {
                    std::println("Server failed to start due to unknown reason.");
                    break;
                }
            }
            // Return server failure error.
            return std::unexpected(LlamaBackendError::ServerStartFailed);
        }
        // Return success.
        return {};
    } 
    std::expected<void, LlamaBackendError> LlamaBackend::stop()
    {
        auto server_status = m_server.stop();

        if (server_status)
        {
            m_state = LlamaBackendState::Stopped;
            std::println("Server has stopped.");
        }
        else
        {
            switch (server_status.error())
            {
                case LlamaServerError::KillFailed:
                {
                    std::println("Server failed to stop because the process could not be killed.");
                    break;
                }
                default:
                {
                    std::println("Server failed to stop due to unknown reason.");
                    break;
                }
            }
            // Return server failure error.
            return std::unexpected(LlamaBackendError::ServerStopFailed);
        }
        // Return success.
        return {};
    } 

    LlamaBackendState LlamaBackend::getState() const
    {
        return m_state;
    }
}