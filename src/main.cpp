#include "Producer.h"
#include "Consumer.h"

#include <array>

int main() {
    using TYP1 = int;
    using TYP2 = float;

    // A bounded collection. It can hold no more than 1000 items at once.
    BlockingCollectionPtr<TYP1> collectionPtr1 = std::make_shared<code_machina::BlockingCollection<TYP1>>(1000);
    BlockingCollectionPtr<TYP2> collectionPtr2 = std::make_shared<code_machina::BlockingCollection<TYP2>>(1000);

    std::unique_ptr< PromptDataReader<TYP1>> promptDataReader = std::make_unique<PromptDataReader<TYP1>>();

    std::array<TYP1, 3> array = {1,2,3};
    std::unique_ptr<MemoryDataReader<TYP1,std::array<TYP1,3>>> memDataReader = std::make_unique<MemoryDataReader<TYP1, std::array<TYP1,3>>>(array);

    auto processFunction = [](const TYP1 &data)->TYP2 { return data * 3;};

    Consumer<TYP1, TYP2> consumer(collectionPtr1, collectionPtr2, processFunction);
    Consumer<TYP1, TYP2> consumer2(collectionPtr1, collectionPtr2, processFunction);

    auto displayFunction = [](const TYP2 &data){ std::cout << "producer*2(" << std::this_thread::get_id() << "):" << data << std::endl;};

    Producer<TYP1, TYP2> producer(std::move(promptDataReader),collectionPtr1,collectionPtr2, displayFunction, 0);

    producer.join();
    consumer.join();
    consumer2.join();

    return 0;
}