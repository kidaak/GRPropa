#ifndef GRPROPA_CLOCK_H
#define GRPROPA_CLOCK_H

namespace grpropa {

//class ClockImpl;

class Clock {
private:
    class Impl;
    Impl *impl;
public:
    Clock();
    virtual ~Clock();

    void reset();
    double getSecond();
    double getMillisecond();
    static Clock &getInstance();
};

} // namespace grpropa

#endif // GRPROPA_CLOCK_H
