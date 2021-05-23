#ifndef ASYNCVECTOR_H
#define ASYNCVECTOR_H

#include <vector>
#include <mutex>

template<class T>
class AsyncVector
{
public:
    AsyncVector()
    {

    }
    AsyncVector(const std::vector<T> &data):
        m_data(move(data))
    {

    }

    unsigned Size()
    {
        m_mutex.lock();

        unsigned size = m_data.size();

        m_mutex.unlock();

        return size;
    }
    void PushBack(const T& value)
    {
        m_mutex.lock();

        m_data.push_back(value);

        m_mutex.unlock();
    }
    const T& Get(unsigned id)
    {
        return m_data.at(id);
    }

    void Clear()
    {
        m_mutex.lock();
        m_data.clear();
        m_mutex.unlock();
    }

    T& At(unsigned id)
    {
        m_mutex.lock();

        T& item = m_data.at(id);

        m_mutex.unlock();
        return item;
    }


private:
    std::mutex m_mutex;
    std::vector<T> m_data;
};

#endif // ASYNCVECTOR_H
