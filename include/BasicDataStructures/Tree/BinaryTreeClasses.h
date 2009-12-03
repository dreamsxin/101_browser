#ifndef _BinaryTreeClasses_h
#define _BinaryTreeClasses_h

#include <cstdlib>

#include "BasicDataStructures/Comparator.h"

template <typename Type, typename NodePropertyType> class BinaryTreeNode
{
protected:
	BinaryTreeNode<Type, NodePropertyType> *mpParent, *mpLeft, *mpRight;
	Type mDatum;
	NodePropertyType mNodeProperty;

public:
	inline BinaryTreeNode(
		const Type& in_datum,
		BinaryTreeNode<Type, NodePropertyType> *in_parent, 
		BinaryTreeNode<Type, NodePropertyType> *in_left=NULL,  
		BinaryTreeNode<Type, NodePropertyType> *in_right=NULL) : 
	mDatum(in_datum),
	mpParent(in_parent), 
	mpLeft(in_left), 
	mpRight(in_right) { }

	inline const Type& datum()
	{
		return mDatum;
	}

	inline NodePropertyType& nodeProperty()
	{
		return mNodeProperty;
	}

	inline BinaryTreeNode<Type, NodePropertyType>*& parent()
	{
		return mpParent;
	}

	inline BinaryTreeNode<Type, NodePropertyType>*& left()
	{
		return mpLeft;
	}

	inline BinaryTreeNode<Type, NodePropertyType>*& right()
	{
		return mpRight;
	}
};

struct InsertResult
{
	/*!
	 * if ok==true the insertion succeeded
	 *
	 * if ok==false the insertion failed and
	 * failReason stores the comparison which failed
	 */
	bool ok; 

	/*!
	 * if ok==false the reason why inserting failed (which kind of
	 * comparison result we got) is stored in failReason. 
	 * In this case (failure) failReason will
	 * never be Less or Greater.
	 * 
	 * if ok==true the value stays the same
	 */
	CompareResult failReason;

	bool operator==(const InsertResult& in_res)
	{
		return ((ok==true && in_res.ok==true) || 
			(ok==false && in_res.ok==false && failReason==in_res.failReason));
	}
};

template <typename Type> struct FindResult
{
	bool found;
	CompareResult comparison;
	Type& datum;
};

template <typename Type, typename NodePropertyType> void deleteBinaryTreeNodeTree(
	BinaryTreeNode<Type, NodePropertyType>* in_pRoot);

template <typename Type, typename NodePropertyType> class BinaryTree
{
protected:
	BinaryTreeNode<Type, NodePropertyType>* mpRoot;
	
	BinaryTree(const BinaryTree& in_binaryTree);
	BinaryTree& operator=(const BinaryTree& in_binaryTree);

	CompareResult (*mpCompare)(const Type&, const Type&);
	BinaryTreeNode<Type, NodePropertyType>* (*mpInserted)(BinaryTreeNode<Type, NodePropertyType>*);

public:
	BinaryTree(
		CompareResult (*in_pCompare)(const Type&, const Type&), 
		BinaryTreeNode<Type, NodePropertyType>* (*in_pInserted)(BinaryTreeNode<Type, NodePropertyType>*)) : 
	mpRoot(NULL),
		mpCompare(in_pCompare), 
		mpInserted(in_pInserted)
	{ }

	~BinaryTree()
	{
		deleteBinaryTreeNodeTree(root());
	}

	BinaryTreeNode<Type, NodePropertyType>*& root()
	{
		return mpRoot;
	}

	InsertResult insert(const Type& in_datum);

	template <typename OtherType> const Type* get(
		const OtherType& in_pItem,
		CompareResult (*in_pContainedIn)(const OtherType&, const Type&));
};

#endif
