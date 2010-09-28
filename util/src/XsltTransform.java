// Origin: http://www.oreilly.de/artikel/java_xslt_tips.html

import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamSource;
import javax.xml.transform.stream.StreamResult;
import java.io.*;

public class XsltTransform {

	/**
	* XSLT-Transformation durchführen und Ergebnis
	* an System.out schicken.
	*/
	public static void main(String[] args) throws Exception {
		if (args.length != 3) {
			System.err.println("Usage: java Transform xmlfile xsltfile outfile");
			System.exit(1);
		}

		File xmlFile = new File(args[0]);
		File xsltFile = new File(args[1]);
		File outFile = new File(args[2]);

		// JAXP liest Daten über die Source-Schnittstelle
		Source xmlSource = new StreamSource(xmlFile);
		Source xsltSource = new StreamSource(xsltFile);

		// das Factory-Pattern unterstützt verschiedene XSLT-Prozessoren
		TransformerFactory transFact = TransformerFactory.newInstance();
		Transformer trans = transFact.newTransformer(xsltSource);

		trans.transform(xmlSource, new StreamResult(outFile));
    }
}