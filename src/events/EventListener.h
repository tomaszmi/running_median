#pragma once

namespace tplx
{

template<typename T>
struct Event;

template<typename T>
class EventListener
{
public:
    virtual ~EventListener() = default;

    /**
        \brief Notifies about new event.
        \param event - details
        \return \a true if is not interested in further events any longer.
     */
    virtual bool newEvent(Event<T>&& event) = 0;
};

} // namespace tplx
