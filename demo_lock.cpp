#include <procon.h>

#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: provide interval(ms) for producer"
                     " and time span for running(s)"
                  << std::endl;
        return EXIT_FAILURE;
    }

    auto prod_interval = std::chrono::milliseconds(std::atoi(argv[1]));
    auto run_time = std::chrono::seconds(std::atoi(argv[2]));

    auto bq = std::make_shared<procon::BlockingQueue<procon::DataFrame>>();

    procon::Producer producer(bq, prod_interval);
    // procon::Consumer consumer(bq, [](procon::DataFrame&) {});
    procon::Consumer consumer(
        bq, [](procon::DataFrame&) { std::this_thread::sleep_for(5ms); });

    std::thread prod_thr(&procon::Producer::run, &producer);
    std::thread cons_thr(&procon::Consumer::run, &consumer);

    std::thread watcher_thr([&] {
        std::this_thread::sleep_for(run_time);
        producer.stop();
        consumer.stop();
    });

    prod_thr.join();
    std::cout << "Producer shut down" << std::endl;
    cons_thr.join();
    std::cout << "Consumer shut down" << std::endl;
    watcher_thr.join();
    std::cout << "System shut down" << std::endl;
    return EXIT_SUCCESS;
}
