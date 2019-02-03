#ifndef PORTABLESLEEP_H
#define PORTABLESLEEP_H

#ifdef _WIN32
#  include <Windows.h>
#else
#  include <unistd.h>
#endif

class PortableSleep
{
    public:
        static void msleep(unsigned int milliSec)
        {
#ifdef _WIN32
            Sleep(milliSec);
#else
            usleep(milliSec * 1000);
#endif
        };
};

#endif // PORTABLESLEEP_H
