set OLDPATH=%PATH%
set PATH="C:\icu\bin";%PATH%

copy ..\testfiles\json\jsonlint_a_handy_json_validator_and_reformatter.json ..\testfiles\json\jsonlint_utf8.json
uconv.exe -f UTF-8 -t UTF-16BE -o ../testfiles/json/jsonlint_utf16be.json ../testfiles/json/jsonlint_utf8.json
uconv.exe -f UTF-8 -t UTF-16LE -o ../testfiles/json/jsonlint_utf16le.json ../testfiles/json/jsonlint_utf8.json
uconv.exe -f UTF-8 -t UTF-32BE -o ../testfiles/json/jsonlint_utf32be.json ../testfiles/json/jsonlint_utf8.json
uconv.exe -f UTF-8 -t UTF-32LE -o ../testfiles/json/jsonlint_utf32le.json ../testfiles/json/jsonlint_utf8.json

copy ..\testfiles\json\sample.json ..\testfiles\json\sample_utf8.json
uconv.exe -f UTF-8 -t UTF-16BE -o ../testfiles/json/sample_utf16be.json ../testfiles/json/sample_utf8.json
uconv.exe -f UTF-8 -t UTF-16LE -o ../testfiles/json/sample_utf16le.json ../testfiles/json/sample_utf8.json
uconv.exe -f UTF-8 -t UTF-32BE -o ../testfiles/json/sample_utf32be.json ../testfiles/json/sample_utf8.json
uconv.exe -f UTF-8 -t UTF-32LE -o ../testfiles/json/sample_utf32le.json ../testfiles/json/sample_utf8.json

set PATH=%OLDPATH%
