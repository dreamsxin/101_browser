/**
 * @(#)BooleanTree.java
 *
 *
 * @author 
 * @version 1.00 2010/3/14
 */
import java.util.*;

public class BooleanTree<L extends List<Boolean>> implements TreeNode<L> {
	TreeNode<L> root;

    public BooleanTree() {
    	root = new LeafTreeNode(6);
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
		
		return incrementTree(root);
	}
	
	public static <L extends List<Boolean>> boolean incrementTree(TreeNode<L> node) {
		// TODO we have to restructure the tree
		
		return false;
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
    
}