#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <random>
using namespace std;
int Min;
const int MAXSIZE = 32767;
class Node{
    public:
        int key, data, minData;
        Node* left;
        Node* right;
};
int min3(int a, int b, int c){
    int min;
    if (a < b){
        if (a < c){
            min = a;
        }
        else{
            min = c;
        }
    }
    else if (b < c){
        min = b;
    }
    else{
        min = c;
    }
    return min;
}
Node* createNode(int key, int data){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->data = data;
    newNode->minData = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}
Node* insert(Node* root, int key, int data){
    if (root == nullptr){
        root = createNode(key, data);
        root->minData = root->data;
        return root;
    }
    if (key < root->key){
        root->minData = min(root->minData, data);
        root->left = insert(root->left, key, data);
    }
    else if (key > root->key){
        root->minData = min(root->minData, data);
        root->right = insert(root->right, key, data);
    }
    else{
        if (root->data < data) {
            root->data = data;
            root->minData = root->data;
        }
    }
    return root;
}
void RMR(Node* root, int k1){
    if (root != nullptr){
        if (k1 < root->key){
            Min = min3(Min, root->data, root->right != nullptr ? root->right->minData : MAXSIZE);
            RMR(root->left, k1);
        }
        else if (k1 > root->key){
            RMR(root->right, k1);
        }
        else{
            Min = min3(Min, root->data, root->right != nullptr ? root->right->minData : MAXSIZE);
        }
    }
}
void RML(Node* root, int k2){
    if (root != nullptr) {
        if (k2 > root->key) {
            Min = min3(Min, root->data, root->left != nullptr ? root->left->minData : MAXSIZE);
            RML(root->right, k2);
        }
        else if (k2 < root->key) {
            RML(root->left, k2);
        }
        else {
            Min = min3(Min, root->data, root->left != nullptr ? root->left->minData : MAXSIZE);
        }
    }
}
int RMQ(Node* root, int k1, int k2){
    while (root != nullptr && !(k1 <= root->key && k2 >= root->key)){
        if (k1 < root->key && k2 < root->key){
            root = root->left;
        }
        else{
            root = root->right;
        }
    }
    Min = root->data;
    RMR(root->left, k1);
    RML(root->right, k2);
    return Min;
}
int naiveRMQ(Node* root, int k1, int k2){
    int leftMin = MAXSIZE;
    int rightMin = MAXSIZE;

    if (root != nullptr) {
        if (k1 < root->key) {
            leftMin = naiveRMQ(root->left, k1, k2);
        }
        if (k2 > root->key) {
            rightMin = naiveRMQ(root->right, k1, k2);
        }
        if ((k1 <= root->key) && (root->key <= k2)) {
            Min = root->data;
        }
        return min3(leftMin, rightMin, Min);
    }
}
void inorder(Node* root){
    if (root != nullptr){
        inorder(root->left);
        cout << "Key: " << root->key << "   " << "Data: " << root->data << endl;
        inorder(root->right);
    }
}
void generateFile() {
    ofstream outf("inputFile.txt");
    uniform_int_distribution<int> distr(0, MAXSIZE);
    random_device rd;
    mt19937 gen(rd());
    int key, data, k1, k2;

    for (int i = 0; i < 1000; i++) {
        key = distr(gen);
        data = distr(gen);
        outf << "IN " << key << " " << data << endl;
    }
    
    k1 = distr(gen);
    k2 = distr(gen);

    outf << "RMQ " << k1 << " " << k2 << endl;
    outf << "naiveRMQ " << k1 << " " << k2 << endl;
}
int main(){
    generateFile();
    ifstream f("inputFile.txt");
    ofstream outf("timeTest.txt");

    string instruction;
    int key, data, k1, k2;
    Node* root = nullptr;

    while (f >> instruction){
        if (instruction == "IN"){
            f >> key;
            f >> data;
    
           root = insert(root, key, data);
        }
        if (instruction == "RMQ"){
            f >> k1;
            f >> k2;
            if (k1 > k2)
            {
                int temp = k1;
                k1 = k2;
                k2 = temp;
            }
            auto start = chrono::high_resolution_clock::now();
            int ans = RMQ(root, k1, k2);
            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
            outf << "RMQ runtime (ns): " << duration.count() << endl;
            cout << "The minimum Data between keys " << k1 << " and " << k2 << " is: " << ans << endl;
        }
        if (instruction == "naiveRMQ"){
            f >> k1;
            f >> k2;
            if (k1 > k2){
                int temp = k1;
                k1 = k2;
                k2 = temp;
            }
            auto start = chrono::high_resolution_clock::now();
            int ans = naiveRMQ(root, k1, k2);
            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
            outf << "naiveRMQ runtime (ns): " << duration.count() << endl;
            cout << "The minimum Data between keys " << k1 << " and " << k2 << " is: " << ans << endl;
        }
    }
    inorder(root);
    return 0;
}
