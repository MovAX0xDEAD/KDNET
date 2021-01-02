Kernel Debugging over LAN cable for Windows XP/2003 x32
=======================================================

Hardware requirements:

KDNET patcher support "Debug LAN drivers" from Windows 8.1 or Windows10
1) Option A (Windows 8.1) Supported Netcards: <http://go.microsoft.com/fwlink/p/?linkid=230754>
2) Option B (Windows 10 ) Supported Netcards: <http://go.microsoft.com/fwlink/p/?linkid=>

Software requirements:

1) `Microsoft Debuggers And Tools x86` for Windows 8.1 (WinDBG v6.3.9600.xxxxx), can be taken from Windows 8.1 WDK or downloaded directly

2) Option A:
   Original files from Windows 8.1 (boot.wim/install.wim or KB3000850, only kdnet.dll version `6.3.9600.17276 ENGLISH` supported )

   These files copyrighted/owned by Microsoft and before using it you must agree with Microsoft EULAs/Rules/Licenses/etc

   One of files need to modify for become compatible with Windows XP/2003 and you take all RISKs & Liabilities to do this

        kdnet.dll       - (v6.3.9600.17276 sha256=21a2fede12e1fa48c86e9de057e83956469c6981e9ff9889203c6a3bc902653b)`
        kd_02_14e4.dll` - Broadcom         (v6.3.xxxx)
        kd_02_10df.dll` - Emulex           (v6.3.xxxx)
        kd_02_19a2.dll` - Emulex           (v6.3.xxxx)
        kd_02_8086.dll` - Intel            (v6.3.xxxx)
        kd_02_1969.dll` - Qualcomm Atheros (v6.3.xxxx)
        kd_02_10ec.dll` - Realtek          (v6.3.xxxx)


   Option B:
   Original files from Windows 8.1 (kdnet.dll from boot.wim/install.wim or KB3000850, only version `6.3.9600.17276 ENGLISH` supported)
   Original files from Windows 10  (kd_02_*.dll)

   These files copyrighted/owned by Microsoft and before using it you must agree with Microsoft EULAs/Rules/Licenses/etc

   One of files need to modify for become compatible with Windows XP/2003 and you take all RISKs & Liabilities to do this

        kdnet.dll       - (v6.3.9600.17276 sha256=21a2fede12e1fa48c86e9de057e83956469c6981e9ff9889203c6a3bc902653b)`
        kd_02_14e4.dll` - Broadcom         (v10.0.xxxx) 
        kd_02_10df.dll` - Emulex           (v10.0.xxxx)
        kd_02_19a2.dll` - Emulex           (v10.0.xxxx)
        kd_02_8086.dll` - Intel            (v10.0.xxxx)
        kd_02_1969.dll` - Qualcomm Atheros (v10.0.xxxx)
        kd_02_10ec.dll` - Realtek          (v10.0.xxxx)


3) Download from project:
   Option A:
        xdelta3.exe
        kdnet_delta.bin
        debug_card.inf

   Option B:
        xdelta3.exe
        kdnet_delta.bin
        kdstub.dll
        debug_card.inf

4) Make kdnet.dll:
       rename original kdnet.dll to kdnet_orig.dll

       xdelta3.exe -v -f -d -s kdnet_orig.dll kdnet_delta.bin kdnet.dll

