#include <gtest/gtest.h>

#include <Producer.h>
#include <Consumer.h>

#include <vector>
#include <numeric>

std::vector<std::vector<int>> testVectors = {
    {},
    {1},
    {1, 2, 0, 4, 5, 6, 7},
    {-1, -2, -3, 0, 2},
    {-1, 2, -3, 4, 0},
    {0, 1, 2}};

class ProducerConsumerSuite : public testing::TestWithParam<std::vector<int>>
{
};

TEST_P(ProducerConsumerSuite, DataDrivenTest)
{
    const std::vector<int> &vec = GetParam();

    using TYP1 = int;
    using TYP2 = float;

    BlockingCollectionPtr<TYP1> collectionPtr1 = std::make_shared<code_machina::BlockingCollection<TYP1>>(1000);
    BlockingCollectionPtr<TYP2> collectionPtr2 = std::make_shared<code_machina::BlockingCollection<TYP2>>(1000);

    auto processFunction = [](const TYP1 &data) -> TYP2
    { return data * 3; };

    TYP1 stop_flag = 0;

    auto zeroPosition = std::find(vec.begin(), vec.end(), stop_flag);
    double realSum = std::accumulate(vec.begin(), zeroPosition, 0.0, [processFunction](double first, TYP1 last)
                                     { return first + processFunction(last); });

    using VectorIntDataReader = MemoryDataReader<TYP1, std::vector<TYP1>>;
    std::unique_ptr<VectorIntDataReader> memDataReader = std::make_unique<VectorIntDataReader>(vec);

    double resultSum = 0;
    auto displayFunction = [&resultSum](const TYP2 &data)
    { resultSum += data; };

    Consumer<TYP1, TYP2> consumer(collectionPtr1, collectionPtr2, processFunction);
    Producer<TYP1, TYP2> producer(std::move(memDataReader), collectionPtr1, collectionPtr2, displayFunction, stop_flag);

    producer.join();
    consumer.join();

    std::cout << "DataDrivenTest: " << resultSum << " == " << realSum << std::endl;

    EXPECT_EQ(realSum, resultSum);
}

INSTANTIATE_TEST_SUITE_P(ProducerConsumer, ProducerConsumerSuite, testing::ValuesIn(testVectors));