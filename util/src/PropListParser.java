/*
* Copyright 2012 Wolfgang Keller
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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;


public class PropListParser {
	public static final String licenseHeader = 
		"/*\n"+
"* Copyright 2012 Wolfgang Keller\n"+
"*\n"+
"* Licensed under the Apache License, Version 2.0 (the \"License\");\n"+
"* you may not use this file except in compliance with the License.\n"+
"* You may obtain a copy of the License at\n"+
"*\n"+
"*     http://www.apache.org/licenses/LICENSE-2.0\n"+
"*\n"+
"* Unless required by applicable law or agreed to in writing, software\n"+
"* distributed under the License is distributed on an \"AS IS\" BASIS,\n"+
"* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"+
"* See the License for the specific language governing permissions and\n"+
"* limitations under the License.\n"+
"*/\n\n";
	
	public static void main(String[] args) {
		if (args.length < 3) {
			System.err.println("Usage: java XsltTransform infile outfile property varname");
			System.exit(1);
		}
		
		File inFile = new File(args[0]);
		File outFile = new File(args[1]);
		String property = args[2];
		
		DataInputStream in;
		DataOutputStream out;
		
		try {
			in = new DataInputStream(new FileInputStream(inFile));
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			
			out = new DataOutputStream(new FileOutputStream(outFile));
			BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(out));

			try {
				bw.write(licenseHeader);
				bw.write("#include \"Util/Interval.hpp\"" + "\n");
				bw.write("#include \"Unicode/Unicode.h\"" + "\n\n");
				bw.write("const Interval<UnicodeCodePoint> "+property+"Intervals[] = {\n");
				
				String line;
				boolean first = true;
				
				while ((line = br.readLine()) != null) {
					if (line.length() == 0)
						continue;
					
					if (line.charAt(0) == '#')
						continue;
					
					String[] parts1 = line.split(";");
					
					if (parts1.length != 2) {
						System.err.println("Invalid format. Exiting.");
						System.exit(1);
					}
					
					String intervalStr = parts1[0].trim();
					
					String[] parts2 = parts1[1].split("#");
					
					if (parts2.length != 2) {
						System.err.println("Invalid format. Exiting.");
						System.exit(1);
					}
					
					String currentProperty = parts2[0].trim();
					
					if (currentProperty.equals(property)) {
						if (!first) {
							bw.write(", \n");
						}
						bw.write("\t{");
						
						// Two dots
						String[] intervalParts = intervalStr.split("\\x2e\\x2e");
						
						String intPart0 = null, intPart1 = null;
						
						if (intervalParts.length == 1) {
							intPart0 = intervalParts[0];
							intPart1 = intervalParts[0];
						} else if (intervalParts.length == 2) {
							intPart0 = intervalParts[0];
							intPart1 = intervalParts[1];
						} else {
							System.err.println("Invalid format. Exiting.");
							System.exit(1);
						}
						
						bw.write("0x"+intPart0+", "+"0x"+intPart1);
						
						bw.write("}");
						
						first = false;
					}
				}
				
				bw.write("\n};\n\n");
				
				bw.write("const void* get"+property+"Intervals()"+"\n");
				bw.write("{\n");
				bw.write("\treturn "+property+"Intervals;\n");
				bw.write("}\n\n");
				
				bw.write("size_t get"+property+"IntervalsCount()"+"\n");
				bw.write("{\n");
				bw.write("\treturn sizeof("+property+"Intervals) / sizeof(Interval<UnicodeCodePoint>);\n");
				bw.write("}\n");

				br.close();
				bw.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
