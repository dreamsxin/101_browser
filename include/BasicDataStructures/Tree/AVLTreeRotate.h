/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _AVLTreeRotate_h
#define _AVLTreeRotate_h

#include <algorithm>
#include "BasicDataStructures/Tree/AVLTree.h"

/*
 * _General remarks concerning the correctness proofs_
 *
 * Let b(·) denote the balance of a node and let d(·) 
 * denote the depth of the tree rooted in the node parameter of d.
 *
 * The balance b(Y) of a node Y with subnodes X and Z (which may be null)
 *
 *           Y
 *          / \
 *         X   Z
 *
 * is defined as
 * b(Y) := d(Z)-d(X)         (I)
 *
 * and the depth of a node Y with subnodes X and Z (which may be null) - same situation
 * is defined as
 * d(Y) := 1+max(d(X), d(Z)) (II)
 * where
 * d(null) := 0              (III)
 */


/* 
 * We only consider the case where NodePropertyType is AVLTreeNodePropertyType
 * But here we formulate it the general way
 *
 *          3
 *         / \
 *        1   4
 *       / \   
 *      0   2
 *
 * Parameters:
 * in_p1Balance: Pointer to the balance of node 1
 * in_p3Balance: Pointer to the balance of node 3
 *
 * Return values
 *
 *          1'
 *         / \
 *        0   3'
 *           / \    
 *          2   4
 *
 * (the ' denotes that that the balancing of 1 and 3 has changed)
 *
 * in_p1Balance: contains the "new" balance of node 1 (=balance of node 1')
 * in_p3Balance: contains the "new" balance of node 3 (=balance of node 3')
 * 
 * _Proof of correctness of the algorithm_
 *
 * We want to identify the numbers of the nodes with the nodes themself.
 *
 * Then we have:
 *
 * b(1) = d(2)-d(0)               (IV)  (by (I))
 * b(3) = d(4)-d(1)                     (by (I))
 *      = d(4)-1-max(d(0), d(2))  (V)   (by (II))
 * 
 * b(1') = d(3')-d(0)                   (by (I))
 *       = 1+max(d(2), d(4))-d(0) (VI)  (by (II))
 * b(3') = d(4)-d(2)              (VII) (by (I))
 *
 * From (V) we get
 * d(4) = b(3)+1+max(d(0), d(2))  (VIII)
 *
 * So we have
 * b(3') = d(4)-d(2)                                    (by (VII))
 *       = b(3)+1+max(d(0), d(2))-d(2)                  (by (VIII))
 *       = b(3)+1+1/2*(d(0)+d(2)+|d(2)-d(0)|)-d(2)      (since max(x, y)=1/2*(x+y+|x-y|) (*) as you can check easily)
 *       = b(3)+1+1/2*(d(0)-d(2)+|d(2)-d(0)|)
 *       = b(3)+1+1/2*(-b(1)+|b(1)|)                    (by (IV))
 *       = b(3)+1-1/2*(b(1)+0-|b(1)-0|)
 *       = b(3)+1-min(0, b(1))                     (IX) (since min(x, y)=1/2*(x+y-|x-y|) (**) as you can check easily)
 * and
 * b(1') = 1+max(d(2), d(4))-d(0)                       (by (VI))
 *       = 1+max(0, d(4)-d(2))+d(2)-d(0)                (since max(x, y)=z+max(x-z, y-z), which follows from (*))
 *       = d(2)-d(0)+1+max(0, d(4)-d(2))
 *       = b(1)+1+max(0, b(3'))                    (X)  (by (IV) and (VII))
 *
 * from (IX) and (X) we follow that
 * b(3) <- b(3)+1-min(0, b(1))
 * b(1) <- b(1)+1+max(0, b(3))    (b(3) stores now the value b(3'))
 * stores the only solutions of the equations (IV)-(VII) as new values
 */
template <typename NodePropertyType> void generateRotateRightBalances(
	NodePropertyType* in_p1Balance, NodePropertyType* in_p3Balance)
{
	*in_p3Balance = *in_p3Balance+1-std::min<NodePropertyType>(0, *in_p1Balance);
	*in_p1Balance = *in_p1Balance+1+std::max<NodePropertyType>(0, *in_p3Balance);
}

