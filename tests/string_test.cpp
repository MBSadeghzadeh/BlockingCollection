#include <gtest/gtest.h>

#include <Producer.h>
#include <Consumer.h>

#include <vector>
#include <numeric>

TEST(ProducerConsumerSuite, StringTest)
{
    using TYP1 = std::string;
    using TYP2 = int;

    BlockingCollectionPtr<TYP1> collectionPtr1 = std::make_shared<code_machina::BlockingCollection<TYP1>>(100);
    BlockingCollectionPtr<TYP2> collectionPtr2 = std::make_shared<code_machina::BlockingCollection<TYP2>>(1);

    auto consumerFunc = [](const TYP1 &data) -> TYP2
    { return data.length(); };

    double resultSum = 0;
    auto producerFunc = [&resultSum](const TYP2 &data)
    { resultSum += data; };

    std::vector<TYP1> vec{"ab cd", "ab cde", "ab cd e", "ab cd ef"};

    TYP1 stop_flag = "";
    auto stopFlagPosition = std::find(vec.begin(), vec.end(), stop_flag);
    long realSum = std::accumulate(vec.begin(), stopFlagPosition, 0l, [consumerFunc](long first, TYP1 last)
                                   { return first + consumerFunc(last); });

    auto memDataReader = getVectorDataReader<TYP1>(vec);

    Consumer<TYP1, TYP2> consumer(collectionPtr1, collectionPtr2, consumerFunc);

    Producer<TYP1, TYP2> producer(std::move(memDataReader), collectionPtr1, collectionPtr2, producerFunc, stop_flag);

    producer.join();
    consumer.join();

    EXPECT_EQ(realSum, resultSum);
}