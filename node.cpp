#include <iostream>
#include <cstring>

#include"node.h"

using namespace std;

Node::Node() { //constructor
    data = NULL;
    right = NULL;
    left = NULL;
    next = NULL;
}

Node::~Node() { //destructor
    
}

//setters
void Node::setData(char* newData) {
    data = newData;
}

void Node::setRight(Node* newRight) {
    right = newRight;
}

void Node::setLeft(Node* newLeft) {
    left = newLeft;
}

void Node::setNext(Node* newNext) {
    next = newNext;
}

//getters
char* Node::getData() {
  return data;
}

Node* Node::getLeft() {
  return left;
}

Node* Node::getRight() {
  return right;
}

Node* Node::getNext() {
  return next;
}