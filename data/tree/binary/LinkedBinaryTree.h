/*************************************************************************
	> File Name: LinkedBinaryTree.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月24日 星期三 00时24分33秒
 ************************************************************************/

#ifndef _LINKEDBINARYTREE_H
#define _LINKEDBINARYTREE_H

#include"BinaryTree.h"

template<typename T>
class LinkedBinaryTree : public BinaryTree<T>
{
    typedef std::function<void(BinaryTreeNode<T>*)> CallBack;

    public:
        LinkedBinaryTree();
        LinkedBinaryTree(const LinkedBinaryTree<T>& lhs);
        LinkedBinaryTree(LinkedBinaryTree<T>&& lhs);
        LinkedBinaryTree<T>& operator=(const LinkedBinaryTree<T>& lhs);
        LinkedBinaryTree<T>& operator=(LinkedBinaryTree<T>&& lhs);
        ~LinkedBinaryTree();

        bool empty() const override
        {return theTreeSize == 0;};

        int size() const override
        {return theTreeSize;};
        
        int hight() const override
        {return theHigh;};
        
        void perOrder(const CallBack& cb = std::bind(&LinkedBinaryTree::visit,std::placeholders::_1)) override
        {
            callback = cb;
            thePerOrder(boot);
        }
        
        void inOrder(const CallBack& cb = std::bind(&LinkedBinaryTree::visit,std::placeholders::_1)) override
        {
            callback = cb;
            theInOrder(boot);
        }
        
        void postOrder(const CallBack& cb = std::bind(&LinkedBinaryTree::visit,std::placeholders::_1)) override
        {
            callback = cb;
            thePostOrder(boot);
        }

        void levelOrder(const CallBack& cb = std::bind(&LinkedBinaryTree::visit,std::placeholders::_1)) override
        {
            callback = cb;
            theLevelOrder(boot);
        }
    
    private:
        void thePerOrder(BinaryTreeNode<T>* ptr);
        void theInOrder(BinaryTreeNode<T>* ptr);
        void thePostOrder(BinaryTreeNode<T>* ptr);
        void theLevelOrder(BinaryTreeNode<T>* ptr);
        BinaryTreeNode<T>* theCopy(BinaryTreeNode<T>* ptr);
        
        void visit(const BinaryTreeNode<T>& lhs)
        {std::cout << lhs->data << std::endl;}

    private:
        BinaryTreeNode<T>* boot;
        int theTreeSize;
        int theHigh;
        CallBack callback;
};



#endif
