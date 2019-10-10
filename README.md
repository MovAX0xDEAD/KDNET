Kernel Debugging over LAN cable for Windows XP/2003 x32
=======================================================

Hardware requirements:

1) Supported Netcards: <http://go.microsoft.com/fwlink/p/?linkid=230754>

Software requirements:

1) `Microsoft Debuggers And Tools x86` for Windows 8.1 (WinDBG v6.3.9600.xxxxx), can be taken from Windows 8.1 WDK or downloaded directly

2) Original files from Window 8.1 ISO (boot.wim/install.wim archives), only version `6.3.9600.17276 ENGLISH` supported

   These files copyrighted/owned by Microsoft and before using it you must agree with Microsoft EULAs/Rules/Licenses/etc

   One of files need to modify for become compatible with Windows XP/2003 and you take all RISKs & Liabilities to do this

        kdnet.dll       - (v6.3.9600.17276 sha1=54bef4f1c508eeec84b20334eea5b3f9154e2edd)`
        kd_02_14e4.dll` - Broadcom
        kd_02_10df.dll` - Emulex
        kd_02_19a2.dll` - Emulex
        kd_02_8086.dll` - Intel
        kd_02_1969.dll` - Qualcomm Atheros
        kd_02_10ec.dll` - Realtek

3) Download from project:

        xdelta3.exe
        kdnet_delta.bin
        debug_card.inf

4) Make kdnet.dll:

       rename original kdnet.dll to kdnet_orig.dll
       xdelta3.exe -v -f -d -s kdnet_orig.dll kdnet_delta.bin kdnet.dll

5) Place `kdnet.dll` to folder `Windows\system32\` of your installed Windows XP/2003

6) Select one of `kd_02_XXXX.dll` and rename to `kdstub.dll`, place `kdstub.dll` to folder `Windows\system32\`

7) Add new lines to boot.ini to section [operating systems]:
  (**Host** -  machine where WinDBG/KD started. **Target** - debuggee machine)

       C:\WINDOWS="KDNET"     /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=NET /HOST_IP=192.168.5.1 /HOST_PORT=50000 /ENCRYPTION_KEY=make.winxp.great.again /CPUFREQ=3000 /TARGET_IP=192.168.5.2
       C:\WINDOWS="KDNET_Brk" /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=NET /HOST_IP=192.168.5.1 /HOST_PORT=50000 /ENCRYPTION_KEY=make.winxp.great.again /CPUFREQ=3000 /TARGET_IP=192.168.5.2 /BREAK

* **/noexecute=optin** (optout, disabled) - copy this setting from existing boot.ini

* **/HOST_IP** - IP of Host

* **/HOST_PORT** - UDP Port of WinDBG/KD on Host, must be higher 49152

* **/ENCRYPTION_KEY** - text password, any four words separated by dots

* **/CPUFREQ** - declare nominal frequency of your CPU in Mhz (without turbomode and other speedups)

Optional:

* **/BREAK** - instant stop at INT3 opcode, if netcard was not intialized properly by kdnet, Target will be rebooted

* **/TARGET_IP** - By default Target get own IP from DHCP on your network, this option disable DHCP and force to use this IP

* **/W2003** - set for Windows 2003 or mixed OS like "Windows XP + Windows 2003's hal.dll"

* **/PAE /NOPAE** - select kernel with/out PAE

* **/BUSPARAMS=4.25.8** - When you have more than 1 netcard, look in Device Manager and search there for your used lan card, here for example PCI-Bus 4, Device 25, Function 8. This option force to use only this netcard

8) Disable firewall at Host on choosen UDP Port

9) Connect both PC to you network (hub/switch) or by cable directly
    Don't use crossover cable, and don't use crossover port in hub/switch (Microsoft recomendation)

10) Run WinDBG/KD on Host, select Kernel Debug, enter UDP port number and password (ENCRYPTION_KEY)

11) Run Target PC, select debug boot option

12) In Windows on Target, original netcard will be transformed to System Device, use debug_card.inf to install dummy driver for it
      

Tips & Tricks
-------------
- To run WinDBG v6.3.9600.xxxxx on Windows XP/2003, you need newer msvcrt.dll, one of these project: <http://blog.livedoor.jp/blackwingcat/archives/1706829.html>

- To fix empty Register window of WinDBG, patch dbgeng.dll 6.3.9600.17200 (size= 2865840):

  `000ECEA2:       0B 00`

- Connect string for Interactive Disassembler (IDA):

  `net:port=50000,key=make.winxp.great.again`


Kernel Debugging over USB3.0 Debugging cable for Windows XP/2003 x32
=======================================================

Hardware requirements:

1) Special **USB3.0 Debugging Cable**, it is cross-cable with disconnected usb2.0 signal and +5V

**Do not use cables from external HDD or pci-express raisers, they are not debugging cables and probably will burn you devices !**

Pinout Reference:

     PINOUT   |    SIGNAL NAME   |   DESC                       |   WIRE COLOR
     ---------|------------------|------------------------------|-------------
     1     1  |    1 +5V         |   NO CONNECTION !            |
     2     2  |    2 USB2 +      |   NO CONNECTION !            |
     3     3  |    3 USB2 -      |   NO CONNECTION !            |
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

1) On Target PC need Windows 8/10 to be installed with Microsoft drivers for USB3.0 controllers (Vendor's usb3.0 drivers not supported by WinDBG/KD)

2) `Microsoft Debuggers And Tools x86` for Windows 8.1 (WinDBG v6.3.9600.xxxxx), can be taken from Windows 8.1 WDK or downloaded directly

3) Original files from Window 8.1 ISO (boot.wim/install.wim archives), only version `6.3.9600.17399 ENGLISH` supported

   These files copyrighted/owned by Microsoft and before using it you must agree with Microsoft EULAs/Rules/Licenses/etc

   One of files need to modify for become compatible with Windows XP/2003 and you take all RISKs & Liabilities to do this

        kdusb.dll       - (v6.3.9600.17399 sha1=54bef4f1c508eeec84b20334eea5b3f9154e2edd)`

