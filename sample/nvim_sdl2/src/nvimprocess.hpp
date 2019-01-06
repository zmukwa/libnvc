/*
 * =====================================================================================
 *
 *       Filename: nvimprocess.hpp
 *        Created: 01/06/2019 04:35:01
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#include "libnvc.hpp"
#include <reproc++/sink.hpp>
#include <reproc++/reproc.hpp>

class nvim_process: public libnvc::io_device
{
    private:
        reproc::process m_reproc;

    private:
        std::string m_output;

    public:
        nvim_process()
            : m_reproc(reproc::wait, reproc::infinite)
            , m_output()
        {
            if(auto ec = m_reproc.start({"nvim", "--embed"}); ec){
                throw std::runtime_error("Error when starting nvim: " + ec.message());
            }
        }

    public:
        size_t send(const char *buf, size_t length)
        {
            unsigned int done_length = 0;
            m_reproc.write(buf, length, &done_length);
            return (size_t)(done_length);
        }

    public:
        size_t recv(char *buf, size_t length)
        {
            if(m_output.empty()){
                m_reproc.drain(reproc::stream::out, reproc::string_sink(m_output));
            }

            if(m_output.empty()){
                return 0;
            }

            size_t out_length = std::min<size_t>(length, m_output.length());
            std::copy(m_output.begin(), m_output.begin() + out_length, buf);

            m_output.erase(0, out_length);
            return out_length;
        }
};
