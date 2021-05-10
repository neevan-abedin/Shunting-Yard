/******************************************************************************

Shunting Yard by Neevan Abedin

Expression converter that can output prefix, infix, or postfix notation of 
inputted expression. Displays a binary expression tree of the postfix notation.

5.9.2021

*******************************************************************************/

#include <iostream>
#include <cstring>
#include <iomanip>
#include <stack>
#include "node.h"

using namespace std;

/*Program Structure
-take in input in infix
-translate input into postfix using shunting yard
-create expression tree from postfix notation
*/

//function prototypes
//notation
void prefix(Node* tree);
void infix(Node* tree);
void postfix(Node* tree);

//getPrecedence of operators (PEMDAS)
int getPrecedence(char* Operator);

//tree manipulators
void pushTree(Node* &tree, Node* &add);
void popTree(Node* &tree);

//queue
void enqueue(Node* &front, Node* &rear, char* val);
void dequeue(Node* &front, Node* &rear);
bool queueEmpty(Node* front, Node* rear);
void displayQueue(Node* front, Node* rear);

//node operators
void push(Node* &top, char* val);
void pop(Node* &top);
bool nodeEmpty(Node* top);

int main()
{
  int signCount = 0;
  int numCount = 0;
    
  Node* top = NULL;
  Node* front = NULL;  
  Node* rear = NULL;

  bool running = true;
  char input[100];
  char in[10];
  
  while(running)
  {
    cout << endl << "Do you want to 'calculate' or 'quit'?" << endl;
    cin.get(in, 10);
    cin.clear();
    cin.ignore(10000, '\n');

    if (strcmp(in, "calculate") == 0) {
      cout << "Input equation with infix notation:" << endl;
      cin.get(input, 100);
      cin.clear();
      cin.ignore(10000, '\n');
      
      char** modif = new char*[100];  //clear char* array
      top = front = rear = NULL;  //clear stack & queue
      //begin input splitting:
      //break input by spaces
      int l = 0;  //keeps track of # of chars before space
      int count = 0; //number of chars
      
      for (int i = 0; i < strlen(input); i++)
      {
	if (input[i] == ' ')
	{
	  if (l == 1)
	  {
	    char *temp = new char[10];
	    for (int z = 0; z < sizeof(temp); z++)
	    {  //clear temp memory
	      temp[z] = 0;
	    }
	    temp[0] = input[i-1];
	    modif[count] = temp;
	    count++;
	    l = 0;
	  }
	  else
	  {
	    char *temp = new char[10];
	    for (int z = 0; z < sizeof(temp); z++)
	    {
	      temp[z] = 0;
	    }
	    for (int a = 0; a < l; a++) {
	      temp[a] = input[i-l+a]; 
	    }
	    modif[count] = temp;
	    count++;
	    l = 0;
	  }
	}
	else
	{
	  char *temp = new char[10];
	  for (int z = 0; z < sizeof(temp); z++)
	  {  //clear temp memory
	    temp[z] = 0;
	  }
	  l++;
	  if (i == strlen(input) - 1)
	  {  //last possible pair of chars
	    for (int a = 0; a < l; a++)
	    {
	      temp[a] = input[i+a+1-l];
	    }
	    modif[count] = temp;
	    count++;
	  }
	}
      }
      for (int i = 0; i < count; i++)
      {
	cout << modif[i] << "  p: " << getPrecedence(modif[i]) << endl;
      }
      int c = 0;
      
      // SHUNTING YARD
      while (c < count)
      {
	//if number
	if (getPrecedence(modif[c]) == 0)
	{  
	  //enqueue on output queue
	  enqueue(front, rear, modif[c]);
	}
	if (getPrecedence(modif[c]) == 1 || getPrecedence(modif[c]) == 2 || getPrecedence(modif[c]) == 3)
	{
	  //when operator is found
	  if (top != NULL) {
	    while (getPrecedence(top->getData()) >= getPrecedence(modif[c]) &&
		   *top->getData() != '(') {
	      //pop from stack and enqueue on output queue
	      enqueue(front, rear, top->getData());
	      pop(top);
	      if (top == NULL) break;
	    }
	  }
	  //push to stack
	  push(top, modif[c]);
	}
	if (*modif[c] == '(') {  //if left parenthesis is found
	  //push to stack
	  push(top, modif[c]);
	}
	if (*modif[c] == ')') {  //if right parenthesis is found
	  while (*top->getData() != '(') { 
	    //pop from stack and enqueue on output queue
	    enqueue(front, rear, top->getData());
	    pop(top);
	  }
	  if (*top->getData() == '(') {
	    //pop from stack and remove
	    pop(top);
	  }
	}
	c++;
      }
      if (c == count) { //if no tokens left to read, pop from stack and enqueue on output queue
	while(top != NULL) {
	  enqueue(front, rear, top->getData());
	  pop(top);
	}
      }
      //print queue node list
      cout << "Binary Tree created based on:" << endl;
      displayQueue(front, rear);
      cout << "with " << count << " tokens." << endl;
      //binary expression tree
      Node* tree = NULL;  //tree stack top
      //turn queue into tree stack
      while (front != rear) {
	Node* n = new Node();
	n->setData(front->getData());
	pushTree(tree, n);
	front = front->getNext();
      }
      //do the same for the rear
      Node* n = new Node();
      n->setData(rear->getData());
      pushTree(tree, n);
      //output post-, pre-, or in- fix
      cout << "Type in pre, post, or in, if you want to see prefix, postfix, or infix." << endl;
      char in[5];
      cin.get(in, 5);
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Output:" << endl;
      if (strcmp(in, "pre") == 0) {
	prefix(tree);  //prefix notation
      } else if (strcmp(in, "post") == 0) {
	postfix(tree);  //postfix notation
      } else if (strcmp(in, "in") == 0) {
	infix(tree);  //infix notation
      } else {
	cout << "Invalid input." << endl;
      }
      cout << endl;
    }
    else if (strcmp(in, "quit") == 0) {
      cout << endl << "Program has ended." << endl;
      running = false;
    }
    else {
      cout << endl << "Invalid input. Try again." << endl;
    }
  }
  return 0;
}

