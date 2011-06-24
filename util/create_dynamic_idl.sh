#!/bin/sh

wget http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt -O ../testfiles/Unicode/UTF-8-test.txt

wget http://unicode.org/Public/UNIDATA/PropList.txt -O ../data/Unicode/PropList.txt

wget http://matroska.svn.sourceforge.net/viewvc/matroska/trunk/foundation_src/spectool/specdata.xml?revision=HEAD -O ../data/matroska/specdata.xml


wget http://www.whatwg.org/specs/web-apps/current-work/complete.html -O complete.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_whatwg_webapps_idl.xslt --input-html=complete.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/whatwg_webapps.idl


wget http://www.whatwg.org/specs/web-apps/current-work/ -O whatwg_html.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_whatwg_html_idl.xslt --input-html=whatwg_html.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/whatwg_html.idl


wget http://www.whatwg.org/specs/web-apps/current-work/ -O w3c_html5.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_w3c_html5_idl.xslt --input-html=w3c_html5.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/w3c_html5.idl


wget http://dev.w3.org/geo/api/spec-source.html -O geolocation.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_geolocation_idl.xslt --input-html=geolocation.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/geolocation.idl


wget http://www.w3.org/TR/xproc/ -O xproc.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_xproc_grammar.xslt --input-html=xproc.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&lt;/</" -e "s/&gt;/>/" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/web_grammar/generated/xproc.txt





rm ../data/idl/temp_.idl
rm complete.html
rm whatwg_html.html
rm geolocation.html
rm xproc.html
