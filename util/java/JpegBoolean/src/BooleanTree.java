/**
 * @(#)BooleanTree.java
 *
 *
 * @author 
 * @version 1.00 2010/3/14
 */
import java.util.*;

public class BooleanTree<L extends List<Boolean>> implements TreeNode<L> {
	public TreeNode<L> root;
	public static final int VARIABLES_COUNT = 6;

    public BooleanTree() {
    	root = new LeafTreeNode<L>(VARIABLES_COUNT);
    }
    
	/**
	 * Method increment
	 *
	 *
	 * @return
	 *
	 */
	public boolean increment() {
		if (root.increment())
			return true;
		
		return incrementTree();
	}
	
	public boolean incrementTree() {
		Stack<TreeNode<L>> stack=new Stack<TreeNode<L>>();
		
		TreeNode<L> current = root;
		
		// Build the stack
		while (true) {
			if (current instanceof NotTreeNode) {
				current = ((NotTreeNode<L>) current).child;
			} else if (current instanceof BinaryTreeNode) {
				current = ((BinaryTreeNode<L>) current).child0;
			} else {
				break;
			}
			
			stack.push(current);
		}
		
		// Now we use the stack
		while (true) {
			if (!stack.empty()) {
				current = stack.pop();
				if (current instanceof NotTreeNode) {
					if (stack.empty()) {
						root = new BinaryTreeNode<L>(current.size(), VARIABLES_COUNT);
					} else {
						TreeNode<L> parent = stack.pop();
						if (parent instanceof NotTreeNode) {
							((NotTreeNode<L>) parent).child = TreeUtil.createSubtree(current.size(), VARIABLES_COUNT);
						} else if (parent instanceof BinaryTreeNode) {
							((BinaryTreeNode<L>) parent).child0 = TreeUtil.createSubtree(current.size(), VARIABLES_COUNT);
						}
					}
					
					return true;
				} else if (current instanceof BinaryTreeNode) {
					BinaryTreeNode<L> currBin = (BinaryTreeNode<L>) current;
					int currentSize = currBin.child0.size();
					
					if (currentSize != 0) {
						currBin.child0 = TreeUtil.createSubtree(currentSize-1, VARIABLES_COUNT);
						currBin.child1 = TreeUtil.createSubtree(currentSize+1, VARIABLES_COUNT);
						return true;
					} else {
						continue;
					}
				}
			} else {
				int size = root.size();
				root = new NotTreeNode<L>(size, VARIABLES_COUNT);
				return true;
			}
		}
	}

	/**
	 * Method computeValue
	 *
	 *
	 * @param s
	 *
	 * @return
	 *
	 */
	public boolean computeValue(L list) {
		return root.computeValue(list);
	}

	/**
	 * Method size
	 *
	 *
	 * @return
	 *
	 */
	public int size() {
		return root.size();
	}
	
	public void reset()	{
		root.reset();
	}
	
	public String toString() {
		return root.toString();
	}   
}