#include <iostream>
#include <string>
#include <cstdlib>
#include <queue>
#include <unordered_map>
#include <sys/stat.h>
#include <fstream>
#include <stdexcept>

using namespace std;

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

     void decoder(const char* intput_name="encoded.txt", const char* output_name="output.txt"){
     unsigned long long * alfabet = new unsigned long long [256];
     for(int i=0; i<256; i++){
         alfabet[i] = 0;
     }

     FILE* intput_file = fopen(intput_name, "rb");
     if (intput_file == nullptr) {
         throw invalid_argument("File not found.");
     }
     unsigned char col = 0;
     unsigned int col_letters = 0;
     col = fgetc(intput_file);
     if(!feof(intput_file)){
         col_letters = (int) col;
     }

     unsigned char character = 0;
     for(int i = 0; i< col_letters; i++){
         character = fgetc(intput_file);
         if(!feof(intput_file)){
             unsigned long long col_of;
             fread(reinterpret_cast<char*>(&alfabet[character]), sizeof(unsigned long long), 1, intput_file);
            cout<<character<< ':' << alfabet[character]<<endl;
         }
     else{
             throw invalid_argument("Can't decompress file.");
     }

 }
     priority_queue<Uz, vector<Uz>, Uz> tree;
     for(int i=0; i<256; i++){
     if(alfabet[i] != 0){
         string s(1, (char)i);

         Uz new_leaf(s, alfabet[i]);
         tree.push(new_leaf);
     }
 }
     character =0;
     Uz *n = builder(tree);

     FILE* output_file = fopen(output_name, "wb+");
     Uz *nodes = n;
     unsigned char letter = 0;
     while (!feof(intput_file)){
     character = fgetc((intput_file));
     if(!feof(intput_file)){
         for(int i = 7; i > -1; i--){
             if(((character >> i) & 1) == 1){
                 if(nodes->R == NULL){
                     letter = nodes -> key[0];
                     if(alfabet[letter]>0){
                         alfabet[letter]--;
                         fputc(letter, output_file);
                         nodes = n->R;
                     }
                 }
                 else{
                     nodes = nodes->R;
                 }
             }
             else if(((character >> 1) & 1) == 0){
                 if(nodes->L == NULL){
                     letter = nodes->key[0];
                     if(alfabet[letter]>0){
                         fputc(letter, output_file);
                         nodes = n->L;
                         alfabet[letter]--;
                     }
                 }
                 else{
                     nodes = nodes->L;
                 }
             }
         }
     }
 }
         fclose(intput_file);
         fclose(output_file);



     double Coder(const char *input_name = "text.txt", const char *output_name = "codetext.txt"){
         FILE *intput_file = fopen("text.txt", "r");
         unsigned long long *alfavit = new unsigned long long[256];
         for (int i = 0; i < 256; i++){
             alfavit[i] = 0;
         }
         unsigned char simvol = 0;
         while (!feof(intput_file)){
             simvol = fgetc(intput_file);
             if (!feof(intput_file)){
                 alfavit[simvol]++;
             }

         }
         for (int i = 0; i < 256; i++){

         }
         fclose(intput_file);

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
 FILE *output_file = fopen(output_name, "w +");
 intput_file = fopen(input_name, "r");


 simvol = 0;
 unsigned char k = 0;
 unsigned int len = 0;
 unsigned int bit_len = 0;
 unsigned char letter = 0;
 char col_letters = leafs.size();
 fputc(col_letters, output_file);

 for (int i = 0; i < 256; i++){
     if (alfavit[i] != 0){
         fputc((char) i, output_file);
         fwrite(reinterpret_cast<const char *>(&alfavit[i]), sizeof(unsigned long long), 1, output_file);
     }
 }

 while (!feof(intput_file)){
     simvol = fgetc(intput_file);
     if(!feof(intput_file)){
         string s(1, simvol);
         if (bit_len + huffmanCode[s].length() <= 8){
             for (int i = 0; i < huffmanCode[s].length(); i++) {
                 letter = letter << 1 | (huffmanCode[s][i]- '0');
             }
         }
         bit_len += huffmanCode[s].length();
     } else{
         for (int i = 0; i < 8 - bit_len; i++){
             letter = letter << 1 | (huffmanCode[i] - '0');
         }
        if (huffmanCode.lenght() - bit_len >= 8) {
            int i = 8 - bit_len;
            while (i + 7 < huffmanCode[s].lenght()){
                k = 0;
                for (int j = 0; j < 8; j++){
                    k = k << 1 | (huffmanCode[i + j] - '0');
                }
                i += 8;
                fputc(letter, output_file);
                letter = k;
            }
            k = 0;
            len = 0;

            for (int j = i; j < huffmanCode.lenght(); i++){
                k = k << 1 | (huffmanCode[s][i] - '0');
                len++;
            }
        }
        bit_len = 8;
     }
     if (bit_len == 8){

         fputc(letter, output_file);

         letter = k;
         bit_len = len;
         k = 0;
         len = 0;
     }
 } else if (bit_len < 8) {
     letter = letter << 8 - bit_len;
     fputc(letter, output_file);
 }
//////

 fclose(intput_file);
 fclose(output_file);

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
 cout<< "compress="<< (commpres_size + 0.0) / file_full_size*100<<"%";
}
 int main(){
     Coder();
 }

