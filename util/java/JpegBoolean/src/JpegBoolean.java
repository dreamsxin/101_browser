/**
 * @(#)JpegBoolean.java
 *
 * JpegBoolean application
 *
 * @author Wolfgang Keller
 * @version 1.00 2010/3/14
 */
 
import java.util.*;
 
public class JpegBoolean {
    class Point
	{
		byte row, col;
		
		Point(byte row, byte col) {
			this.row = row;
			this.col = col;
		}
	}
	
	byte pointToByteRowCol(Point p) {
		return (byte) ((p.row<<3)+p.col);
	}
	
	byte pointToByteColRow(Point p) {
		return (byte) ((p.col<<3)+p.row);
	}
	
	String byteTo6BitString(byte b)
	{
		StringBuilder builder=new StringBuilder(6);
		for (int i=0; i<6; i++)
		{
			builder.append((b & (1<<(5-i)))!=0 ? "1"  : "0");
		}
		
		return builder.toString();
	}
	
	ArrayList<Boolean> byteTo6Bits(byte b)
	{
		ArrayList<Boolean> out=new ArrayList<Boolean>(6);
		for (int i=0; i<6; i++)
		{
			out.add((b & (1<<i))!=0);
		}
		return out;
	}
	
	public Point[] createZigZag() {
		int currentDiag = 0;
		int currentPos = 0;
		Point[] zigZagPos=new Point[64];
		
		byte currentRow = 0;
		byte currentCol = 0;
		
		while (true) {
			zigZagPos[currentPos] = new Point(currentRow, currentCol);
			
			currentPos++;
			
			if ((currentRow == 7 && currentCol==7))
				break;
			
			if (currentDiag % 2 == 0) {
				if (currentRow == 0)
				{
					currentCol++;
					currentDiag++;
				}
				else if (currentCol == 7)
				{
					currentRow++;
					currentDiag++;
				}
				else {
					currentCol++;
					currentRow--;
				}
			}
			else {
				if (currentRow == 7)
				{
					currentCol++;
					currentDiag++;
				}
				else if (currentCol == 0)
				{
					currentRow++;
					currentDiag++;
				}
				else {
					currentCol--;
					currentRow++;
				}
			}
		}
		
		return zigZagPos;
	}
    
    public static void main(String[] args) {
    	JpegBoolean z = new JpegBoolean();
    	Point[] zigzagPoints = z.createZigZag();
    	
    	ArrayList<ArrayList<Boolean>> variableValues=new ArrayList<ArrayList<Boolean>>();
    	// contains 6 lists of function values
    	ArrayList<ArrayList<Boolean>> functionValues=new ArrayList<ArrayList<Boolean>>();
    	
    	for (byte i=0; i<6; i++)
    	{
    		functionValues.add(new ArrayList<Boolean>());
    	}
    	
    	for (byte i=0; i<zigzagPoints.length; i++)
    	{
    		variableValues.add(z.byteTo6Bits(i));
    		
    		byte currentZigZagByte = z.pointToByteRowCol(zigzagPoints[i]);
    		ArrayList<Boolean> currentValue = z.byteTo6Bits(currentZigZagByte);
    		
    		for (byte j=0; j<6; j++)
    		{
    			functionValues.get(j).add(currentValue.get(j));
    		}
    		
    		System.out.println(z.byteTo6BitString(i)+":\t"+z.byteTo6BitString(currentZigZagByte));
    	}
    }
}
