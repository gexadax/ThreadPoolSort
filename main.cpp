#include "optimized_thread.h"

int main()
{
    std::vector<int> massive(100);
    std::vector<std::vector<int>> arrays;

    for (int i = 0; i < massive.size(); ++i) {
        massive[i] = rand() % 99;
    }

    int size = massive.size();
    int i = 0;
    while (i < massive.size()) {
        if (size > 10)
        {
            std::vector<int> array;
            for (int j = 0; j < 10; ++j)
            {
                array.push_back(massive[i]);
                i++;
                size--;
            };
            arrays.push_back(array);

        }
        else
        {
            std::vector<int> array;
            for (int j = 0; j < size; ++j)
            {
                array.push_back(massive[i]);
                i++;
            };
            arrays.push_back(array);
        };

    };

    std::vector<int> massiveCopy = massive;
    auto startSingleThreaded = std::chrono::high_resolution_clock::now();
    singleThreadedSort(massiveCopy, 0, massiveCopy.size() - 1);
    auto endSingleThreaded = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationSingleThreaded = endSingleThreaded - startSingleThreaded;
    std::cout << "Single thread sorting " << durationSingleThreaded.count() << " milliseconds" << std::endl;

    RequestHandler2 rh;

    auto startMultiThreaded = std::chrono::high_resolution_clock::now();
    rh.push_task(quicksort, arrays[0], 0, arrays[0].size() - 1);
    auto endMultiThreaded = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationMultiThreaded = endMultiThreaded - startMultiThreaded;
    std::cout << "Multi threads sorting " << durationMultiThreaded.count() << " milliseconds" << std::endl;

    return 0;
}
