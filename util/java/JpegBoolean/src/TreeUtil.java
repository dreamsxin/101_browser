/**
 * @(#)TreeUtil.java
 *
 *
 * @author 
 * @version 1.00 2010/3/14
 */

import java.util.*;

public class TreeUtil {

    public static <L extends List<Boolean>> TreeNode<L> createSubtree(int size, int variablesCount) {
    	if (size == 0) {
    		return new LeafTreeNode<L>(variablesCount);
    	} else {
    		return new NotTreeNode<L>(size-1, variablesCount);
    	}
    }
    
}