REM Parameters:
REM %1: URL to download
REM %2: Namespace name
REM %3: Output file

wget %1 -O temp.html
java -cp htmlparser-1.3.1.jar nu.validator.htmlparser.tools.HTML2XML temp.html temp.xml
java -cp out XsltTransform temp.xml create_idl.xslt namespace %2 %3
dos2unix %3
