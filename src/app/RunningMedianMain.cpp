#include "median/MedianCalculator.h"
#include "events/EventLoop.h"
#include <iostream>

namespace tplx
{

class MedianCalculatingEventListener : public EventListener<int>
{
public:
    explicit MedianCalculatingEventListener(std::ostream& out) : out_{out} {}
    bool newEvent(Event<int>&& event) override
    {
        switch(event.type)
        {
            case EventType::new_value:
            {
                calculator_.add(event.value);
                break;
            }
            case EventType::calculate_median:
            {
                out_ << calculator_.calculate() << ' ';
                break;
            }
            case EventType::sequence_end:
            {
                calculator_.reset();
                break;
            }
        }
        return true;
    }

private:
    std::ostream& out_;
    MedianCalculator<int> calculator_; 
};

} // namespace tplx

using namespace tplx;

int main(int, char**)
{
    try
    {
        MedianCalculatingEventListener listener{std::cout};
        runEventLoop(std::cin, listener);
        std::cout << std::endl;
        return 0;
    }
    catch(std::exception& e)
    {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "[ERROR] unknown error" << std::endl;
    }
    return 1;
}
