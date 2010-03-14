/**
 * @(#)TreeNode.java
 *
 *
 * @author 
 * @version 1.00 2010/3/2
 */


public interface TreeNode<State> extends Incrementable, Resetable {
    public boolean computeValue(State s);
    
}