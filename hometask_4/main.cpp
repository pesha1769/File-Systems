#include <iostream>
#include "BTree.h"

int main() {
    BTree tree1(2);
    tree1.insert(3);
    tree1.insert(5);
    tree1.insert(7);
    tree1.insert(9);
    tree1.insert(11);
    tree1.Dump();

    BTree tree2(2);
    tree2.insert(4);
    tree2.insert(6);
    tree2.insert(8);
    tree2.insert(10);
    tree2.insert(12);
    tree2.Dump();

    BTree* tree3 = tree1.merge(&tree2);
    tree3->Dump();

    return 0;
}
