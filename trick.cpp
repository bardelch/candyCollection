#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <assert.h>
#include <vector>
#include <omp.h>


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
	
//		printf("localMax: %d => %d: %d\n", 
//			houseStart, houseEnd-1, numCandies);
		//found a new max; save
		if(numCandies > targetCandies){
//			printf("updating max candies from %d => %d\n", 
//				targetCandies, numCandies);
			targetStart = houseStart; targetEnd = houseEnd-1; 
				targetCandies = numCandies;
		}

		//advance the start of the window
		//subtract from ongoing sum
		numCandies -= homeCandyDirectory[houseStart];
//		if(houseStart < numHomes-1){
//			printf("advance window: %d => %d: %d\n", houseStart+1, houseEnd-1,
//				numCandies);
//		}
	}

//	//found solution print success and break;
//	if(targetCandies > 0){
//		printf("Start at home %d and go to home %d ",
//			targetStart+1, targetEnd+1);
//		printf("getting %d pieces of candy\n",
//			targetCandies);
//	}else{
//		printf("Don't go here\n");
//	}

//	printf("====\n");
//	houseStart = houseEnd = numCandies = 0; 
//	targetStart = targetEnd = targetCandies = 0;
	unsigned long long max_val = 0; //64 bits

#pragma omp parallel for \
	shared(homeCandyDirectory) reduction(max : max_val)
	for(unsigned short idxStart = 0; idxStart  < numHomes; ++idxStart){
//		printf("hello from thread: %d\n", omp_get_thread_num());
		unsigned short idxRunner = 0, candyCount = 0;

		for(idxRunner = idxStart; 
			idxRunner < numHomes && //stop at end of hosue list
			//do not add if it will push it over the max
			candyCount + homeCandyDirectory[idxRunner] <= maxCandies;
			++idxRunner)
		{
			candyCount += homeCandyDirectory[idxRunner];
//			printf("idxStart = %d ", idxStart);
//			printf("[%d]:%d => %d\n", idxRunner, 
//				homeCandyDirectory[idxRunner], candyCount);
		}

//it has stepped one index over; not needed for next loop, so reset
		--idxRunner; 
	
//		printf("idxStart = %d, idxRunner = %d; candyCount = %d\n",
//			idxStart, idxRunner, candyCount);

		//compact single structure; using 3*16 (48) of 64 bits
		//leading candy count will not go above max count due to above loop
		unsigned long long val = candyCount;
		val <<= 16; //size of short
		val |= idxStart;
		val <<= 16;
		val |= idxRunner;

		if(val > max_val){
			max_val = val;
		}
	} //end of OMP parallel section

	//upack variables
	targetEnd = max_val & 0xff;
	max_val >>= 16;
	targetStart = max_val & 0xff;
	max_val >>= 16;
	targetCandies = max_val;

//	printf("start = %d, end = %d, candies = %d\n", 
//		targetStart, targetEnd, targetCandies);
	if(targetCandies > 0){
		printf("Start at home %d and go to home %d ",
			targetStart+1, targetEnd+1);
		printf("getting %d pieces of candy\n",
			targetCandies);
	}else{
		printf("Don't go here\n");
	}

}
