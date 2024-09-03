cl /nologo /EHsc /std:c17 /c miniz/miniz.c /Fo:.miniz.obj
cl /nologo /DEBUG /Zi /fsanitize=address /RTC1 /RTCs /RTCu /nologo /EHsc /std:c17 /c 05-compress.c /Fo:.05-compress.obj
link /nologo /DEBUG .05-compress.obj .miniz.obj /out:05-compress.exe