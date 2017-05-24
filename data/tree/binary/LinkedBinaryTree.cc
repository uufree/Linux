/*************************************************************************
	> File Name: LinkedBinaryTree.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月24日 星期三 00时57分36秒
 ************************************************************************/

#include"LinkedBinaryTree.h"

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree() : boot(nullptr),   theTreeSize(0),theHigh(0)
{};

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree(const LinkedBinaryTree<T>& lhs)
{
}

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree(LinkedBinaryTree&& lhs)
{
}

template<typename T>
LinkedBinaryTree<T>& LinkedBinaryTree<T>::operator=(const LinkedBinaryTree<T>& lhs)
{
}

template<typename T>
LinkedBinaryTree<T>& LinkedBinaryTree<T>::operator=(LinkedBinaryTree<T>&& lhs)
{
}

template<typename T>
LinkedBinaryTree<T>::~LinkedBinaryTree()
{
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
{}























