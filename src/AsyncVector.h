#ifndef ASYNCVECTOR_H
#define ASYNCVECTOR_H

#include <vector>
#include <pthread.h>


// Вектор с возможностью асинхронного чтения\записи
template<class T>
class AsyncVector
{
public:
    AsyncVector()
    {
        pthread_mutex_init(&m_mutex, nullptr);
    }
    AsyncVector(const std::vector<T> &data):
        m_data(move(data))
    {
        pthread_mutex_init(&m_mutex, nullptr);
    }
    ~AsyncVector()
    {
        Clear();
        pthread_mutex_destroy(&m_mutex);
    }

    unsigned Size()
    {
        pthread_mutex_lock(&m_mutex);

        unsigned size = m_data.size();

        pthread_mutex_unlock(&m_mutex);

        return size;
    }
    void PushBack(const T& value)
    {
        pthread_mutex_lock(&m_mutex);

        m_data.push_back(value);

        pthread_mutex_unlock(&m_mutex);
    }
    const T& Get(unsigned id)
    {
        return m_data.at(id);
    }

    void Clear()
    {
        pthread_mutex_lock(&m_mutex);
        m_data.clear();
        pthread_mutex_unlock(&m_mutex);
    }

    T& At(unsigned id)
    {
        pthread_mutex_lock(&m_mutex);

        T& item = m_data.at(id);

        pthread_mutex_unlock(&m_mutex);
        return item;
    }


private:
    pthread_mutex_t m_mutex;
    std::vector<T> m_data;
};

#endif // ASYNCVECTOR_H
