// Origin: http://www.oreilly.de/artikel/java_xslt_tips.html

import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamSource;
import javax.xml.transform.stream.StreamResult;
import java.io.*;

public class XsltTransform {
	public static void main(String[] args) throws Exception {
		if (args.length < 3 || (args.length % 2 != 1)) {
			System.err.println("Usage: java XsltTransform xmlfile xsltfile [param1 value1 param2 value2...] outfile");
			System.exit(1);
		}

		File xmlFile = new File(args[0]);
		File xsltFile = new File(args[1]);
		File outFile = new File(args[args.length-1]);

		// JAXP liest Daten über die Source-Schnittstelle
		Source xmlSource = new StreamSource(xmlFile);
		Source xsltSource = new StreamSource(xsltFile);

		// das Factory-Pattern unterstützt verschiedene XSLT-Prozessoren
		TransformerFactory transFact = TransformerFactory.newInstance();
		Transformer trans = transFact.newTransformer(xsltSource);

		for (int parameterIdx = 0; parameterIdx < (args.length-3)/2; parameterIdx++) {
			trans.setParameter(args[2+2*parameterIdx], args[3+2*parameterIdx]);
		}

		trans.transform(xmlSource, new StreamResult(outFile));
    }
}
