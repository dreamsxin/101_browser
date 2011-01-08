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

#include "BasicDataStructures/Tree/AVLTree.h"
#include "TestSuite/TestSuite.h"

// WARNING: VERY SLOW
template <typename Type, typename NodePropertyType> size_t depth(
	BinaryTreeNode<Type, NodePropertyType>* in_pNode)
{
	if (in_pNode==NULL)
		return 0;
	else
		// set std::max into parantheses to avoid conflict with max from
		// windef.h under Windows
		return 1+(std::max)(depth(in_pNode->left()), depth(in_pNode->right()));
}

template <typename Type> void testAVLPropertyRecursively(
	BinaryTreeNode<Type, AVLTreeNodePropertyType>* in_pNode)
{
	if (!in_pNode)
		return;

	signed long leftDepth = depth(in_pNode->left());
	signed long rightDepth = depth(in_pNode->right());

	test(rightDepth-leftDepth==in_pNode->nodeProperty());

	test(in_pNode->nodeProperty()>=-2);
	test(in_pNode->nodeProperty()<=2);

	testAVLPropertyRecursively(in_pNode->left());
	testAVLPropertyRecursively(in_pNode->right());
}

template <typename Type, typename NodePropertyType> void testValidPointersInTreeRecursively(
	BinaryTreeNode<Type, NodePropertyType>* in_pNode)
{
	assert(in_pNode);

	if (in_pNode == NULL)
		return;
	
	if (in_pNode->left() != NULL)
	{
		test(in_pNode->left()->parent()==in_pNode);
		testValidPointersInTreeRecursively(in_pNode->left());
	}

	if (in_pNode->right() != NULL)
	{
		test(in_pNode->right()->parent()==in_pNode);
		testValidPointersInTreeRecursively(in_pNode->right());
	}
}

template <typename Type> void testAllAVLTreePropertiesRecursively(
	BinaryTreeNode<Type, AVLTreeNodePropertyType>* in_pRootNode)
{
	test(in_pRootNode->parent()==NULL);
	testValidPointersInTreeRecursively(in_pRootNode);
	testAVLPropertyRecursively(in_pRootNode);
}

void testBasicDataStructuresTree()
{
	AVLTreeDefiner<int>::AVLTree avlTree(&extendedCompare, &avlTreeInserted);

	InsertResult failEqual={false, ExtendedCompareResultEqual};

	BinaryTreeNode<int, AVLTreeNodePropertyType>* root;
	BinaryTreeNode<int, AVLTreeNodePropertyType>* other;

	test(avlTree.insert(32).ok==true);
	testAllAVLTreePropertiesRecursively(avlTree.root());
	root=avlTree.root();

	test(avlTree.insert(32)==failEqual);
	testAllAVLTreePropertiesRecursively(avlTree.root());

	test(avlTree.insert(25).ok==true);
	testAllAVLTreePropertiesRecursively(avlTree.root());

	test(avlTree.insert(25)==failEqual);
	testAllAVLTreePropertiesRecursively(avlTree.root());

	other=avlTree.root()->left();
	test(avlTree.insert(23).ok==true);
	test(avlTree.root()==other);
	testAllAVLTreePropertiesRecursively(avlTree.root());

	root=avlTree.root();
	test(avlTree.insert(21).ok==true);
	test(avlTree.root()==root);
	testAllAVLTreePropertiesRecursively(avlTree.root());

	root=avlTree.root();
	test(avlTree.insert(19).ok==true);
	testAllAVLTreePropertiesRecursively(avlTree.root());
	test(avlTree.root()==root);

	AVLTreeNodePropertyType balance1;
	AVLTreeNodePropertyType balance3;

	balance1=-1;
	balance3=-2;

	generateRotateRightBalances(&balance1, &balance3);
	test(balance1==0);
	test(balance3==0);

	balance3=1;
	balance1=2;
	generateRotateLeftBalances(&balance1, &balance3);
	test(balance1==0);
	test(balance3==0);

	signed char bal1, bal3;

	for (signed char i=-10; i<=10; i++)
		for (signed char j=-10; j<=10; j++)
		{
			// Part 1 (first right, then left)
			bal1=i;
			bal3=j;

			generateRotateRightBalances(&bal1, &bal3);
			generateRotateLeftBalances(&bal1, &bal3);

			test(bal1==i);
			test(bal3==j);

			// Part 2 (first left, then right)
			bal1=i;
			bal3=j;

			generateRotateLeftBalances(&bal1, &bal3);
			generateRotateRightBalances(&bal1, &bal3);

			test(bal1==i);
			test(bal3==j);
		}
}