5) Place `kdnet.dll` to folder `Windows\system32\` of your installed Windows XP/2003

6) Option A:
   Select one of `kd_02_XXXX.dll` and rename to `kdstub.dll`, place `kdstub.dll` to folder `Windows\system32\`

   Option B:
   Select one of `kd_02_XXXX.dll` and rename to `kdnet10.dll`, place `kdstub.dll` and `kdnet10.dll` to folder `Windows\system32\`

7) Add new lines to boot.ini to section [operating systems] on Target PC, use this example as template, change parameters to yours:
  (**Host** -  PC where WinDBG/KD started, **Target** - debuggee WinXP/2003 PC)

       C:\WINDOWS="KDNET DHCP"        /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=NET /HOST_IP=192.168.5.1 /HOST_PORT=50000 /ENCRYPTION_KEY=make.winxp.great.again /CPUFREQ=3000
       C:\WINDOWS="KDNET Instant Brk" /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=NET /HOST_IP=192.168.5.1 /HOST_PORT=50000 /ENCRYPTION_KEY=make.winxp.great.again /CPUFREQ=3000 /BREAK
       C:\WINDOWS="KDNET Force IP"    /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=NET /HOST_IP=192.168.5.1 /HOST_PORT=50000 /ENCRYPTION_KEY=make.winxp.great.again /CPUFREQ=3000 /TARGET_IP=192.168.5.2
       C:\WINDOWS="KDNET 10Gb"        /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=NET /HOST_IP=192.168.5.1 /HOST_PORT=50000 /ENCRYPTION_KEY=make.winxp.great.again /CPUFREQ=3000 /PCI_ID=10EC8136


* **C:\WINDOWS or multi(x)disk(x)rdisk(x)partition(x)** - copy this setting from existing boot.ini
* **/noexecute=optin** (optout, disabled) - copy this setting from existing boot.ini

* **/HOST_IP** - IP of Host

* **/HOST_PORT** - UDP Port of WinDBG/KD on Host, must be higher 49152

* **/ENCRYPTION_KEY** - text password, any four words separated by dots

* **/CPUFREQ** - declare maximal frequency of Target PC's CPU in Mhz (include turbomode and other speedups)

* **/PCI_ID** - option required for Intel/Broadcom 10Gb/40Gb LAN Controllers (without this option, driver set too small size of memory buffer for 10Gb+ cards)

Optional:

* **/BREAK** - instant stop at INT3 opcode, if LAN card was not intialized properly by kdnet, Target PC will be rebooted

* **/TARGET_IP** - by default Target get own IP from DHCP on your network, this option disable DHCP and force to use this IP on debuggee PC

* **/W2003** - set for Windows 2003 or mixed OS like "Windows XP + Windows 2003's hal.dll"

* **/PCI_ID** - search LAN Card by VEN_ID & DEV_ID, for example Realtek PCI-e LAN card in Device Manager detected as PCI\VEN_10EC&DEV_8136, to skip autodetect and use this card set /PCI_ID=10EC8136 (must be 8 chars !)

* **/BUSPARAMS=4.25.8** - obsolete option (When you have more than 1 netcard, look in Device Manager and search there for your used lan card, here for example PCI-Bus 4, Device 25, Function 8. This option force to use only this netcard)

8) Disable firewall at Host on choosen UDP Port

9) Connect both PC to you network (hub/switch) or by cable directly
    (Microsoft recomendations: 1) don't use crossover cable 2) don't use crossover port in hub/switch)

10) Run WinDBG/KD on Host, select Kernel Debug, enter UDP port number and password (ENCRYPTION_KEY)

11) Run Target PC, select one of debug boot options

12) In Device Manager on Target, original LAN card will be transformed to "Unknow Controller", use debug_card.inf to install dummy driver for it
      

Tips & Tricks
-------------
- To run WinDBG v6.3.9600.xxxxx on Windows XP/2003, you need newer msvcrt.dll, one of these project: <http://blog.livedoor.jp/blackwingcat/archives/1706829.html>

- To fix empty Register window of WinDBG, patch dbgeng.dll 6.3.9600.17200 (size= 2865840):

  `000ECEA2:       0B 00`

- Connect string for Interactive Disassembler (IDA) when using windbg remote debugging:

  `net:port=50000,key=make.winxp.great.again`



Kernel Debugging over USB3.0 Debugging cable for Windows XP/2003 x32
=======================================================

Hardware requirements:

1) Special **USB3.0 Debugging Cable**, it is cross-cable with disconnected usb2.0 signals/+5V

**Do not use cables from external HDD or pci-express raisers, they are not debugging cables and probably will burn you devices !**

Pinout Reference:

     PINOUT   |    SIGNAL NAME   |   DESC                       |   WIRE COLOR
     ---------|------------------|------------------------------|-------------
     1     1  |    1 +5V         |   NO CONNECTION !            |
     2     2  |    2 USB2 -      |   NO CONNECTION !            |
     3     3  |    3 USB2 +      |   NO CONNECTION !            |
     4-----4  |    4 GND GROUND  |   FOR POWER RETURN           |   BLACK
     5-----8  |    5 StdA_SSRX-  |   SUPERSPEED REC DIFF PAIR   |   BLUE
     6-----9  |    6 StdA_SSRX+  |   SUPERSPEED REC DIFF PAIR   |   YELLOW
     7-----7  |    7 GND_DRAIN   |   GROUND FOR SIGNAL RETURN   |
     8-----5  |    8 StdA_SSTX-  |   SUPERSPEED TRANS DIFF PAIR |   PURPLE
     9-----6  |    9 StdA_SSTX+  |   SUPERSPEED TRANS DIFF PAIR |   ORANGE
     Sh---Sh  |      SHIELD      |   CONNECTOR METAL SHELL      |

2) Only USB3.0 onboard ports/controller supported, external cards may not work properly even with original windows 8 debugging

3) USB3.0 Controller and selected Port must support DEBUG specification, check with USBVIEW.exe from "Debuggers And Tools" install folder.
   Port must have *Port Debug Capable=Yes*, if NO, check other available ports. USBVIEW.exe requires Windows 8/10 (work with Windows 8/10 PE ISOs too)


Software requirements:

1) PC with Windows 8/10 and Microsoft drivers for USB3.0 controller (Vendor's usb3.0 drivers not supported by WinDBG/KD)

2) `Microsoft Debuggers And Tools x86` for Windows 8.1 (WinDBG v6.3.9600.xxxxx), can be taken from Windows 8.1 WDK or downloaded directly

3) Original files from Windows 8.1 (boot.wim/install.wim archives or KB3000850, only kdusb.dll version `6.3.9600.17399 ENGLISH` supported)

   These files copyrighted/owned by Microsoft and before using it you must agree with Microsoft EULAs/Rules/Licenses/etc

   One of files need to modify for become compatible with Windows XP/2003 and you take all RISKs & Liabilities to do this

        kdusb.dll       - (v6.3.9600.17399 sha256=4bffc3728004225a6207b112cb9fded488067481c9cd6ce745185111bdd6adc7)`

