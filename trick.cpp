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
//	printf("homes: %d\n", numHomes);

	unsigned short maxCandies;
	input >> maxCandies;
	assert(maxCandies < 1000);
//	printf("maxCandies: %d\n", maxCandies);

	std::vector<unsigned int> homeCandyDirectory;

	unsigned short num; 
	for(unsigned short hIdx = 0; 
		hIdx < numHomes && !input.eof(); 
		++hIdx)
	{
		input >> num;
//		std::cout << num << std::endl;
		assert(num < 1000);
		homeCandyDirectory.push_back(num);
	}
	input.close();

//	for(unsigned short hIdx = 0;
//		hIdx < homeCandyDirectory.size();
//		++hIdx)
//	{
//		//printf("%d: %d\n", hIdx+1, homeCandyDirectory[hIdx]);
//		printf("%d: %d\n", hIdx, homeCandyDirectory[hIdx]);
//	}

	unsigned short houseStart = 0, houseEnd = 0, numCandies = 0, 
		targetStart = 0, targetEnd = 0, targetCandies = 0;

	//solution 1: beginning and end pointers (zero-based) 
	// try to maintain maxCandies or less by adding upto but not
	// over maxCandies; subtract from head house to maintain 
	// contigious 'window' of houses
	for(houseStart = 0; houseStart < numHomes; ++houseStart){
	
		//1: may start in middle of loop (OMP) assumes 
		//	contigious outer loop segment from here
		// TODO random start points
		//2: houseEnd add loop may have aborted due to house candy > max
		if(houseStart > houseEnd){houseEnd = houseStart;}
	
		//can only add from more houses if there are more houses
		//can only add if we are at or under the max
		//can only add if the next house will not push over the max
		while(houseEnd < numHomes && numCandies <= maxCandies
				&& numCandies + homeCandyDirectory[houseEnd] <= maxCandies)
		{
			numCandies += homeCandyDirectory[houseEnd];
//			printf("[%d]:%d => %d\n", houseEnd, homeCandyDirectory[houseEnd],
//				numCandies);
			++houseEnd;
		}
	
//		printf("localMax: %d => %d: %d\n", houseStart, houseEnd-1, numCandies);
		//found a new max; save
		if(numCandies > targetCandies){
//			printf("updating max candies from %d => %d\n", targetCandies, numCandies);
			targetStart = houseStart; targetEnd = houseEnd-1; targetCandies = numCandies;
		}

		//advance the start of the window
		//subtract from ongoing sum
		numCandies -= homeCandyDirectory[houseStart];
//		if(houseStart < numHomes-1){
//			printf("advance window: %d => %d: %d\n", houseStart+1, houseEnd-1,
//				numCandies);
//		}
	}

	//found solution print success and break;
	if(targetCandies > 0){
		printf("Start at home %d and go to home %d getting %d pieces of candy\n",
			targetStart+1, targetEnd+1, targetCandies);
	}else{
		printf("Don't go here\n");
	}


}
