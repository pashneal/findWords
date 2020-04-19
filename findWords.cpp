#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>

const int rows = 4;
const int cols = 5;
const int wordLimit = 8;
std::vector <std::pair<int , int >> newDirections = {{-1,0}, {0,-1}, {0,1}, {1,0}};

std::vector <char> randomChoice(std::string letters, int numChoices){
	std::vector <char> randomVector;
	while (numChoices--) {
		randomVector.push_back(letters[rand() % letters.size()]);
	}
	return randomVector;
}

class Dictionary{
		std::unordered_map <std::string, std::unordered_set<std::string>> words;
	public:
		void insert(std::string word){
			std::string potentialWord = word;
			std::sort(potentialWord.begin(), potentialWord.end());
			words[potentialWord].insert(word);
		};
		bool isWord(std::string word){
			std::string potentialWord = word;
			std::sort(potentialWord.begin(), potentialWord.end());
			if (words.find(potentialWord) != words.end()) 
				return (words[potentialWord].find(word) != words[potentialWord].end());
			return false;
		}
};

int findIndex(std::pair<int, int> currentPosition) {
	return (currentPosition.first + currentPosition.second*cols);
}

//returns a list of words that can be found by moving
// horizontally or vertically across the given vector
std::vector <std::string> solvePuzzle (std::vector<char>& puzzle, Dictionary& dic, std::unordered_set<int>& visited,std::pair<int, int> currentPos, std::string currWord) {
	std::vector<std::string> solutions;
	if (dic.isWord(currWord)) solutions.push_back(currWord);
	if (currWord.size() == wordLimit) return solutions;
	for (auto dxdy : newDirections){

		int dx = dxdy.first;
		int dy = dxdy.second;

		auto newPos = std::make_pair(currentPos.first + dx, currentPos.second + dy);

		if (visited.find(findIndex(newPos)) != visited.end()) continue;
		
		if (newPos.first >= 0 && newPos.first < cols
			&& newPos.second >= 0 && newPos.second < rows){

			visited.insert(findIndex(currentPos));
			for (std::string solution: solvePuzzle(puzzle, dic, visited, newPos, currWord+puzzle[findIndex(newPos)])){
				solutions.push_back(solution);
			}
			visited.erase(findIndex(currentPos));
		}
	}

	return solutions;
}


int main() {
	std::ifstream words;
	words.open("ukenglish.txt",std::ios::in);
	std::string input;
	Dictionary dictionary;
	while (getline(words, input)) {

		//the dictionary file that I am using has comments
		if (input.size() > 0 && input[0] != '#') {
			dictionary.insert(input);
		}
	}
	//do not include very uncommon letters
	std::string alphabet  = "aaaaaaaaaaaaaaaaabbbbcddddddeeeeeeeeeffffffggghhhhhiiiiiiilmnnnnnnnooooooooppprssssssssssssttttttttttttttttuuuuuuuuuwwwyyyy";


	srand(time(NULL));
	std::vector <char> puzzle =  randomChoice(alphabet, cols*rows);
	std::vector <std::string> solutions;
	std::unordered_set<int> visited =  {};
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (auto solution : solvePuzzle(puzzle, dictionary, visited, std::make_pair(j,i), ""))
				solutions.push_back(solution);
		}
	}

	for (int i = 0 ; i < rows; i++) {
		for (int j = 0 ; j < cols; j++) {
			std::cout << puzzle[j + i*cols] << " ";
		}
		std::cout << std::endl;
	}

	for (auto sol: solutions) {
		if (sol.size() < 2) continue;
		std::cout << sol << std::endl;
	}
}
