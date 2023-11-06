#include <iostream>
#include <string>
#include <cstdlib>
#include <queue>
#include <unordered_map>
#include <sys/stat.h>


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
    Uz(const string &value ="", unsigned long long cast = 0, Uz *lest = NULL, Uz *right = NULL){
      key = value;
      size = cast;
      L = left;
      R = right;
    }
    Uz *join(Uz uz) {
        return new Uz(uz.key + key, uz.size + size, new Uz(uz), this);
    }
};


 Uz *builder(priority_queue<Uz, vector<Uz>, Uz> leafs){
     while (leafs.size() > 1) {
         Uz *n = new Uz(leafs.top());
         leafs.pop();
         leafs.push(*n ->join(*new Uz(leafs.top())));
         leafs.pop();
     }
     void huffmanCodes(Uz *root, string code, unordered_map<string, string> &huffmanCode){
     if (root == nullptr)
         return;
     if (!root -> L && !root ->R) {
         huffmanCode[root ->key] = code;
     }
     huffmanCodes(root ->L, code + "0", huffmanCode);
     huffmanCodes(root ->R, code + "1", huffmanCode);

     }

     double Coder(const char *input_name = "text.txt", const char *output_name = "codetext.txt"){
         FILE *intput_file = fopen("text.txt", "r");
         unsigned long long *alfavit = new unsigned long long[256];
         for (int i = 0; i < 256; i++){
             alfavit[i] = 0;
         }
     }
 }

