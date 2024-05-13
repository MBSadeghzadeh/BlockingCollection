#pragma once

#include "Common.h"
#include "DataReader.h"

#include <memory>
#include <thread>
#include <chrono>
#include <functional>

template <class T, class R>
class Producer
{
    using DisplayFunction = std::function<void(const R &r)>;

public:
    Producer(std::unique_ptr<DataReader<T>> dataReader,
             BlockingCollectionPtr<T> collection1,
             BlockingCollectionPtr<R> collection2,
             const DisplayFunction &displayFunction,
             const T &stopValue) : m_dataReader{std::move(dataReader)},
                                   m_collection1{collection1},
                                   m_collection2{collection2},
                                   m_stopValue{stopValue},
                                   m_displayFunction{displayFunction}
    {
        m_thread = std::thread([this]()
                               {
                               m_collection1->attach_producer();
                               m_collection2->attach_consumer();
                               this->produce(); });
    }

    void join()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

private:
    std::unique_ptr<DataReader<T>> m_dataReader;
    BlockingCollectionPtr<T> m_collection1;
    BlockingCollectionPtr<R> m_collection2;
    DisplayFunction m_displayFunction;
    const T m_stopValue;
    std::thread m_thread;

    void produce();
    auto takeOneDataFromCollection2();
};

template <class T, class R>
void Producer<T, R>::produce()
{
    T input;
    do
    {
        if (!m_dataReader->eof())
        {
            input = m_dataReader->getNext();

            if (input != m_stopValue)
            {
                // blocks if collection.size() == collection.bounded_capacity()
                m_collection1->add(input);

                takeOneDataFromCollection2();
            }
        }
        else
        {
            input = m_stopValue;
        }

    } while (input != m_stopValue);

    m_collection1->complete_adding();

    m_collection1->detach_producer();
    m_collection2->detach_consumer();
}

template <class T, class R>
auto Producer<T, R>::takeOneDataFromCollection2()
{
    R data;
    auto status = m_collection2->take(data);
    if (status == code_machina::BlockingCollectionStatus::Ok)
    {
        m_displayFunction(data);
    }
    return status;
}