## Dependencies
- BlockingCollection
- gtest

## Configure
- cmake -S . -B ./build -DTESTS_ENABLED=ON

## Build
- cmake --build ./build

## Run app
- ./build/ProducerConsumer

## Run app with an input file
- cat sample-input.txt  | ./build/ProducerConsumer

## Run Tests
- cd ./build/tests && ctest