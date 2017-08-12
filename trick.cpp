#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <assert.h>
#include <vector>

int main () {
	std::ifstream input;
	input.open("input.txt");

	unsigned short numHomes;
	input >> numHomes;
	assert(numHomes < 10000);
	printf("homes: %d\n", numHomes);

	unsigned short maxCandies;
	input >> maxCandies;
	assert(maxCandies < 1000);
	printf("maxCandies: %d\n", maxCandies);

	std::vector<unsigned int> homeCandyDirectory;

	unsigned short num; 
	for(unsigned short hIdx = 0; 
		hIdx < numHomes && !input.eof(); 
		++hIdx)
	{
		input >> num;
		std::cout << num << std::endl;
		assert(num < 1000);
		homeCandyDirectory.push_back(num);
	}

//	for(unsigned short hIdx = 0;
//		hIdx < homeCandyDirectory.size();
//		++hIdx)
//	{
//		printf("%d: %d\n", hIdx+1, homeCandyDirectory[hIdx]);
//	}

	input.close();
}
