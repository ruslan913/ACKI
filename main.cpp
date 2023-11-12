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
    unsigned long long size;
    Uz *R;
    Uz *L;

    bool operator()(const Uz &x, const Uz &y) {
        return x.size >= y.size;
    }
    Uz(const string &value = "", unsigned long long cast = 0, Uz *left = NULL, Uz *right = NULL) {
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
             unsigned long long *alfabet = new unsigned long long[256];
             for (int i = 0; i < 256; i++) {
                 alfabet[i] = 0;
             }

             ifstream input_file(input_name, std::ios::binary);
             if (!input_file.is_open()) {
                 throw invalid_argument("File not found.");
             }
             unsigned char col = 0;
             unsigned int col_letters = 0;
             col = input_file.get();
             if (!input_file.eof()) {
                 col_letters = (int) col;
             }

             unsigned char character;
             for (int i = 0; i < col_letters; i++) {
                 character = input_file.get();
                 if (!input_file.eof()) {
                     unsigned long long col_of;
//                     alfabet[character] = input_file.get(); //TODO (Считывание шапки)
                     input_file.read((char*)alfabet[character], sizeof(alfabet[character]));
                     cout << character << ':' << alfabet[character] << endl;
                 } else {
                     throw invalid_argument("Can't decompress file.");
                 }

             }
             priority_queue<Uz, vector<Uz>, Uz> tree;
             for (int i = 0; i < 256; i++) {
                 if (alfabet[i] != 0) {
                     string s(1, (char) i);

                     Uz new_leaf(s, alfabet[i]);
                     tree.push(new_leaf);
                 }
             }
             character = 0;
             Uz *n = builder(tree);

             ofstream output_file(output_name, std::ios::binary);
             Uz *nodes = n;
             unsigned char letter = 0;
             while (!input_file.eof()) {
                 character =input_file.get();
                 if (!input_file.eof()) {
                     for (int i = 7; i > -1; i--) {
                         if (((character >> i) & 1) == 1) {
                             if (nodes->R == NULL) {
                                 letter = nodes->key[0];
                                 if (alfabet[letter] > 0) {
                                     alfabet[letter]--;
                                     output_file << letter;
//                                     fputc(letter, output_file);
                                     nodes = n->R;
                                 }
                             } else {
                                 nodes = nodes->R;
                             }
                         } else if (((character >> 1) & 1) == 0) {
                             if (nodes->L == NULL) {
                                 letter = nodes->key[0];
                                 if (alfabet[letter] > 0) {
                                     output_file << letter;
                                     nodes = n->L;
                                     alfabet[letter]--;
                                 }
                             } else {
                                 nodes = nodes->L;
                             }
                         }
                     }
                 }
             }
             input_file.close();
             output_file.close();
         }

     void checker(const char* before_name="input.txt", const char* after_name="output.txt"){
     unsigned int same = 0;
     ifstream before_file(before_name, std::ios::binary);
     ifstream after_file(after_name, std::ios::binary);

     unsigned char after_1 = 0;
     unsigned char before_1 = 0;
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
         unsigned long long *alfavit = new unsigned long long[256];
         for (int i = 0; i < 256; i++){
             alfavit[i] = 0;
         }
         if(!input_file.is_open()){
             cout << "vi dolbayobi ne tam smotrite" << endl;
             exit(1);
         }
         char simvol;
         while (!input_file.eof()){
             simvol =input_file.get();
             cout<<simvol<< endl;
             if (!input_file.eof()){
                 alfavit[simvol]++;

             }

         }

         for (int i = 0; i < 256; i++){

         }
         input_file.close();
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
 ofstream output_file(output_name, std::ios::out);

 input_file.open("input.txt", std::ios::in);

 unsigned char k = 0;
 unsigned int len = 0;
 unsigned int bit_len = 0;
 unsigned char letter = 0;
 char col_letters = leafs.size();
 output_file<< col_letters;
 for (int i = 0; i < 256; i++){
     if (alfavit[i] != 0){
         output_file << (char)i;
         output_file << alfavit[i];
     }
 }

 while (!input_file.eof()){
     simvol =input_file.get();
     string s = "" + simvol;
//     string s(1, simvol);
     if(!input_file.eof()){
         if (bit_len + huffmanCode[s].length() <= 8){
             for (int i = 0; i < huffmanCode[s].length(); i++) {
                 letter = letter << 1 | (huffmanCode[s][i]- '0');
             }
         }
         bit_len += huffmanCode[s].length();
     } else{
         for (int i = 0; i < 8 - bit_len; i++){
             letter = letter << 1 | (huffmanCode[s][i] - '0');
         }
        if (huffmanCode[s].length() - bit_len >= 8) {
            int i = 8 - bit_len;
            while (i + 7 < huffmanCode[s].length()){
                k = 0;
                for (int j = 0; j < 8; j++){
                    k = k << 1 | (huffmanCode[s][i + j] - '0');
                }
                i += 8;
                 output_file << letter;
                letter = k;
            }
            k = 0;
            len = 0;

            for (int j = i; j < huffmanCode[s].length(); i++){
                k = k << 1 | (huffmanCode[s][i] - '0');
                len++;
            }
        }
        bit_len = 8;
     }
     if (bit_len == 8){

         output_file << letter;

         letter = k;
         bit_len = len;
         k = 0;
         len = 0;

     }
     else if  (bit_len < 8) {
         letter = letter << (8 - bit_len);
         output_file << letter;
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

