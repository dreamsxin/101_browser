java -cp htmlparser-1.2.1.jar nu.validator.htmlparser.tools.XSLT4HTML5 --template=create_idl.xslt --input-html=html5.html --output-xml=../data/idl/html5_.idl
sed -e '1d' -e "s/&lt;/</" -e "s/&gt;/>/" ../data/idl/html5_.idl > ../data/idl/html5.idl
rm ../data/idl/html5_.idl