4) Download from project:

        xdelta3.exe
        kdusb_delta.bin

5) Make kdusb.dll:

       rename original kdusb.dll to kdusb_orig.dll
       xdelta3.exe -v -f -d -s kdusb_orig.dll kdusb_delta.bin kdusb.dll

6) Place `kdusb.dll` to folder `Windows\system32\` of your installed Windows XP/2003


7) Add new lines to boot.ini to section [operating systems], use this example as template, change parameters to yours:
  (**Host** -  PC where WinDBG/KD started, **Target** - debuggee PC)

       C:\WINDOWS="KDUSB"             /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=USB /CPUFREQ=3000 /TARGETNAME=Donald
       C:\WINDOWS="KDUSB Instant Brk" /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=USB /CPUFREQ=3000 /TARGETNAME=Donald /BREAK

* **/noexecute=optin** (optout, disabled) - copy this setting from existing boot.ini

* **/TARGETNAME** - any USB name

* **/CPUFREQ** - declare maximal frequency of Target PC's CPU in Mhz (include turbomode and other speedups)

Optional:

* **/BREAK** - instant stop at INT3 opcode, if USB3 Controller was not intialized properly by kdstub, Target PC will be rebooted

* **/W2003** - set for Windows 2003 or mixed OS like "Windows XP + Windows 2003's hal.dll"

* **/PCI_ID** - search USB3 Controller by VEN_ID & DEV_ID, for example Intel USB3 controller in Device Manager detected as PCI\VEN_8086&DEV_A12F, to skip autodetect and use this controller set /PCI_ID=8086A12F (must be 8 chars !)

* **/BUSPARAMS=4.25.8** - obsolete option (When you have more than 1 usb3 controller, look in Device Manager and search there for your used usb3 controller, here for example PCI-Bus 4, Device 25, Function 8. This option force to use only this controller


8) Connect both PC by USB3.0 debugging cable

9) Run WinDBG/KD on Host, select Kernel Debug, enter USB name

10) Run Target PC, select one of debug boot options

11) On Host with USBView.exe you can check proper initialization of Target. It will show connected Target PC as "USB 2.0 Debug Connection Device"

      

Tips & Tricks
-------------

- Connect string for Interactive Disassembler (IDA) when using windbg remote debugging:

  `usb2:targetname=Donald`
