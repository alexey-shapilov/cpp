#include <cstddef>
#include <cstdlib>
#include <iostream>

using namespace std;

struct Node {
  int value;
  Node *next;
};

Node *head = NULL;
Node *tail = NULL;

void InitLinkedList(int value) {
  head = (Node *)malloc(sizeof *head);
  head->value = value;
  head->next = NULL;

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

  tail->next = node;
  tail = node;

  return node;
}

void PrintLinkedList(Node *node) {
  Node *p = node;

  while (p != NULL) {
    cout << p->value << " ";

    p = p->next;
  }
  cout << endl;
}

int main() {
  PushNode(1);
  PushNode(2);
  Node *node = PushNode(5);
  PushNode(3);
  PushNode(10);

  PrintLinkedList(head);
  PrintLinkedList(node);

  return 0;
}
