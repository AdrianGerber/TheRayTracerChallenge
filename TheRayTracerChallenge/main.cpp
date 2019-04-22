#include "main.h"
#include "Experiments.h"

#include <chrono>

int main(void) {

	auto begin = std::chrono::high_resolution_clock::now();
	DrawChapter7Scene();
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us";

    std::cout << std::endl;
    char c;
    std::cin >> c;
    
    return 0;
}

