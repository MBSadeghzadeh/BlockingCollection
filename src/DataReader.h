#pragma once

#include <iostream>
#include <vector>
#include <memory>

template<class T>
class DataReader {
    public:
    DataReader() = default;

    virtual T getNext() = 0;
    virtual bool eof() = 0;
};

template<class T>
class PromptDataReader: public DataReader<T>
{
    public:
    PromptDataReader() = default;

    T getNext() override {
        T input;

        std::cin >> input;

        return input;
    }

    bool eof() override {
        return std::cin.eof();
    }
};

template<class T, class MemCollection> 
class MemoryDataReader: public DataReader<T>
{
private:
    size_t m_current = 0;
    MemCollection m_memCollection;

public:
    MemoryDataReader(const MemCollection &memCollection): 
    m_memCollection {memCollection}
    {

    }

    T getNext() override {
        return m_memCollection[m_current++];
    }

    bool eof() {
        return m_current == m_memCollection.size();
    }
};

template<class T>
auto getVectorDataReader(const std::vector<T>& vec)
{
    using VectorDataReader = MemoryDataReader<T, std::vector<T>>;
    std::unique_ptr<VectorDataReader> memDataReader = std::make_unique<VectorDataReader>(vec);
    return memDataReader;
}