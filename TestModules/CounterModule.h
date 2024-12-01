#ifndef COUNTERMODULE_H
#define COUNTERMODULE_H

#include <vector>

class CounterModule {
public:
    void renderStandalone();

    void saveLogToJson(const std::vector<int> &values);
};

#endif // COUNTERMODULE_H
