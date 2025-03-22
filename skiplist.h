//
// Created by 30884 on 25-2-18.
//

#pragma once
#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <vector>
#include <iostream>
#include <random>

const int MAX_LEVEL = 16;   // 最大层数
const float POSSIBILITY = 0.5; // 每层增长的概率

class Skiplist {
private:
    struct Node {
        int value;
        std::vector<Node*> forward; // 存储各层的指向

        Node(int val, int level) : value(val), forward(level, nullptr) {}
    };

    Node* header;  // 头结点
    int level;     // 当前最大层数
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;

public:
    Skiplist() : level(1), gen(rd()), dist(0.0, 1.0) {
        header = new Node(-1, MAX_LEVEL);
    }

    ~Skiplist() {
        Node* current = header;
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }

    bool search(int target);
    void insert(int value);
    bool erase(int value);
    int randomLevel();
};

// **随机层数生成**
int Skiplist::randomLevel() {
    int levels = 1;
    while (dist(gen) < POSSIBILITY && levels < MAX_LEVEL) {
        levels++;
    }
    return levels;
}

// **查找目标值**
bool Skiplist::search(int target) {
    Node* current = header;
    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < target) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    return current && current->value == target;
}

// **插入元素**
void Skiplist::insert(int value) {
    std::vector<Node*> update(MAX_LEVEL, nullptr);
    Node* current = header;

    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    if (current->forward[0] && current->forward[0]->value == value) {
        std::cout << "Duplicate value: " << value << std::endl;
        return;
    }

    int newLevel = randomLevel();
    if (newLevel > level) {
        for (int i = level; i < newLevel; i++) {
            update[i] = header;
        }
        level = newLevel;
    }

    Node* newNode = new Node(value, newLevel);
    for (int i = 0; i < newLevel; i++) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
}

// **删除元素**
bool Skiplist::erase(int value) {
    std::vector<Node*> update(MAX_LEVEL, nullptr);
    Node* current = header;

    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (!current || current->value != value) return false;

    for (int i = 0; i < level; i++) {
        if (update[i]->forward[i] != current) break;
        update[i]->forward[i] = current->forward[i];
    }

    delete current;

    while (level > 1 && !header->forward[level - 1]) {
        level--;
    }
    return true;
}

#endif // SKIPLIST_H
