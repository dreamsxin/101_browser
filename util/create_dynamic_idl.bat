set OLDPATH=%PATH%
set PATH=c:\cygwin\bin;%PATH%

wget http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt -O ../testfiles/Unicode/UTF-8-test.txt

wget http://unicode.org/Public/UNIDATA/PropList.txt -O ../data/Unicode/PropList.txt

wget http://matroska.svn.sourceforge.net/viewvc/matroska/trunk/foundation_src/spectool/specdata.xml?revision=HEAD -O ../data/matroska/specdata.xml


CALL create_single_dynamic_idl.bat http://www.whatwg.org/specs/web-apps/current-work/complete.html webapps ../data/idl/generated/whatwg_webapps.idl

CALL create_single_dynamic_idl.bat http://www.whatwg.org/specs/web-apps/current-work/ html ../data/idl/generated/whatwg_html.idl

CALL create_single_dynamic_idl.bat http://dev.w3.org/html5/spec/Overview.html html5 ../data/idl/generated/w3c_html5.idl
CALL create_single_dynamic_idl.bat http://dev.w3.org/html5/md/ microdata ../data/idl/generated/w3c_microdata.idl
CALL create_single_dynamic_idl.bat http://dev.w3.org/html5/2dcontext/ canvas ../data/idl/generated/w3c_canvas.idl
CALL create_single_dynamic_idl.bat http://dev.w3.org/html5/postmsg/ webmessaging ../data/idl/generated/w3c_webmessaging.idl
CALL create_single_dynamic_idl.bat http://dev.w3.org/html5/workers/ webworkers ../data/idl/generated/w3c_webworkers.idl
CALL create_single_dynamic_idl.bat http://dev.w3.org/html5/webstorage/ webstorage ../data/idl/generated/w3c_webstorage.idl
CALL create_single_dynamic_idl.bat http://dev.w3.org/html5/websockets/ websockets ../data/idl/generated/w3c_websockets.idl
CALL create_single_dynamic_idl.bat http://dev.w3.org/html5/eventsource/ serversent  ../data/idl/generated/w3c_serversent.idl

CALL create_single_dynamic_idl.bat http://dev.w3.org/geo/api/spec-source.html geo ../data/idl/generated/geolocation.idl

CALL create_single_dynamic_idl.bat http://dvcs.w3.org/hg/domcore/raw-file/tip/Overview.html domcore ../data/idl/generated/domcore.idl

CALL create_single_dynamic_idl.bat http://dev.w3.org/csswg/cssom/ cssom ../data/idl/generated/cssom.idl
CALL create_single_dynamic_idl.bat http://dev.w3.org/csswg/cssom-view/ cssom ../data/idl/generated/cssom-view-module.idl

CALL create_single_dynamic_idl.bat http://dev.w3.org/2006/webapi/FileAPI/ fileapi ../data/idl/generated/fileapi.idl

CALL create_single_dynamic_idl.bat http://www.w3c-test.org/webperf/specs/NavigationTiming/ navigationtiming ../data/idl/generated/navigationtiming.idl

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
