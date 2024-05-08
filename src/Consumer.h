#pragma once

#include "Common.h"

#include <chrono>
#include <functional>

template <class T, class R>
class Consumer
{
    using ProcessFunction = std::function<R(const T &)>;

private:
    BlockingCollectionPtr<T> m_collection1;
    BlockingCollectionPtr<R> m_collection2;
    ProcessFunction m_processFunction;
    std::thread m_thread;

    void consume()
    {
        while (!m_collection1->is_completed())
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(0s);

            T data;

            // take will block if there is no data to be taken
            auto status = m_collection1->take(data);
            if (status == code_machina::BlockingCollectionStatus::Ok)
            {
                // blocks if collection.size() == collection.bounded_capacity()
                m_collection2->add(m_processFunction(data) /*data * 2*/);

                //std::cout << "consumer(" << std::this_thread::get_id() << "):" << data << std::endl;
            }
        }

        m_collection1->detach_consumer();
        //m_collection2->complete_adding();
        m_collection2->detach_producer();
    }

public:
    Consumer(BlockingCollectionPtr<T> collection1, 
    BlockingCollectionPtr<R> collection2,
    const ProcessFunction &processFunction) : 
    m_collection1{collection1},
    m_collection2{collection2},
    m_processFunction{processFunction}
    {
        m_thread = std::thread([this]
                               {
            m_collection1->attach_consumer();
            m_collection2->attach_producer();
            this->consume(); });
    }

    void join()
    {
        m_thread.join();
    }
};
