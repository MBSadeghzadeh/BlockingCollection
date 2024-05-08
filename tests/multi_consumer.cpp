#include <gtest/gtest.h>

#include <Producer.h>
#include <Consumer.h>

#include <array>
#include <random>
#include <vector>

namespace
{
    int randNumber()
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        // Define a distribution (e.g., uniform distribution from 1 to 100)
        std::uniform_int_distribution<> dis(1, 10);

        int random_number = dis(gen);

        return random_number;
    }

    std::vector<size_t> random_vector_size = {1, 10, 100, 1'000, 10'000, 100'000, 1'000'000};
}

class MultiConsumerSuite : public testing::TestWithParam<size_t>
{
};

TEST_P(MultiConsumerSuite, MultiConsumer)
{
    const size_t size = GetParam();

    using TYP1 = int;
    using TYP2 = float;

    std::vector<TYP1> vec;
    for (size_t i = 0; i < size; i++)
    {
        vec.push_back(randNumber());
    }

    BlockingCollectionPtr<TYP1> collectionPtr1 = std::make_shared<code_machina::BlockingCollection<TYP1>>(100);
    BlockingCollectionPtr<TYP2> collectionPtr2 = std::make_shared<code_machina::BlockingCollection<TYP2>>(1);

    auto processFunction = [](const TYP1 &data) -> TYP2
    { return data * 3; };

    auto zeroPosition = std::find(vec.begin(), vec.end(), 0);
    double realSum = std::accumulate(vec.begin(), zeroPosition, 0.0, [processFunction](double first, TYP1 last)
                                     { return first + processFunction(last); });

    std::unique_ptr<MemoryDataReader<TYP1, std::vector<TYP1>>> memDataReader =
        std::make_unique<MemoryDataReader<TYP1, std::vector<TYP1>>>(vec);

    double resultSum = 0;
    auto displayFunction = [&resultSum](const TYP2 &data)
    { resultSum += data; };

    Consumer<TYP1, TYP2> consumer1(collectionPtr1, collectionPtr2, processFunction);
    Consumer<TYP1, TYP2> consumer2(collectionPtr1, collectionPtr2, processFunction);
    Consumer<TYP1, TYP2> consumer3(collectionPtr1, collectionPtr2, processFunction);
    Consumer<TYP1, TYP2> consumer4(collectionPtr1, collectionPtr2, processFunction);

    Producer<TYP1, TYP2> producer(std::move(memDataReader), collectionPtr1, collectionPtr2, displayFunction, 0);

    producer.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();

    std::cout << "DataDrivenTest: " << resultSum << " == " << realSum << std::endl;

    EXPECT_EQ(realSum, resultSum);
}

INSTANTIATE_TEST_SUITE_P(ProducerConsumer, MultiConsumerSuite, testing::ValuesIn(random_vector_size));