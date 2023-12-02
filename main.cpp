#include <iostream>
#include <string>
#include <cstdlib>
#include <queue>
#include <unordered_map>
#include <sys/stat.h>
#include <fstream>
#include <stdexcept>
#include <stdio.h>

using namespace std;

void choice(){

}

class Uz {
public:
    string key;
    int size;
    Uz *R;
    Uz *L;

    bool operator()(const Uz &x, const Uz &y) {
        return x.size >= y.size;
    }
    Uz(const string &value = "", char cast = 0, Uz *left = NULL, Uz *right = NULL) {
      key = value;
      size = cast;
      L = left;
      R = right;
    }
    Uz *join(Uz uz) {
        return new Uz(uz.key + key, uz.size + size, new Uz(uz), this);
    }
};

Uz *builder(priority_queue<Uz, vector<Uz>, Uz> leafs) {
    while (leafs.size() > 1) {
        Uz *n = new Uz(leafs.top());
        leafs.pop();
        leafs.push(*n->join(*new Uz(leafs.top())));
        leafs.pop();
    }
    return new Uz(leafs.top());
}

void huffmanCodes(Uz *root, string code, unordered_map<string, string> &huffmanCode) {
    if (root == nullptr)
        return;
    if (!root -> L && !root ->R) {
        huffmanCode[root ->key] = code;
    }
    huffmanCodes(root ->L, code + "0", huffmanCode);
    huffmanCodes(root ->R, code + "1", huffmanCode);
}

void decod(const char* input_name="codetext.txt", const char* output_name="output.txt") {
    int *alfabet = new int[256];
    for (int i = 0; i < 256; i++) {
        alfabet[i] = 0;
    }

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
    priority_queue<Uz, vector<Uz>, Uz> tree;
    for (int i = 0; i < 256; i++) {
        if (alfabet[i] != 0) {
            string s(1, (char) i);

            Uz new_leaf(s, alfabet[i]);
            tree.push(new_leaf);
        }
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
                letter = nodes->key[0];
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
    int *alfavit = new int[256];
    for (int i = 0; i < 256; i++){
        alfavit[i] = 0;
    }

    char simvol;
    while (!input_file.eof()){
        simvol =input_file.get();
        cout<<simvol<< endl;
        if (!input_file.eof()){
            alfavit[simvol]++;

        }

    }

    priority_queue<Uz, vector<Uz>, Uz> leafs;
    for (int i = 0; i < 256; i++) {
        if (alfavit[i] != 0) {
            string s(1, (char) i);
            Uz new_leafs(s, alfavit[i]);
            leafs.push(new_leafs);
        }
    }
    Uz *tree = builder(leafs);

    unordered_map<string, string> huffmanCode;
    huffmanCodes(tree, "", huffmanCode);

    cout << "Huffman Codes are :\n" << '\n';
    for (auto pair: huffmanCode){
        cout << pair.first << " " << pair.second << '\n';
    }
    ofstream output_file(output_name, std::ios::binary);

    char k = 0;
    int len = 0;
    int col_letters = leafs.size() * 40;
    output_file.write((char*) &col_letters, sizeof(col_letters));
    for (int i = 0; i < 256; i++){
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
        simvol = input_file.get();
        string s(1, simvol);
        for (int i = 0; i < huffmanCode[s].length(); i++) {
            letter = letter | (huffmanCode[s][i]- '0') << (7 - bit_len);
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