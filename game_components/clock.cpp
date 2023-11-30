#include "clock.h"

namespace Components {
    Time::Time(int t){
        hours = t / 3600;
        t %= 3600;
        minutes = t / 60;
        t %= 60;
        seconds = t;
    }

    int Time::getTimeInSeconds() const{
        return hours * 3600 + minutes * 60 + seconds;
    }

    void Time::formTime(){
        if (hours != 0 && minutes == 0){
            hours--;
            minutes = 60;
        }
        if (minutes != 0 && seconds == 0){
            minutes--;
            seconds = 60;
        }
        if (minutes > 60){
            hours++;
            minutes -= 60;
        }
        if (seconds > 60){
            minutes++;
            seconds -= 60;
        }
    }

    bool Time::operator==(int value) const{
        return (seconds + minutes * 60 + hours * 3600) == value;
    }

    std::ostream &operator<<(std::ostream &os, const Time &time1){
        os << "Hours: " << time1.hours << "; Minutes: " << time1.minutes << "; Seconds: " << time1.seconds;
        return os;
    }

    void Clock::increaseTime1(){
        lock_guard<mutex> lockS(clockTimeMutex);
        time1.seconds += increment;
        time1.formTime();
    }

    void Clock::increaseTime2(){
        lock_guard<mutex> lockS(clockTimeMutex);
        time2.seconds += increment;
        time2.formTime();
    }

    void Clock::decreaseTime(int &result){
        int chunk1 = 0, chunk2 = 0;
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
        double sleep_for;
        while (true){
            if (flag){
                start = std::chrono::high_resolution_clock::now();
                chunk1++;
                if (chunk1 == 10){
                    lock_guard<mutex> lockT(clockTimeMutex);
                    chunk1 = 0;
                    time1.formTime();
                    time1.seconds--;
                    if (time1 == 0){
                        lock_guard<mutex> lockR(clockResMutex);
                        result = 1;
                        return;
                    }
                }
                end = std::chrono::high_resolution_clock::now();
                sleep_for = 0.1 - static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()) / 1e9;
                std::this_thread::sleep_for(std::chrono::duration<double>(sleep_for));
            } else {
                start = std::chrono::high_resolution_clock::now();
                chunk2++;
                if (chunk2 == 10){
                    lock_guard<mutex> lockT(clockTimeMutex);
                    chunk2 = 0;
                    time2.formTime();
                    time2.seconds--;
                    if (time2 == 0){
                        lock_guard<mutex> lockR(clockResMutex);
                        result = 2;
                        return;
                    }
                }
                end = std::chrono::high_resolution_clock::now();
                sleep_for = 0.1 - static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()) / 1e9;
                std::this_thread::sleep_for(std::chrono::duration<double>(sleep_for));
            }
            if (stopFlag){
                return;
            }
        }
    }

    void Clock::clockFlagFlip(){
        flag = !flag;
    }

    void Clock::startThread(int &result){
        clockThread = make_shared<thread>([this, &result]{
            decreaseTime(result);
        });
    }

    void Clock::stopThread(){
        stopFlag = true;
        if (clockThread && clockThread->joinable()){
            clockThread->join();
        }
    }
}