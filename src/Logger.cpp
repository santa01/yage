#include <cstdio>
#include <cstdarg>
#include <cstring>

#include "Logger.h"

Logger::Logger() {
    this->threshold = Logger::LOG_ERROR;
}

void Logger::log(int level, const char* message, ...) {
    if (level > this->threshold)
        return;

    FILE* stream = (level == Logger::LOG_ERROR) ? stderr : stdout;
    switch (level) {
        case Logger::LOG_INFO:
            fprintf(stream, "Info: ");
            break;

        case Logger::LOG_WARNING:
            fprintf(stream, "Warning: ");
            break;

        case Logger::LOG_ERROR:
            fprintf(stream, "Error: ");
            break;
    }

    va_list ap;
    va_start(ap, message);
    vfprintf(stream, message, ap);
    va_end(ap);

    fprintf(stream, "\n");
}

void Logger::setThreshold(int threshold) {
    switch (threshold) {
        case Logger::LOG_INFO:
        case Logger::LOG_WARNING:
        case Logger::LOG_ERROR:
            this->threshold = threshold;
            break;          
    }
}
