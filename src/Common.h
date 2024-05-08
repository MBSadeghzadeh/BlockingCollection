#pragma once

#include <third_party/BlockingCollection/BlockingCollection.h>

#include <memory>

template<class T>
using BlockingCollectionPtr = std::shared_ptr<code_machina::BlockingCollection<T, code_machina::QueueContainer<T>>>;