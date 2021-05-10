#ifndef NODE_H
#define NODE_H

using namespace std;

class Node //node with left and right pointers
{
 public:
  Node();
  ~Node();
  void setRight(Node*);
  void setLeft(Node*);
  void setNext(Node*);
  Node* getRight();
  Node* getLeft();
  Node* getNext();
  void setData(char*);
  char* getData();
 private:
  Node* left;
  Node* right;
  Node* next;
  char* data;
};
#endif