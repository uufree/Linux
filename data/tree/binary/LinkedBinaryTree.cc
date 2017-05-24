/*************************************************************************
	> File Name: LinkedBinaryTree.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月24日 星期三 00时57分36秒
 ************************************************************************/

#include"LinkedBinaryTree.h"
#include<deque>

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree() : boot(nullptr),   theTreeSize(0),theHigh(0)
{};

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree(const LinkedBinaryTree<T>& lhs) : theTreeSize(lhs.theTreeSize),theHigh(lhs.theHigh)
{
    boot = theCopy(lhs.boot);
}

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree(LinkedBinaryTree&& lhs) : boot(std::move(lhs.boot)),theTreeSize(lhs.theTreeSize),theHigh(lhs.theHigh)
{}

template<typename T>
LinkedBinaryTree<T>& LinkedBinaryTree<T>::operator=(const LinkedBinaryTree<T>& lhs)
{
    postOrder([this](BinaryTreeNode<T>* ptr){delete ptr;});
    theTreeSize = lhs.theTreeSize;
    theHigh = lhs.theHigh;
    boot = theCopy(lhs.boot);
}

template<typename T>
LinkedBinaryTree<T>& LinkedBinaryTree<T>::operator=(LinkedBinaryTree<T>&& lhs)
{
    postOrder([this](BinaryTreeNode<T>* ptr){delete ptr;});
    theTreeSize = lhs.theTreeSize;
    theHigh = lhs.theHigh;
    boot = std::move(lhs.boot);
}

template<typename T>
LinkedBinaryTree<T>::~LinkedBinaryTree()
{
    postOrder([this](BinaryTreeNode<T>* ptr){delete ptr;});
}


template<typename T>
void LinkedBinaryTree<T>::thePerOrder(BinaryTreeNode<T>* ptr)
{
    if(ptr != nullptr)
    {
        callback(ptr);
        thePerOrder(ptr->leftChild);
        thePerOrder(ptr->rightChild);
    }
}

template<typename T>
void LinkedBinaryTree<T>::theInOrder(BinaryTreeNode<T>* ptr)
{
    if(ptr != nullptr)
    {
        theInOrder(ptr->leftChild);
        callback(ptr);
        theInOrder(ptr->rightChild);
    }
}

template<typename T>
void LinkedBinaryTree<T>::thePostOrder(BinaryTreeNode<T>* ptr)
{
    if(ptr != nullptr)
    {
        thePostOrder(ptr->leftChild);
        thePostOrder(ptr->rightChild);
        callback(ptr);
    }
}

template<typename T>
void LinkedBinaryTree<T>::theLevelOrder(BinaryTreeNode<T>* ptr)
{
    std::deque<BinaryTreeNode<T>*> nodeList;
    nodeList.push_back(ptr);

    while(ptr != nullptr)
    {
        if(ptr->leftChild != nullptr)
            nodeList.push_back(ptr->leftChild);
        if(ptr->rightChild != nullptr)
            nodeList.push_back(ptr->rightChild);
        
        callback(ptr);
        nodeList.pop_front();
        ptr = nodeList.front();
    }
}

template<typename T>
BinaryTreeNode<T>* LinkedBinaryTree<T>::theCopy(BinaryTreeNode<T>* ptr)
{
    BinaryTreeNode<T>* newNode;

    if(ptr != nullptr)
        return nullptr;
    else
    {
        newNode = new BinaryTreeNode<T>;
        newNode->data = ptr->data;
        newNode->leftChild = theCopy(ptr->leftChild);
        newNode->rightChild = theCopy(ptr->rightChild);
        return newNode;
    }
}






















