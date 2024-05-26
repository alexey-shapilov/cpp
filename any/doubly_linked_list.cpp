#include <cstddef>
#include <cstdlib>
#include <iostream>

using namespace std;

struct Node {
  int value;
  Node *prev;
  Node *next;
};

Node *head = NULL;
Node *tail = NULL;

void InitLinkedList(int value) {
  head = (Node *)malloc(sizeof *head);
  head->value = value;
  head->next = NULL;
  head->prev = NULL;

  tail = head;
}

Node *PushNode(int value) {
  if (tail == NULL) {
    InitLinkedList(value);

    return head;
  }

  Node *node = (Node *)malloc(sizeof *node);
  node->value = value;
  node->next = NULL;
  node->prev = tail;

  tail->next = node;
  tail = node;

  return node;
}

void PrintLinkedListForward(Node *node) {
  Node *p = node;

  while (p != NULL) {
    cout << p->value << " ";

    p = p->next;
  }
  cout << endl;
}

void PrintLinkedListBackward(Node *node) {
  Node *p = node;

  while (p != NULL) {
    cout << p->value << " ";

    p = p->prev;
  }
  cout << endl;
}

int main() {
  PushNode(1);
  PushNode(2);
  Node *node = PushNode(5);
  PushNode(3);
  PushNode(10);

  PrintLinkedListForward(node);
  PrintLinkedListBackward(node);

  return 0;
}
