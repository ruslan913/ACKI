#include <iostream>
#include <string>
#include <cstdlib>
#include <list>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <fstream>
#include <stdexcept>
#include <stdio.h>

using namespace std;

class Uz {
public:
    int key;
	char s;
	Uz* L, * R;
	Uz() { L = NULL; R = NULL; }
    Uz(Uz* second){
        key = second->key;
        s = second->s;
        L = second->L;
        R = second->R;
    }

	Uz(Uz* l, Uz* r)
	{
		L = l;
		R = r;
		key = l->key + r->key;
	}

	~Uz()
	{
		while (L)
			delete L;
		while (R)
			delete R;
		L = NULL;
		R = NULL;
	}


};

struct Sort
	{
		bool operator() (const Uz* l, const Uz* r)
		{
			return l->key < r->key;
		}
	};

Uz *builder(list<Uz*> leafs) {
    while (leafs.size() != 1)
	{
		leafs.sort(Sort());
		Uz* Left = leafs.front();
		leafs.pop_front();
		Uz* Right = leafs.front();
		leafs.pop_front();
		Uz* pr = new Uz(Left, Right);
		leafs.push_back(pr);
	}
    return new Uz(leafs.front());
}

void huffmanCodes(Uz *root, vector<bool> &code, map<char, vector<bool> > &huffmanCode) {
    if(root -> L){
        code.push_back(0);
        huffmanCodes(root ->L, code, huffmanCode);
    }
    if(root -> R){
        code.push_back(1);
        huffmanCodes(root ->R, code, huffmanCode);
    }

    if (!root -> L && !root ->R) {
        huffmanCode[root -> s] = code;
    }

    if(!code.empty())
        code.pop_back();
}

void decod(const char* input_name="codetext.txt", const char* output_name="output.txt") {
    map<char, int> alfabet;

    ifstream input_file(input_name, std::ios::binary);
    if (!input_file.is_open()) {
        throw invalid_argument("File not found.");
    }
    int col_letters = 0, col = 0;
    char character;
    input_file.read((char*)&col_letters, sizeof(col_letters));

    while (col_letters != 0) {
        input_file.read((char*)&character, sizeof(character));
        input_file.read((char*)&col, sizeof(col));

        alfabet[character] = col;
        cout << character << ':' << alfabet[character] << endl;
        col_letters -= 40;
    }
    list<Uz*> tree;
    for (auto iterator = alfabet.begin(); iterator != alfabet.end(); iterator++) {
        Uz* new_leaf = new Uz();
        new_leaf->key = iterator->second;
        new_leaf->s = iterator->first;
        tree.push_back(new_leaf);
    }
    Uz *n = builder(tree);

    ofstream output_file(output_name, std::ios::binary);
    Uz *nodes = n;
    char letter = 0;
    character = input_file.get();
    while (!input_file.eof()) {
        for (int i = 7; i > -1; --i) {
            if (((character >> i) & 1) == 1) {
                nodes = nodes->R;
            } else {
                nodes = nodes->L;
            }

            if(nodes->L == NULL && nodes->R == NULL) {
                letter = nodes->s;
                output_file << letter;
                nodes = n;
            }
        }
        character = input_file.get();
    }
    input_file.close();
    output_file.close();
}

void checker(const char* before_name="input.txt", const char* after_name="output.txt"){
    unsigned int same = 0;
    ifstream before_file(before_name, std::ios::binary);
    ifstream after_file(after_name, std::ios::binary);

    char after_1 = 0;
    char before_1 = 0;
    while (!after_file.eof() && !before_file.eof()){
        after_1 =after_file.get();
        before_1 =before_file.get();
        if(!after_file.eof() && !before_file.eof()){
            if(after_1 != before_1){
                same++;
            }
        }
    }
    while (!after_file.eof())
    {
        after_1 =after_file.get();
        if(!after_file.eof()){
            same++;
        }
    }
    while (!before_file.eof())
    {
        before_1 =before_file.get();
        if(!after_file.eof()){
            same++;
        }
    }
    after_file.close();
    before_file.close();
    if( same == 0) cout<<"Mister_robot_is_one_love"; else cout<< "drop_out";
}

double Coder(const char *input_name = "input.txt", const char *output_name = "codetext.txt"){
    ifstream input_file("input.txt", std::ios::in);
    map<char, int> alfavit;

    while (!input_file.eof()){
        char simvol = input_file.get();
        alfavit[simvol]++;
    }

    list<Uz*> leafs;
    for (auto iterator = alfavit.begin(); iterator != alfavit.end(); iterator++) {
        Uz* new_leafs = new Uz();
        new_leafs->key = iterator->second;
        new_leafs->s = iterator->first;
        leafs.push_back(new_leafs);
    }
    Uz *tree = builder(leafs);

    map<char, vector<bool> > huffmanCode;
    vector<bool> code;
    huffmanCodes(tree, code, huffmanCode);

    cout << "Huffman Codes are :\n" << '\n';
    for (auto pair: huffmanCode){
        cout << pair.first << " ";
        for(auto b : pair.second)
            cout << b;
        cout << endl;
    }
    ofstream output_file(output_name, std::ios::binary);

    char k = 0;
    int len = 0;
    int col_letters = leafs.size() * 40;
    output_file.write((char*) &col_letters, sizeof(col_letters));
    for (int i = 0; i < 500; i++){
        if (alfavit[i] > 0){
            char s = char(i);
            output_file.write((char*)&s, sizeof(s));
            output_file.write((char*)(&alfavit[i]), sizeof(alfavit[i]));
        }
    }

    input_file.clear();
    input_file.seekg(0);

    int bit_len = 0;
    char letter = 0;
    while (!input_file.eof()){
        char simvol = input_file.get();
        for (auto i = huffmanCode[simvol].begin(); i != huffmanCode[simvol].end(); i++) {
            bool b = *i;
            letter = letter | (b << (7 - bit_len));
            bit_len++;
            if(bit_len == 8){
                bit_len = 0;
                output_file << letter;
                letter = 0;
            }
        }
    }

    input_file.close();
    output_file.close();

    unsigned long long file_full_size = 0;
    unsigned long long commpres_size = 0;
    struct stat sb{};
    struct stat se{};
    if (!stat(input_name, &sb)) {
        file_full_size = sb.st_size;
    } else {
        perror("stat");
    }
    if (!stat(output_name, &se)) {
        commpres_size = se.st_size;
    } else {
        perror("stat");
    }
    double compress =(commpres_size + 0.0) / file_full_size*100;
    return compress;
}
 int main(){
    double compress = Coder();
    cout << "compress = " << compress << " %" << endl;
    decod();
    checker();
}