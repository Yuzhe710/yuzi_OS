
#ifndef __YUZIOS__DRIVERS__DRIVER_H
#define __YUZIOS__DRIVERS__DRIVER_H

namespace yuzi_os
{
    namespace drivers
    {
        class Driver
        {
        public:
            Driver();
            ~Driver();

            virtual void Activate();
            virtual int Reset();
            virtual void Deactivate();
        };

        class DriverManager
        {
        private:
            Driver* drivers[265];
            int numDrivers;

        public:
            DriverManager();
            void AddDriver(Driver*);
            void ActivateAll();
        };
    }
}



#endif