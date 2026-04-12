//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_MYTOOLS_H
#define CPP_CODE_MYTOOLS_H

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <string>
#include "types.h"
#include <iomanip>
#include <memory>
#include <chrono>

//-----------------------------------------------------------------------------
//  MyTools class
//  Define the general tools of the project
//-----------------------------------------------------------------------------
using std::chrono::high_resolution_clock;

namespace myTools {

    //-----------------------------------------------------------------------------
    //  TIMER CLASS
    //-----------------------------------------------------------------------------
    class Timer {
    private:
        high_resolution_clock::time_point cpuInit_;        // time point of the initialization of the timer
        std::chrono::duration<double> cpuSinceStart_;      // time duration since the last start of the timer
        std::chrono::duration<double> cpuSinceInit_;       // time duration since the initialization of the timer

        int coStop_;	            //number of times the timer was stopped
        bool isInit_;               // flag to check if the timer is initialized
        bool isStarted_;            // flag to check if the timer is started
        bool isStopped_;            // flag to check if the timer is stopped

        // Constructor and Destructor
    public:
        Timer();
        virtual ~Timer();


        void init();                // function fo initialize the timer
        bool isInit() const;        // function to check initialization status
        void start();               // function to start the timer
        void stop();                // function to stop the timer
        void addTime(double sec);

        // get the time spent since the initialization of the timer and since the last
        // time it was started
        std::chrono::duration<double> dSinceInit();
        std::chrono::duration<double> dSinceStart();
    };

    //-----------------------------------------------------------------------------
    //  MY EXCEPTION CLASS
    //-----------------------------------------------------------------------------

    class myException : public std::exception {
        std::string msg_;            // message of the exception

    public:
        myException(std::string message, int line)
            : msg_(std::move(message) + " (line " + std::to_string(line) + ")") {}

        const char *what() const noexcept override { return msg_.c_str(); }
        static void throwError(const char *exceptionMsg) {
            throwError(std::string(exceptionMsg).c_str());
        }
    };


    //-----------------------------------------------------------------------------
    //  COUT REDIRECTOR CLASS
    //  Define a RAII guard to manage std::cout redirection
    //-----------------------------------------------------------------------------
    class CoutRedirector {
        std::ofstream logFile_;                 // Log file stream
        std::streambuf *originalBuffer_;        // Original buffer of std::cout
    public:
        CoutRedirector(const std::string &logFilePath, const std::string& model)
            : logFile_(logFilePath, std::ofstream::app),
              originalBuffer_(std::cout.rdbuf()) {
            logFile_ << "----------------------- " << model << " ------------------------" << std::endl;
            std::cout.rdbuf(logFile_.rdbuf());
        }
        ~CoutRedirector() {
            std::cout.rdbuf(originalBuffer_);
        }

    };


} // namespace myTools



#endif //CPP_CODE_MYTOOLS_H
