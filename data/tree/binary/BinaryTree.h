/*************************************************************************
	> File Name: BinaryTree.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月24日 星期三 00时11分52秒
 ************************************************************************/

#ifndef _BINARYTREE_H
#define _BINARYTREE_H

#include<functional>
#include<iostream>

template<typename T>
struct BinaryTreeNode
{
    T data;
    struct BinaryTreeNode<T>* leftChild;
    struct BinaryTreeNode<T>* rightChild;

    BinaryTreeNode() : leftChild(nullptr),rightChild(nullptr)
    {};
    
    BinaryTreeNode(const T& data_) : data(data_),leftChild(nullptr),rightChild(nullptr)
    {};
};


template<typename T>
class BinaryTree
{
    typedef std::function<void(T*)> CallBack;
    
    public:
        virtual ~BinaryTree();
        virtual bool empty() const = 0;
        virtual int size() const = 0;
        virtual int hight() const = 0; 
        virtual void perOrder(const CallBack& cb) = 0;
        virtual void inOrder(const CallBack& cb) = 0;
        virtual void postOrder(const CallBack& cb) = 0;
        virtual void levelOrder(const CallBack& cb) = 0;
};


#endif
