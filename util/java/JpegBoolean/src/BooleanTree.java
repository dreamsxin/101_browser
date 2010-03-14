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
	
    public BooleanTree() {
    	root = new LeafTreeNode<L>(TreeUtil.VARIABLES_COUNT);
    }
    
	/**
	 * Method increment
	 *
	 *
	 * @return
	 *
	 */
	public boolean increment() {
		if (!root.increment()) {
			root = new BinaryTreeNode<L>(root.size(), TreeUtil.VARIABLES_COUNT);
		}
		
		return true;
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