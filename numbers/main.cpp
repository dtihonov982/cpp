#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int pow10(int i) {
	int result = 1;
	while(i--) result *= 10;
	return result;
}

bool isPalindrome(int x) {

	//Counting digits in x
	int length = 1;
	//Each iteration cut last digit
	int temp = x;
	while(temp /= 10) {
		length++;
	}

	//Comparing mirror pairs of digits
	for(int i = 1; i <= length/2; i++) {
		if(
			(x / pow10(i - 1) % 10) != (x / pow10(length - i) % 10)
		) return false;
	}
	return true;
}



int getEquivalent(char symbol) {
	switch(symbol) {
	case 'I': return 1;
	case 'V': return 5;
	case 'X': return 10;
	case 'L': return 50;
	case 'C': return 100;
	case 'D': return 500;
	case 'M': return 1000;
	}
	return 0;
}

int romanToInt(string s) {
	int result = 0;
	int current, next;
	int length = s.length();
	for(int i = 0; i < length - 1; i++) {
		current = getEquivalent(s[i]);
		next = getEquivalent(s[i + 1]);
		if (current >= next)
			result += current;
		else
			result -= current;
	}
	result += getEquivalent(s[length - 1]);
	return result;
}


class Converter {
public:
	Converter(initializer_list<char> lst): literals(lst), base(lst.size()) {
	}
	string convert(int number) {
		int current;
		string result;
		do {
			current = number % base;
			result.push_back(literals[current]);
			number = number / base;
		} while (number > 0);
		reverse(result.begin(), result.end());
		return result;
	}
private:
	vector<char> literals;
	int base;
};


int main() {
	Converter excel = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
	 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
	  'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
	Converter hex = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	cout << excel.convert(26);
	return 0;
}
