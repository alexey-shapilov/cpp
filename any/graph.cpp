#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <set>
using namespace std;

struct Vertex;
struct Node;

struct Node {
  Vertex *vertex;
  Node *next;
};

struct Connections {
  int count;
  Node *elements;
};

struct Vertex {
  int id;
  struct Connections connections;
};

struct VertexHeader {
  int id;
  int countConnections;
};

Node *CreateNode(Vertex *vertex) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->next = NULL;
  node->vertex = vertex;

  return node;
}

void LinkChild(Vertex *vertex, Vertex *child) {
  Node *connect = vertex->connections.elements;

  if (connect == NULL) {
    vertex->connections.elements = CreateNode(child);
    vertex->connections.count++;
    return;
  }

  // выходим из функции т.к. вершина уже добавлена
  if (connect->vertex->id == child->id) {
    return;
  }
  while (connect->next != NULL) {
    connect = connect->next;

    // выходим из функции т.к. вершина уже добавлена
    if (connect->vertex->id == child->id) {
      return;
    }
  }

  connect->next = CreateNode(child);
  vertex->connections.count++;
}

void Link(Vertex *vertex1, Vertex *vertex2) {
  LinkChild(vertex1, vertex2);
  LinkChild(vertex2, vertex1);
}

Vertex *CreateVertex(int id) {
  Vertex *vertex = (Vertex *)malloc(sizeof(Vertex));
  vertex->id = id;
  vertex->connections.elements = NULL;
  vertex->connections.count = 0;

  return vertex;
}

void PrintVertex(Vertex *vertex) {
  cout << vertex->id;

  if (vertex->connections.count == 0) {
    cout << endl;
    return;
  }

  cout << '(';

  Node *connect = vertex->connections.elements;
  while (connect->next != NULL) {
    cout << connect->vertex->id << ',';
    connect = connect->next;
  }

  cout << connect->vertex->id << ')' << endl;
}

// множество для сохранения уже обработанных вершин
set<int> t;

void WriteGraph(Vertex *vertex, ofstream &fout) {
  VertexHeader h{vertex->id, vertex->connections.count};
  t.insert(vertex->id);

  int *connections = new int[h.countConnections];
  int *connection = connections;

  if (vertex->connections.count > 0) {
    Node *p = vertex->connections.elements;
    while (p != NULL) {
      int vertexId = p->vertex->id;
      *connection++ = vertexId;
      if (t.count(vertexId) == 0) {
        WriteGraph(p->vertex, fout);
      }
      p = p->next;
    }
  }

  fout.write((char *)&h, sizeof(VertexHeader));
  if (h.countConnections > 0) {
    fout.write((char *)connections, sizeof(*connections) * h.countConnections);
  }

  delete[] connections;
}

Vertex *ReadGraph(ifstream &fin) {
  map<int, Vertex *> vertices;
  Vertex *vertex;
  Vertex *child;
  VertexHeader h;

  while (fin) {
    fin.read((char *)&h, sizeof(VertexHeader));
    if (fin.fail()) {
      break;
    }

    if (vertices.count(h.id) > 0) {
      vertex = vertices.at(h.id);
    } else {
      vertex = CreateVertex(h.id);
      vertices.insert({h.id, vertex});
    }

    if (h.countConnections > 0) {
      int *connections = new int[h.countConnections];
      fin.read((char *)connections, sizeof(*connections) * h.countConnections);

      int *connection = connections;
      for (int i{0}; i < h.countConnections; i++) {
        if (vertices.count(*connection) > 0) {
          child = vertices.at(*connection);
        } else {
          child = CreateVertex(*connection);
          vertices.insert({*connection++, child});
        }
        Link(vertex, child);
      }

      delete[] connections;
    }
  }
  return vertex;
}

void PrintGraph(Vertex *vertex) {
  PrintVertex(vertex);
  t.insert(vertex->id);

  if (vertex->connections.count > 0) {
    Node *p = vertex->connections.elements;
    while (p != NULL) {
      int vertexId = p->vertex->id;
      if (t.count(vertexId) == 0) {
        PrintGraph(p->vertex);
      }
      p = p->next;
    }
  }
}

int main(int argc, char *argv[]) {
  Vertex *vertex1 = CreateVertex(1);
  Vertex *vertex2 = CreateVertex(2);
  Vertex *vertex3 = CreateVertex(3);
  Vertex *vertex4 = CreateVertex(4);
  Vertex *vertex5 = CreateVertex(5);
  Vertex *vertex6 = CreateVertex(6);
  Vertex *vertex7 = CreateVertex(7);
  Vertex *vertex8 = CreateVertex(8);
  Vertex *vertex9 = CreateVertex(9);
  Vertex *vertex10 = CreateVertex(10);

  Link(vertex1, vertex2);
  Link(vertex1, vertex9);

  Link(vertex2, vertex3);
  Link(vertex2, vertex8);

  Link(vertex3, vertex4);
  Link(vertex3, vertex9);

  Link(vertex4, vertex5);
  Link(vertex4, vertex6);
  Link(vertex4, vertex7);

  Link(vertex5, vertex6);

  Link(vertex6, vertex7);

  Link(vertex7, vertex8);

  Link(vertex8, vertex9);

  Link(vertex9, vertex10);
  Link(vertex9, vertex1);

  ofstream fout("graph.bin", ios::out | ios::binary | ios::trunc);
  if (fout.is_open()) {
    WriteGraph(vertex10, fout);
    fout.close();
    t.clear();
  }

  ifstream fin("graph.bin", ios::in | ios::binary);
  if (fin.is_open()) {
    PrintGraph(ReadGraph(fin));
    fin.close();
    t.clear();
  }

  return 0;
}
