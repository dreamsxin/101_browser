set OLDPATH=%PATH%
set PATH="C:\icu\bin";%PATH%

uconv.exe -f UTF-8 -t UTF-16BE -o ../testfiles/json/jsonlint_utf16be.json ../testfiles/json/jsonlint_utf8.json
uconv.exe -f UTF-8 -t UTF-16LE -o ../testfiles/json/jsonlint_utf16le.json ../testfiles/json/jsonlint_utf8.json
uconv.exe -f UTF-8 -t UTF-32BE -o ../testfiles/json/jsonlint_utf32be.json ../testfiles/json/jsonlint_utf8.json
uconv.exe -f UTF-8 -t UTF-32LE -o ../testfiles/json/jsonlint_utf32le.json ../testfiles/json/jsonlint_utf8.json

set PATH=%OLDPATH%
