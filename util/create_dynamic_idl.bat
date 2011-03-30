set OLDPATH=%PATH%
set PATH=c:\cygwin\bin;%PATH%

wget http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt -O ../testfiles/Unicode/UTF-8-test.txt

wget http://unicode.org/Public/UNIDATA/PropList.txt -O ../data/Unicode/PropList.txt

wget http://www.whatwg.org/specs/web-apps/current-work/ -O html.html
java -cp htmlparser-1.2.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_html_idl.xslt --input-html=html.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/html.idl


wget http://www.whatwg.org/specs/web-workers/current-work/ -O webworkers.html
java -cp htmlparser-1.2.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_webworkers_idl.xslt --input-html=webworkers.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/webworkers.idl


wget http://dev.w3.org/html5/websockets/ -O websockets.html
java -cp htmlparser-1.2.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_websockets_idl.xslt --input-html=websockets.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/websockets.idl


wget http://dev.w3.org/html5/webdatabase/ -O webdatabase.html
java -cp htmlparser-1.2.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_webdatabase_idl.xslt --input-html=webdatabase.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/webdatabase.idl


wget http://dev.w3.org/html5/webstorage/ -O webstorage.html
java -cp htmlparser-1.2.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_webstorage_idl.xslt --input-html=webstorage.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/webstorage.idl


wget http://dev.w3.org/geo/api/spec-source.html -O geolocation.html
java -cp htmlparser-1.2.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_geolocation_idl.xslt --input-html=geolocation.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/idl/generated/geolocation.idl


wget http://www.w3.org/TR/xproc/ -O xproc.html
java -cp htmlparser-1.2.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_xproc_grammar.xslt --input-html=xproc.html --output-xml=../data/idl/temp_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&lt;/</" -e "s/&gt;/>/" -e "s/&gt;/>/" ../data/idl/temp_.idl > ../data/web_grammar/generated/xproc.txt


rm ../data/idl/temp_.idl
rm html.html
rm webworkers.html
rm websockets.html
rm webdatabase.html
rm webstorage.html
rm geolocation.html
rm xproc.html

set PATH=%OLDPATH%
