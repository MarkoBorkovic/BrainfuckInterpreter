#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <stack>

#define BYTE unsigned char
#define MEMSIZE 30000

using namespace std;

struct _unbalancedbracketex : public exception {
	const char* what() const throw() {
		return "Unbalanced bracket!";
	}
}unbalancedbracketex;

struct _outofboundsex : public exception {
	const char* what() const throw() {
		return "Program went out of interpreter memory bounds!";
	}
}outofboundsex;

string cleanup(string original) {
	string clean;
	for(int i = 0; i < original.size(); i++){
		switch(original[i]) {
		case '>':
		case '<':
		case '+':
		case '-':
		case ',':
		case '.':
		case '[':
		case ']':
			clean += original[i];
		default:
			break;
		}
	}
	return clean;
}

void interpret(string source) {
	BYTE memory[MEMSIZE] = {0};
	BYTE* ptr = memory;
	int insptr = 0;

	map<int, int> bracketmap;
	stack<int> bracketstack;
	while(insptr < source.size()) {
		if(source[insptr] == '['){
			bracketstack.push(insptr);
		}else if(source[insptr] == ']') {
			if(bracketstack.empty())
				throw unbalancedbracketex;

			bracketmap.insert(pair<int, int>(insptr, bracketstack.top()));
			bracketmap.insert(pair<int, int>(bracketstack.top(), insptr));
			bracketstack.pop();
		}
		insptr++;
	}
	if(!bracketstack.empty())
		throw unbalancedbracketex;

	insptr = 0;
	while(insptr < source.size() && source[insptr] != '\0') {
		switch(source[insptr]) {
		case '>':
			ptr++;
			if(ptr > memory + MEMSIZE - 1)
				throw outofboundsex;
			break;
		case '<':
			ptr--;
			if(ptr < memory)
				throw outofboundsex;
			break;
		case '+':
			(*ptr)++;
			break;
		case '-':
			(*ptr)--;
			break;
		case '.':
			putchar(*ptr);
			break;
		case ',':
			*ptr = getchar();
			break;	
		case '[':
			if(*ptr == 0) 
				insptr = bracketmap[insptr];
			break;
		case ']':
			if(*ptr != 0)
				insptr = bracketmap[insptr];
			break;
		}
		insptr++;
	}
}

int main(int argc, char** argv) {
	
	if(argc < 2) {
		cout << "No file provided!" << endl;
		return 1;
	}

	ifstream sourcefile;
	sourcefile.open(argv[1]);
	if(!sourcefile.is_open()) {
		cout << "Error opening source file!" << endl;
		return 1;
	}
	stringstream strStream;
	strStream << sourcefile.rdbuf();
	string source = strStream.str();
	sourcefile.close();

	source = cleanup(source);
	try {
		interpret(source);
	} catch(_outofboundsex e) {
		cout << "Error! " << e.what() << endl;
	} catch(_unbalancedbracketex e) {
		cout << "Error! " << e.what() << endl;
	}

	return 0;
}