#include "BTree.h"
#include <iostream>
#include <cstring>
#include <algorithm>

BTreeNode::BTreeNode(int t, bool leaf) :
    minDegree(t),
    isLeaf(leaf),
    KeysNum(0),
    Keys(new int [2 * t  - 1]),
    Children(new BTreeNode* [2 * t])
{}

BTreeNode::~BTreeNode() {
    delete Keys;
    if (!isLeaf)
        for (int i = 0; i <= KeysNum; i++)
            delete Children[i];
    delete Children;
}

int l = 0; 
BTreeNode *BTreeNode::search(int key) {
    int i = l + (KeysNum - 1) / 2;

    if (Keys[i] == key)
        return this;

    if (isLeaf)
        return nullptr;

	if (Keys[i] > key){
		KeysNum = i - 1 ;
	    return Children[i]->search(key);
	}
	else{
		l = i + 1 ;
		return Children[i]->search(key);
	}
}

bool BTreeNode::isFull() {
    return KeysNum == 2 * minDegree - 1;
}

void BTreeNode::insertNonFull(int key) {
    int i = KeysNum - 1;

    if (isLeaf) {
        while (i >= 0 && Keys[i] > key) {
            Keys[i+1] = Keys[i];
            i--;
        }

        Keys[i + 1] = key;
        KeysNum++;
    }
    else {
        while (i >= 0 && Keys[i] > key)
            i--;

        if (Children[i + 1]->isFull()) {
            splitChild(i + 1, Children[i + 1]);

            if (Keys[i+1] < key)
                i++;
        }
        Children[i+1]->insertNonFull(key);
    }
}

void BTreeNode::splitChild(int childIndex, BTreeNode *child) {
    auto anotherChild = new BTreeNode(minDegree, child->isLeaf);

    anotherChild->KeysNum = minDegree - 1;
    std::memcpy(anotherChild->Keys    , child->Keys     + minDegree,
                ((size_t) minDegree - 1) * sizeof(int));
    std::memcpy(anotherChild->Children, child->Children + minDegree,
                ((size_t) minDegree    ) * sizeof(BTreeNode*));

    child->KeysNum = minDegree - 1;

    std::memcpy(Children + childIndex + 2, Children + childIndex + 1, ((size_t) KeysNum - childIndex) * sizeof(int));
    Children[childIndex + 1] = anotherChild;


    std::memcpy(Keys + childIndex + 1, Keys + childIndex, ((size_t) KeysNum - childIndex) * sizeof(int));
    Keys[childIndex] = child->Keys[minDegree-1];

    KeysNum++;
}

std::vector<int> BTreeNode::getElements() {
    std::vector<int> elements;

    for (int i = 0; i < KeysNum; i++) {
        if (!isLeaf){
            std::vector<int> tmp = Children[i]->getElements();
            elements.insert(elements.end(), tmp.begin(), tmp.end());
        }
        elements.push_back(Keys[i]);
    }
    if (KeysNum && !isLeaf) {
        std::vector<int> tmp = Children[KeysNum]->getElements();
        elements.insert(elements.end(), tmp.begin(), tmp.end());
    }

    return elements;
}

#define TABS for (int i = 0; i < depth; i++) std::cout << "              ";

const void BTreeNode::Dump(int depth) {
    TABS std::cout << "===BTreeNode===" << std::endl;
    //TABS std::cout << "this:    " << this << std::endl;
    TABS std::cout << (isLeaf ? "   Leaf" : "   Not Leaf") << std::endl;
    //TABS std::cout << "minDeg:  " << minDegree << std::endl;
    //TABS std::cout << "KeysNum: " << KeysNum << std::endl;

    for (int i = 0; i < KeysNum; i++) {
        if (!isLeaf)
            Children[i]->Dump(depth + 1);
        TABS std::cout << "   Keys[" << i << "]: " << Keys[i] << std::endl;
    }
    if (KeysNum && !isLeaf)
        Children[KeysNum]->Dump(depth + 1);

    TABS std::cout << "===============" << std::endl;
}
#undef TABS

BTree::BTree(int t) :
    minDegree(t),
    Root(nullptr)
{}

BTree::~BTree() {
    delete Root;
}

void BTree::insert(int key) {
    if (Root == nullptr) {
        Root = new BTreeNode(minDegree, true);
        Root->Keys[0] = key;
        Root->KeysNum = 1;
    }
    else {
        if (Root->isFull()) {
            auto newRoot = new BTreeNode(minDegree, false);

            newRoot->Children[0] = Root;
            newRoot->splitChild(0, Root);

            int i = 0;
            if (newRoot->Keys[0] < key)
                i++;
            newRoot->Children[i]->insertNonFull(key);

            Root = newRoot;
        }
        else
            Root->insertNonFull(key);
    }
}

const void BTree::Dump() {
    if (Root != nullptr)
        Root->Dump(0);
}

BTreeNode *BTree::search(int key) {
    return Root->search(key);
}

BTree* BTree::merge(BTree* anotherTree) {
    int minDegreeNew = std::max(minDegree, anotherTree->minDegree);
    auto tree = new BTree(minDegreeNew);

    std::vector<int> elements;

    auto v1 = this->getElements();
    auto v2 = anotherTree->getElements();

    std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(elements));

    for (auto e : elements)
        tree->insert(e);

    return tree;
}

std::vector<int> BTree::getElements() {
    return Root->getElements();
}
