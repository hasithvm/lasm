/* ---------------------------------------------------------------------------
** Logger.hpp
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** Logger.hpp
** 
** @Author: hasith
** @Date:   2014-09-07 16:41:13
** @Last Modified by:   hasith
** @Last Modified time: 2014-09-07 17:05:21
** -------------------------------------------------------------------------*/

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <iomanip>
#include <stdexcept>

class Logger
{
    public:
        typedef enum
        {
            OUTPUT_NONE,
            OUTPUT_CLOG,
            OUTPUT_FILE,

        } output_t;

        void SetOutput(output_t output_type);
        static Logger& Instance();
        template <typename T>
        Logger& operator<<(const T& object)
        {
            
            if (!m_out)
                throw std::runtime_error("FATAL: no logging output detected!");
            *m_out << object;
            return *this;
        };
        friend Logger& operator<<(Logger& loggerInstance, std::ostream & (*manip)(std::ostream &));
        friend Logger& setIndent(Logger& loggerInstance, int indentLevel);
        ~Logger();

    private:

        Logger();
        Logger(const Logger& l);
        void operator=(const Logger&);

        void free_stream();
        class nullBuffer : public std::streambuf {
        public:
            virtual std::streamsize xsputn (const char * s, std::streamsize n)
            {
                s++;
                return n;
            }
        virtual int overflow (int c) {
            c = c;
            return 1;
        }
        };
        class NullStream : public std::ostream {
            public:
                NullStream() : std::ostream (&buf) {}
            private:
                nullBuffer buf;
        };

        std::ostream* m_out;
        bool owns;

};
Logger& setIndent(Logger& loggerInstance, int indentLevel);
#endif