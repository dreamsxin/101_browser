#!/bin/sh

wget http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt -O ../testfiles/Unicode/UTF-8-test.txt

wget http://unicode.org/Public/UNIDATA/PropList.txt -O ../util/data/PropList.txt

wget http://matroska.svn.sourceforge.net/viewvc/matroska/trunk/foundation_src/spectool/specdata.xml?revision=HEAD -O ../data/matroska/specdata.xml


./create_single_dynamic_idl.sh http://www.whatwg.org/specs/web-apps/current-work/complete.html webapps ../data/idl/generated/whatwg_webapps.idl

./create_single_dynamic_idl.sh http://www.whatwg.org/specs/web-apps/current-work/ html ../data/idl/generated/whatwg_html.idl

./create_single_dynamic_idl.sh http://dev.w3.org/html5/spec/Overview.html html5 ../data/idl/generated/w3c_html5.idl
./create_single_dynamic_idl.sh http://dev.w3.org/html5/md/ microdata ../data/idl/generated/w3c_
./create_single_dynamic_idl.sh http://dev.w3.org/html5/2dcontext/ canvas ../data/idl/generated/w3c_canvas.idl
./create_single_dynamic_idl.sh http://dev.w3.org/html5/postmsg/ webmessaging ../data/idl/generated/w3c_webmessaging.idl
./create_single_dynamic_idl.sh http://dev.w3.org/html5/workers/ webworkers ../data/idl/generated/w3c_webworkers.idl
./create_single_dynamic_idl.sh http://dev.w3.org/html5/webstorage/ webstorage ../data/idl/generated/w3c_webstorage.idl
./create_single_dynamic_idl.sh http://dev.w3.org/html5/websockets/ websockets ../data/idl/generated/w3c_websockets.idl
./create_single_dynamic_idl.sh http://dev.w3.org/html5/eventsource/ serversent  ../data/idl/generated/w3c_serversent.idl

./create_single_dynamic_idl.sh http://dev.w3.org/geo/api/spec-source.html geo ../data/idl/generated/geolocation.idl

./create_single_dynamic_idl.sh http://www.whatwg.org/specs/web-apps/current-work/complete.html webapps ../data/idl/generated/whatwg_webapps.idl
./create_single_dynamic_idl.sh http://www.whatwg.org/specs/web-apps/current-work/ html ../data/idl/generated/whatwg_html.idl
./create_single_dynamic_idl.sh http://dev.w3.org/html5/spec/Overview.html html5 ../data/idl/generated/w3c_html5.idl
./create_single_dynamic_idl.sh http://dev.w3.org/geo/api/spec-source.html geo ../data/idl/generated/geolocation.idl

./create_single_dynamic_idl.sh http://dvcs.w3.org/hg/domcore/raw-file/tip/Overview.html domcore ../data/idl/generated/domcore.idl

./create_single_dynamic_idl.sh http://dev.w3.org/csswg/cssom/ cssom ../data/idl/generated/cssom.idl
./create_single_dynamic_idl.sh http://dev.w3.org/csswg/cssom-view/ cssom_view ../data/idl/generated/cssom-view-module.idl

./create_single_dynamic_idl.sh http://dev.w3.org/2006/webapi/FileAPI/ fileapi ../data/idl/generated/fileapi.idl

./create_single_dynamic_idl.sh http://dvcs.w3.org/hg/webperf/raw-file/tip/specs/NavigationTiming/Overview.html navigationtiming ../data/idl/generated/navigationtiming.idl

./create_single_dynamic_idl.sh http://dvcs.w3.org/hg/editing/raw-file/tip/editing.html html_editing ../data/idl/generated/html_editing.idl

wget http://www.w3.org/TR/xproc/ -O temp.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.HTML2XML temp.html temp.xml
java -cp out XsltTransform temp.xml create_xproc_grammar.xslt ../data/web_grammar/generated/xproc.txt




rm temp.html
rm temp.xml