/* 
 * We only consider the case where NodePropertyType is AVLTreeNodePropertyType
 * But here we formulate it the general way
 *
 *          1
 *         / \
 *        0   3
 *           / \    
 *          2   4
 *
 * Parameters:
 * in_p1Balance: Pointer to the balance of node 1
 * in_p3Balance: Pointer to the balance of node 3
 *
 * Return values
 *
 *          3'
 *         / \
 *        1'  4
 *       / \   
 *      0   2
 *
 * (the ' denotes that that the balancing of 1 and 3 has changed)
 *
 * in_p1Balance: contains the "new" balance of node 1 (=balance of node 1')
 * in_p3Balance: contains the "new" balance of node 3 (=balance of node 3')
 * 
 * _Proof of correctness of the algorithm_
 *
 * We want to identify the numbers of the nodes with the nodes themself.
 *
 * Then we have:
 *
 * b(1) = d(3)-d(0)              ´      (by (I))
 *       = 1+max(d(2), d(4))-d(0) (IV)  (by (II))
 * b(3) = d(4)-d(2)               (V)   (by (I))
 * 
 * b(1') = d(2)-d(0)              (VI)  (by (I))
 * b(3') = d(4)-d(1')                   (by (I))
 *       = d(4)-1-max(d(0), d(2)) (VII) (by (II))
 *
 * From (IV) we get
 * d(0) = 1+max(d(2), d(4))-b(1)  (VIII)
 *
 * So we have
 * b(1') = d(2)-d(0)                                    (by (VI))
 *       = d(2)-1-max(d(2), d(4))+b(1)                  (by (VIII))
 *       = d(2)-1-1/2*(d(2)+d(4)+|d(4)-d(2)|)+b(1)      (since max(x, y)=1/2*(x+y+|x-y|) (*) as you can check easily)
 *       = b(1)-1-1/2*(d(4)-d(2)+|d(4)-d(2)|)
 *       = b(1)-1-1/2*(b(3)+|b(3)|)                     (by (V))
 *       = b(1)-1-1/2*(b(3)+0+|b(3)-0|)
 *       = b(1)-1-max(0, b(3))                     (IX) (by (*))
 * and
 * b(3') = d(4)-1-max(d(0), d(2))                       (by (VII))
 *       = d(4)-1-(max(d(0)-d(2), 0)+d(2))              (since max(x, y)=z+max(x-z, y-z), which follows from (*))
 *       = d(4)-d(2)-1-max(d(0)-d(2), 0)
 *       = d(4)-d(2)-1-1/2*(d(0)-d(2)+0+|d(0)-d(2)-0|)  (by (*))
 *       = d(4)-d(2)-1+1/2*(d(2)-d(0)+0-|d(2)-d(0)-0|)  (follows from the properties of |·|)
 *       = d(4)-d(2)-1+min(0, d(2)-d(0))                (since min(x, y)=1/2*(x+y-|x-y|) (*) as you can check easily)
 *       = b(3)-1+min(0, b(1'))                    (X)  (by (V) and (VI))
 *
 * from (IX) and (X) we follow that
 * b(1) <- b(1)-1-max(0, b(3))
 * b(3) <- b(3)-1+min(0, b(1))    (b(1) stores now the value b(1'))
 * stores the only solutions of the equations (IV)-(VII) as new values
 */
template <typename NodePropertyType> void generateRotateLeftBalances(
	NodePropertyType* in_p1Balance, NodePropertyType* in_p3Balance)
{
	*in_p1Balance = *in_p1Balance-1-std::max<NodePropertyType>(0, *in_p3Balance);
	*in_p3Balance = *in_p3Balance-1+std::min<NodePropertyType>(0, *in_p1Balance);
}

// Missing implementation
template <typename Type> BinaryTreeNode<Type, AVLTreeNodePropertyType>* rotateAVLTreeRight(
	BinaryTreeNode<Type, AVLTreeNodePropertyType>* in_pNode)
{
	BinaryTreeNode<Type, AVLTreeNodePropertyType>* pNode3=in_pNode;
	assert(pNode3 != NULL);

	if (pNode3 == NULL)
		return NULL;

	BinaryTreeNode<Type, AVLTreeNodePropertyType>* pNode1=in_pNode->left();
	assert(pNode1 != NULL);

	if (pNode1 == NULL)
		return NULL;

	generateRotateRightBalances(&pNode1->nodeProperty(), &pNode3->nodeProperty());

	return rotateRight(in_pNode);
}

// Missing implementation
template <typename Type> BinaryTreeNode<Type, AVLTreeNodePropertyType>* rotateAVLTreeLeft(
	BinaryTreeNode<Type, AVLTreeNodePropertyType>* in_pNode)
{
	BinaryTreeNode<Type, AVLTreeNodePropertyType>* pNode1=in_pNode;
	assert(pNode1 != NULL);

	if (pNode1 == NULL)
		return NULL;

	BinaryTreeNode<Type, AVLTreeNodePropertyType>* pNode3=in_pNode->right();
	assert(pNode3 != NULL);

	if (pNode3 == NULL)
		return NULL;

	generateRotateLeftBalances(&pNode1->nodeProperty(), &pNode3->nodeProperty());

	return rotateLeft(in_pNode);
}

#endif
