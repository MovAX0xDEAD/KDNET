chsize32.exe kdnet.dll 76432
fasm         KDNET_PATCH.ASM KDNET.BIN
copy /B /Y   kdnet.dll+KDNET.BIN+dummy kdnet.dllnew
del /Q       kdnet.dll_old
ren          kdnet.dll kdnet.dll_old
ren          kdnet.dllnew kdnet.dll
chsize32.exe kdnet.dll 77824
checksum.exe kdnet.dll