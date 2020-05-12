#include "RingBufferResizable.h"

template<class T>
RingBufferResizable<T>::RingBufferResizable(size_t size) :
    ringBuffer(std::unique_ptr<T[]>(new T[size])),
    maxSize(size)
{
    head = 0;
    tail = 0;
    full = false;
}

template<class T>
void RingBufferResizable<T>::push(T value)
{
    ringBuffer[head] = value;
    head = (head + 1) % maxSize;

    if (full)
    {
        tail = (tail + 1) % maxSize;
    }
    full = head == tail ? true : false;
}

template<class T>
T RingBufferResizable<T>::get()
{
    if (!isEmpty())
    {
        size_t tempTail = tail;
        tail = (tail + 1) % maxSize;
        full = false;
        return ringBuffer[tempTail];
    }
    return T();
}

template<class T>
void RingBufferResizable<T>::pop_front()
{
    if (!isEmpty())
    {
        ringBuffer[tail] = T();
        tail = (tail + 1) % maxSize;

        if (full) { full = false; }
    }
}

template<class T>
void RingBufferResizable<T>::pop_back()
{
    if (!isEmpty())
    {
        head = (head == 0) ? maxSize - 1 : head - 1;
        ringBuffer[head] = T();
        if (full) { full = false; }
    }
}

template<class T>
void RingBufferResizable<T>::resize(size_t newSize)
{
    if (newSize == maxSize) { return; }
    size_t tempValue = newSize < maxSize ? newSize : maxSize; // select min value to create temp RB for copy
    full = newSize > maxSize ? false : full; // if new size more than former, we need to check that full variable is correct
    maxSize = newSize;
    head = newSize > head ? head : newSize - 1;
    tail = newSize > tail ? tail : newSize - 1;
    std::unique_ptr<T[]> tempRingBuffer(new T[tempValue]);

    for (size_t i = 0; i != tempValue; ++i)
    {
        tempRingBuffer[i] = ringBuffer[i];
    }
    ringBuffer.reset(new T[newSize]);

    for (size_t i = 0; i != tempValue; ++i)
    {
        ringBuffer[i] = tempRingBuffer[i];
    }
    tempRingBuffer.reset();
}

template<class T>
void RingBufferResizable<T>::clear()
{
    ringBuffer.reset(new T[maxSize]);
    head = 0;
    tail = 0;
    full = false;
}

template<class T>
bool RingBufferResizable<T>::isEmpty() const
{
    return (!full && (head == tail));
}

template<class T>
bool RingBufferResizable<T>::isFull() const
{
    return full;
}

template<class T>
size_t RingBufferResizable<T>::getMaxSize() const
{
    return maxSize;
}

template<class T>
size_t RingBufferResizable<T>::sizeQueue() const
{
    size_t size = maxSize;

    if (!full)
    {
        if (head >= tail)
        {
            size = head - tail;
        }
        else
        {
            size = maxSize + head - tail;
        }
    }
    return size;
}