set OLDPATH=%PATH%
set PATH=c:\cygwin\bin;%PATH%

wget http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt -O ../testfiles/Unicode/UTF-8-test.txt

wget http://unicode.org/Public/UNIDATA/PropList.txt -O ../data/Unicode/PropList.txt

wget http://matroska.svn.sourceforge.net/viewvc/matroska/trunk/foundation_src/spectool/specdata.xml?revision=HEAD -O ../data/matroska/specdata.xml


wget http://www.whatwg.org/specs/web-apps/current-work/complete.html -O temp.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.HTML2XML temp.html temp.xml
java -cp out XsltTransform temp.xml create_idl.xslt namespace webapps ../data/idl/generated/whatwg_webapps.idl
dos2unix ../data/idl/generated/whatwg_webapps.idl


wget http://www.whatwg.org/specs/web-apps/current-work/ -O temp.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.HTML2XML temp.html temp.xml
java -cp out XsltTransform temp.xml create_idl.xslt namespace html ../data/idl/generated/whatwg_html.idl
dos2unix ../data/idl/generated/whatwg_html.idl


wget  http://dev.w3.org/html5/spec/Overview.html -O temp.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.HTML2XML temp.html temp.xml
java -cp out XsltTransform temp.xml create_idl.xslt namespace html5 ../data/idl/generated/w3c_html5.idl
dos2unix ../data/idl/generated/w3c_html5.idl


wget http://dev.w3.org/geo/api/spec-source.html -O temp.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.HTML2XML temp.html temp.xml
java -cp out XsltTransform temp.xml create_idl.xslt namespace geo ../data/idl/generated/geolocation.idl
dos2unix ../data/idl/generated/geolocation.idl


REM wget http://dvcs.w3.org/hg/IndexedDB/raw-file/tip/Overview.html -O temp.html
REM java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.HTML2XML temp.html temp.xml
REM java -cp out XsltTransform temp.xml create_idl.xslt namespace indexeddb ../data/idl/generated/indexeddb.idl
REM dos2unix ../data/idl/generated/indexeddb.idl


wget http://www.w3.org/TR/xproc/ -O temp.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.HTML2XML temp.html temp.xml
java -cp out XsltTransform temp.xml create_xproc_grammar.xslt ../data/web_grammar/generated/xproc.txt
dos2unix ../data/web_grammar/generated/xproc.txt



rm temp.html
rm temp.xml

set PATH=%OLDPATH%
