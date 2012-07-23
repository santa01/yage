#ifndef LOGGER_H
#define	LOGGER_H

class Logger {
    public:
        static const int LOG_ERROR = 0;
        static const int LOG_WARNING = 1;
        static const int LOG_INFO = 2;

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
