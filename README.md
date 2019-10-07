# Kernel Debugging over LAN cable for Windows XP/2003 x32

Supported Netcards: <http://go.microsoft.com/fwlink/p/?linkid=230754>

1)  Need Microsoft Debuggers And Tools x86 for Windows 8.1 (WinDBG v6.3.9600.xxxxx), can be taken from Windows 8.1 WDK or downloaded directly

2)  Need original files from Window 8.1 ISO (boot.wim/install.wim archives), only version "6.3.9600.17276 ENGLISH" supported.
    These files copyrighted/owned by Microsoft and before using it you must agree with Microsoft EULAs/Rules/Licenses/etc.
    One of files need to modify for become compatible with Windows XP/2003 and you take all RISKs & Liabilities to do this.

* `kdnet.dll       - (sha1 54bef4f1c508eeec84b20334eea5b3f9154e2edd)`
* `kd_02_14e4.dll` - Broadcom
* `kd_02_10df.dll` - Emulex
* `kd_02_19a2.dll` - Emulex
* `kd_02_8086.dll` - Intel
* `kd_02_1969.dll` - Qualcomm Atheros
* `kd_02_10ec.dll` - Realtek

3)  Download from project:
```
    - xdelta3.exe
    - kdnet_delta.bin
    - debug_card.inf
```
4)  Make kdnet.dll:
```
    - rename original kdnet.dll to kdnet_orig.dll
    - xdelta3.exe -v -f -d -s kdnet_orig.dll kdnet_delta.bin kdnet.dll
```
5)  Place `kdnet.dll` to folder `Windows\system32\` of your installed Windows XP/2003

6)  Select one of `kd_02_XXXX.dll` and rename to `kdstub.dll`, place `kdstub.dll` to folder `Windows\system32\`

7)  Add new lines to boot.ini to section [operating systems]:
```
C:\WINDOWS="KDNET"     /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=NET /HOST_IP=192.168.5.1 /HOST_PORT=50000 /ENCRYPTION_KEY=make.winxp.great.again /CPUFREQ=3000 /TARGET_IP=192.168.5.2
C:\WINDOWS="KDNET_Brk" /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=NET /HOST_IP=192.168.5.1 /HOST_PORT=50000 /ENCRYPTION_KEY=make.winxp.great.again /CPUFREQ=3000 /TARGET_IP=192.168.5.2 /BREAK
```

Host -  machine where WinDBG/KD started
Target - debuggee machine

noexecute=optin (optout, disabled) - copy this setting from existing boot.ini.
HOST_IP - IP of Host.
HOST_PORT - UDP Port of WinDBG/KD on Host, must be higher 49152 .
ENCRYPTION_KEY - text password, any four words separated by dots.
CPUFREQ - declare nominal frequency of your CPU in Mhz (without turbomode and other speedups).

Optional:
BREAK - instant stop at INT3 opcode, if netcard was not intialized properly by kdnet/kdstub, Target will be rebooted.
TARGET_IP - By default Target get own IP from DHCP on your network, this option disable DHCP and force to use this IP.
W2003 - set for Windows 2003 or mixed OS like "Windows XP + Windows 2003's hal.dll".
PAE/NOPAE - select kernel with/out PAE.

8)  Disable firewall at Host on choosen UDP Port

9)  Connect both PC to you network (hub/switch) or by cable directly
    Don't use crossover cable, and don't use crossover port in hub/switch (Microsoft recomendation)

10) Run WinDBG/KD on Host, select Kernel Debug, enter UDP port number and password (ENCRYPTION_KEY)

11) Run Target PC, select debug boot option

12) In Windows on Target, original netcard will be transformed to System Device, use debug_card.inf to install dummy driver for it




# Tips & Tricks:
- To run WinDBG v6.3.9600.xxxxx on Windows XP/2003, you need newer msvcrt.dll, one of these project: <http://blog.livedoor.jp/blackwingcat/archives/1706829.html>

- To fix empty Register window of WinDBG, patch dbgeng.dll 6.3.9600.17200 (size= 2865840):
  `000ECEA2:       0B 00`

- Connect string for Interactive Disassembler (IDA):
  `net:port=50000,key=make.winxp.great.again`