4) Download from project:

        xdelta3.exe
        kdusb_delta.bin

5) Make kdusb.dll:

       rename original kdusb.dll to kdusb_orig.dll
       xdelta3.exe -v -f -d -s kdusb_orig.dll kdusb_delta.bin kdusb.dll

6) Place `kdusb.dll` to folder `Windows\system32\` of your installed Windows XP/2003


7) Add new lines to boot.ini to section [operating systems]:
  (**Host** -  machine where WinDBG/KD started. **Target** - debuggee machine)

       C:\WINDOWS="KDUSB"     /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=USB /CPUFREQ=3000 /TARGETNAME=Donald
       C:\WINDOWS="KDUSB_Brk" /noexecute=optin /fastdetect /DEBUG /DEBUGPORT=USB /CPUFREQ=3000 /TARGETNAME=Donald /BREAK

* **/noexecute=optin** (optout, disabled) - copy this setting from existing boot.ini

* **/TARGETNAME** - any USB name

* **/CPUFREQ** - declare nominal frequency of your CPU in Mhz (without turbomode and other speedups)

Optional:

* **/BREAK** - instant stop at INT3 opcode, if usb3.0 was not intialized properly by kdstub, Target will be rebooted

* **/W2003** - set for Windows 2003 or mixed OS like "Windows XP + Windows 2003's hal.dll"

* **/PAE /NOPAE** - select kernel with/out PAE

* **/BUSPARAMS=4.25.8** - When you have more than 1 usb3 controller, look in Device Manager and search there for your used usb3 controller, here for example PCI-Bus 4, Device 25, Function 8. This option force to use only this controller


8) Connect both PC by USB3.0 debugging cable

9) Run WinDBG/KD on Host, select Kernel Debug, enter USB name

10) Run Target PC, select debug boot option

11) On Host with USBView.exe you can check proper initialization of Target. It will show connected Target PC as "USB 2.0 Debug Connection Device"

      

Tips & Tricks
-------------

- Connect string for Interactive Disassembler (IDA):

  `usb2:targetname=Donald`