int getPrecedence(char* op) {
  if (*op == '^') {
    return 3;  //highest precedence
  } else if (*op == '*' || *op == '/') {
    return 2; //second highest precedence
  } else if (*op == '+' || *op == '-') {
    return 1;  //second lowest precedence
  } else if (*op == '(' || *op == ')') {
    return 4;  //unique value for parenthesis
  }
  else {
    return 0; //number has lowest precedence
  }
}

bool nodeEmpty(Node* top) {  //check stack
  if (top == NULL) {
    return true;
  } else {
    return false;
  }
}

void push (Node* &top, char* val) { //allow new element at end of stack
  Node* ptr = new Node();
  ptr->setData(val);
  ptr->setNext(top);
  top = ptr;
}

void pop(Node* &top) { //remove newest element
  if (nodeEmpty(top)) {
    cout << "Stack is empty." << endl;
  } else {
    Node* ptr = top;
    top = top->getNext();
    ptr->setNext(NULL);
  }
}

//add node to queue
void enqueue(Node* &front, Node* &rear, char* val) {
  Node* ptr = new Node();
  ptr->setData(val);
  ptr->setNext(NULL);
  if (front == NULL) { 
    front = ptr;
    rear = ptr;
  } else {
    rear->setNext(ptr);
    rear = ptr;
  }
}

//remove first node in queue
void dequeue(Node* &front, Node* & rear) {
  if(queueEmpty(front, rear)) {
    cout << "Queue is empty." << endl;
  } else if (front == rear) {  //if one node
    //delete existent node
    front = NULL;
    rear = NULL;
  } else {
    Node* ptr = front;
    front = front->getNext();
    ptr->setNext(NULL);
  }
}

//check if queue is empty
bool queueEmpty (Node* front, Node* rear) {
  if (front == NULL && rear == NULL) {
    return true;
  } else {
    return false;
  }
}

//show queue
void displayQueue(Node* front, Node* rear) {
  if(queueEmpty(front, rear)) {  //empty queue
    cout << "Queue is empty." << endl;
  } else {
    Node* ptr = front;
    while (ptr != NULL) {
      cout << ptr->getData() << " ";
      ptr = ptr->getNext();
    }
    cout << endl;
  }
}

//tree manipulators
void pushTree(Node* &tree, Node* &add) {
  if (tree == NULL) {  //empty tree stack
    add->setNext(tree);
    tree = add;
  } else {
    if (getPrecedence(add->getData()) == 0) {  //if token is number
      add->setNext(tree);
      tree = add;
    } else {  //if token is operator
      //pop last two nodes and keep them
      Node* l = tree;
      popTree(tree);
      l->setNext(NULL);
      Node* r = tree;
      popTree(tree);
      r->setNext(NULL);
      //push operator node
      add->setNext(tree);
      tree = add;
      //make the two popped numbers left and right pointers of operator node
      tree->setLeft(l);
      tree->setRight(r);
    }
  }
}

void popTree(Node* &tree) {
  if (tree == NULL) {
    cout << "Tree stack empty." << endl;
  } else {
    Node* ptr = tree;
    tree = tree->getNext();
    ptr->setNext(NULL);
  }
}

//display final tree -- https://en.wikipedia.org/wiki/Binary_expression_tree
void prefix(Node* t) {
  if (t != NULL) {
    cout << t->getData() << " ";
    prefix(t->getLeft());
    prefix(t->getRight());
  }
}

void postfix(Node* t) {
  if(t != NULL) {
    postfix(t->getLeft());
    postfix(t->getRight());
    cout << t->getData() << " ";
  }
}

void infix(Node* t) {
  if (t != NULL) {
    if (getPrecedence(t->getData()) != 0) {
      cout << "( ";
    }
    infix(t->getLeft());
    cout << t->getData() << " ";
    infix(t->getRight());
    if (getPrecedence(t->getData()) != 0) {
      cout << ") ";
    }
  }
}
