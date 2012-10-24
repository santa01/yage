#ifndef LOGGER_H
#define	LOGGER_H

class Logger {
    public:
        enum {
            LOG_ERROR,
            LOG_WARNING,
            LOG_INFO
        };

        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        void log(int level, const char* message, ...);
        void setThreshold(int threshold);
       
    private:
        Logger();
        Logger(const Logger& logger);
        Logger& operator =(const Logger& logger);

        int threshold;
};

#endif	/* LOGGER_H */
