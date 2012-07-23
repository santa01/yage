#ifndef CONFIG_H
#define	CONFIG_H

#include <string>

class Config {
    public:
        Config();
        
        bool load(const std::string& name);

    private:
        Config(const Config& orig);
        Config& operator =(const Config& orig);
};

#endif	/* CONFIG_H */
