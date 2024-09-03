cl /nologo /EHsc /std:c17 /c miniz.c /Fo:miniz.obj
cl /nologo /DEBUG /Zi /fsanitize=address /RTC1 /RTCs /RTCu /nologo /EHsc /std:c17 /c reproduce1.c /Fo:reproduce1.obj
link /nologo /DEBUG reproduce1.obj miniz.obj /out:reproduce1.exe