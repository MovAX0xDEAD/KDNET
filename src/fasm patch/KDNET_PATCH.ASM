use32

macro LOAD arg0, arg1
{
        lea    arg0, [edi+arg1-BASE]
}

macro LOAD_RVA arg0, arg1
{
        lea    arg0, [edi+arg1-BASE+ORG]
}

macro STORE arg0, arg1, size
{
        mov  size  [edi+arg0-BASE], arg1
}
macro STORE_RVA arg0, arg1, size
{
        mov  size  [edi+arg0-BASE+ORG], arg1
}

ORG                        equ $80037490
BASE                       equ $80010000
StallFactor                equ $80021174
HalpKdReadPCIConfig@20     equ $80021404
HalpKdWritePCIConfig@20    equ $80021408
GetPciDataByOffset         equ $800371B2
SetPciDataByOffset         equ $80037020
CPUFREQ_STR                equ $80037360
W2003_STR                  equ $80037368
DEVID_STR                  equ $8003736E
Win2003                    equ $80021178
Header_Patch               equ $8001016C
PCI_ID                     equ $800211AA
Continue1                  equ $8001B5FA - ($$+ORG)
__strupr                   equ $8001B76C - ($$+ORG)
_strstr                    equ $8001B6E0 - ($$+ORG)
_atol                      equ $8001B7AE - ($$+ORG)

;__imp__KeStallExecutionProcessor@4 equ $80031000
;_InitializeLogTiming@0     equ $8001B44C
;MyStallCPU                 equ $800373DC



Start: jmp short MyPatches

Magicstring:    ; hal
        db      $F7, $C2, $F0, $FF, $FF, $FF, $F7

        align 4
Patch_PCI64:
        mov     eax, edx
        and     eax, $6
        cmp     eax, $4
        jnz     short PCI32

addoffset:
        add     dword [ebp+$20], $4
decoffset:
        dec     dword [ebp-$8]
PCI32:
        test    edx, $FFFFFFF0

jmpnear:
        jmp     near Patch_PCI64


Magicstring2:   ; pci
        db      $B0, $01, $EB, $AE, $CC


        align 4
Patch_Debug_PCI:
        mov     al, 1
        mov     esi, [ebp+8]
        cmp     byte [esi+$43], 2
        jnz     short Skip_This
        mov     dword [esi+$38], $BEEFDEAD
Skip_This:
jmpnear2:
        jmp     near Patch_Debug_PCI


        align 4
MyPatches:
        add     esp, $C
        push    eax
        push    ecx
        push    edx
        push    esi
        push    edi
        push    ebx     ;ebx = loadblock

        call    $+5
base1:
        pop     edi
        sub     edi, base1+ORG-BASE

        LOAD    eax, GetPciDataByOffset
        STORE   (HalpKdReadPCIConfig@20),eax
        LOAD    eax, SetPciDataByOffset
        STORE   (HalpKdWritePCIConfig@20),eax
        STORE   (Header_Patch), 5, dword        ; Header_Reloc_Patch

kdstub_dll_Patch:
        pop     ebx
        push    ebx             ; ebx = loadblock
        mov     ecx, [ebx]
Loop0:
        cmp     ebx, ecx
        jz      Cookie_End0
        mov     eax, [ecx+$30]
        mov     edx, [eax]
        or      edx, $200020
        cmp     edx, $64006B ; kd
        jnz     Nextdll0
        mov     edx, [eax+$04]
        or      edx, $200020
        cmp     edx, $740073 ; st
        jnz     Nextdll0
        mov     edx, [eax+$08]
        or      edx, $200020
        cmp     edx, $620075 ; ub
        jnz     Nextdll0
        mov     edx, [eax+$0C]
        or      edx, $200020
        cmp     edx, $64002E ; .d

        jz      Cookie0
Nextdll0:
        mov     ecx, [ecx]
        jmp     Loop0

Cookie0:
        mov     eax, [ecx+$18]
        mov     ebx, eax
        mov     ecx, [eax+$3C]
        mov     dword [eax+ecx+$74], 5  ; Header_Reloc_Patch
        mov     eax, [ecx+ebx+$0C8]
        test    eax, eax
        jz      Cookie_End0
        lea     eax, [eax+ebx+$3C]
        mov     eax, [eax]
        test    eax, eax
        jz      Cookie_End0
        cmp     dword [eax], $BB40E64E  ; Win8 Security Cookie
        jnz     Cookie_End0
        mov     dword [eax], $EFBEADDE
Cookie_End0:

kdnet10_dll_Patch:
        pop     ebx
        push    ebx             ; ebx = loadblock
        mov     ecx, [ebx]
Loop00:
        cmp     ebx, ecx
        jz      Cookie_End00
        mov     eax, [ecx+$30]
        mov     edx, [eax]
        or      edx, $200020
        cmp     edx, $64006B ; kd
        jnz     Nextdll00
        mov     edx, [eax+$04]
        or      edx, $200020
        cmp     edx, $65006E ; ne
        jnz     Nextdll00
        mov     edx, [eax+$08]
        or      edx, $200020
        cmp     edx, $310074 ; t1
        jnz     Nextdll00
        mov     edx, [eax+$0C]
        or      edx, $200020
        cmp     edx, $2E0030 ; 0.
        jz      Cookie00
Nextdll00:
        mov     ecx, [ecx]
        jmp     Loop00

Cookie00:
        mov     eax, [ecx+$18]
        mov     ebx, eax
        mov     ecx, [eax+$3C]
        mov     dword [eax+ecx+$74], 5  ; Header_Reloc_Patch
        mov     eax, [ecx+ebx+$0C8]
        test    eax, eax
        jz      Cookie_End00
        lea     eax, [eax+ebx+$3C]
        mov     eax, [eax]
        test    eax, eax
        jz      Cookie_End00
        cmp     dword [eax], $BB40E64E  ; Win8 Security Cookie
        jnz     Cookie_End00
        mov     dword [eax], $EFBEADDE
Cookie_End00:

W2003_Search:
        mov     ebx, [ebp+8]
        test    ebx, ebx
        jz      W2003_End
        push    ebx
        call    __strupr
        LOAD     eax, W2003_STR
        push    eax
        push    ebx
        call    _strstr
        pop     ebx
        pop     ebx
        pop     ebx
        test    eax, eax
        jz      W2003_End
        STORE  (Win2003), 1, dword
W2003_End:

DEVID_Search:
        mov     ebx, [ebp+8]
        test    ebx, ebx
        jz      DEVID_End
        LOAD    eax, DEVID_STR
        push    eax
        push    ebx
        call    _strstr
        pop     ebx
        pop     ebx
        test    eax, eax
        jz      DEVID_End
        add     eax, 7
        push    eax
        call    hex2long   ; stdcall
        rol     eax,16     ; swap vendor<>device
        STORE  (PCI_ID), eax, dword
DEVID_End:



HAL_Search:
        pop     ebx
        push    ebx             ; ebx = loadblock
        mov     ecx, [ebx]
Loop1:
        cmp     ebx, ecx
        jz      HAL_End
        mov     eax, [ecx+$30]
        mov     edx, [eax+$00]
        or      edx, $00200020
        cmp     edx, $00610068  ; ha
        jnz     Nextdll1
        mov     edx, [eax+$04]
        or      edx, $00200020
        cmp     edx, $002E006C  ; l.
        jz      HAL_Found
Nextdll1:
        mov     ecx, [ecx]
        jmp     Loop1
HAL_End:



PCI_Search:
        pop     ebx
        push    ebx             ; ebx = loadblock
        mov     ecx, [ebx]
Loop2:
        cmp     ebx, ecx
        jz      PCI_End
        mov     eax, [ecx+$30]
        mov     edx, [eax+$00]
        or      edx, $00200020
        cmp     edx, $00630070  ; pc
        jnz     Nextdll2
        mov     edx, [eax+$04]
        or      edx, $00200020
        cmp     edx, $002E0069  ; i.
        jz      Found_PCI
Nextdll2:
        mov     ecx, [ecx]
        jmp     Loop2
PCI_End:

Return:
        pop     ebx
        pop     edi
        pop     esi
        pop     edx
        pop     ecx
        pop     eax
        jmp     Continue1

HAL_Found:
       ; look for F7 C2 F0 FF FF FF F7
       LOAD_RVA esi, Magicstring
       mov edx, edi
       mov edi, [ecx+$18]    ; base
       mov ecx, [ecx+$18+$8] ; len

firstchar:
       mov al, [esi+0]
       repne scasb
       cmp ecx, 0
       jz SkipHAL
       mov al, [esi+1]
       cmp [edi+0], al
       jnz firstchar
       mov al, [esi+2]
       cmp [edi+1], al
       jnz firstchar
       mov al, [esi+3]
       cmp [edi+2], al
       jnz firstchar
       mov al, [esi+4]
       cmp [edi+3], al
       jnz firstchar
       mov al, [esi+5]
       cmp [edi+4], al
       jnz firstchar
       mov al, [esi+6]
       cmp [edi+5], al
       jnz firstchar


Found0:
       mov ebx, edi
       mov edi, edx
       dec ebx
       ; ebx = RR
       cmp word [ebx+$C], $840F
       jnz SkipHAL
       mov esi, [ebx+$E]
       lea esi, [ebx+esi+$C+$6] ; eax = offset to add/add/dec
       cmp word [esi+$0], $4583
       jnz SkipHAL
       mov al, byte [esi+$2] ; al - first  ebp
       STORE_RVA (addoffset+2), al, byte
       cmp word [esi+$7], $4DFF
       jnz SkipHAL
       mov al, byte [esi+$9] ; al - second ebp
       STORE_RVA (decoffset+2), al, byte

jmppatch:
       LOAD_RVA eax, Patch_PCI64
       sub eax, ebx
       sub eax, 5
       mov byte [ebx], $e9
       mov dword [ebx+1], eax
       mov byte [ebx+5], $90
       LOAD_RVA eax, jmpnear
       neg eax
       add eax, ebx
       add eax, 1                       ; skip nop
       STORE_RVA (jmpnear+1), eax


SkipHAL:
       mov edi, edx
       jmp HAL_End


Found_PCI:
       ; look for F7 C2 F0 FF FF FF F7
       mov     eax, [ecx+$18]
       push ecx
       mov     ecx, [eax+$3C]
       mov     dword [eax+ecx+$74], 5 ; dont reloc
       pop ecx

       LOAD_RVA esi, Magicstring2
       mov      edx, edi
       mov edi, [ecx+$18]    ; base
       mov ecx, [ecx+$18+$8] ; len

firstchar2:
       mov al, [esi+0]
       repne scasb
       cmp ecx, 0
       jz SkipPCI
       mov al, [esi+1]
       cmp [edi+0], al
       jnz firstchar2
       mov al, [esi+2]
       cmp [edi+1], al
       jnz firstchar2
       mov al, [esi+3]
       cmp [edi+2], al
       jnz firstchar2
       mov al, [esi+4]
       cmp [edi+3], al
       jnz firstchar2

Found1:
       mov ebx, edi
       mov edi, edx
       dec ebx
       ; ebx = $B0, $01, $EB, $AE, $CC

       LOAD_RVA eax, Patch_Debug_PCI
       sub eax, ebx
       sub eax, 5
       mov byte [ebx], $e9
       mov dword [ebx+1], eax
       LOAD_RVA eax, jmpnear2
       neg eax
       add eax, ebx
       sub eax, $53
       STORE_RVA (jmpnear2+1), eax

SkipPCI:
       mov edi, edx
       jmp PCI_End



hex2long:
;func(const char* ptr) {

        push    ebx
        push    ebp
        push    esi
        push    edi

; 222  :     int             i;
; 223  :     unsigned char   high, low, byte;
; 224  :     unsigned long   dword = 0;
; 225  : 
; 226  :     for (i=0; i < 8; i=i+2) {

        mov     edi, DWORD [esp+$14]
        xor     ebp, ebp
        xor     esi, esi
LL22@func:

; 227  :         high  = hex2decimal(ptr[i]);

        mov     cl, BYTE [edi+esi]
        mov     al, cl
        sub     al, 48                                  ; 00000030H
        cmp     al, 9
        ja      SHORT LN10@func
        sub     cl, 48                                  ; 00000030H
        mov     dl, cl
        jmp     SHORT LN11@func
LN10@func:
        mov     dl, cl
        sub     dl, 97                                  ; 00000061H
        cmp     dl, 5
        ja      SHORT LN8@func
        sub     cl, 87                                  ; 00000057H
        mov     dl, cl
        jmp     SHORT LN11@func
LN8@func:
        mov     al, cl
        sub     al, 65                                  ; 00000041H
        cmp     al, 5
        ja      SHORT LN6@func
        sub     cl, 55                                  ; 00000037H
        mov     dl, cl
        jmp     SHORT LN11@func
LN6@func:
        xor     dl, dl
LN11@func:

; 228  :         low   = hex2decimal(ptr[i+1]);

        mov     cl, BYTE [edi+esi+1]
        mov     al, cl
        sub     al, 48                                  ; 00000030H
        cmp     al, 9
        ja      SHORT LN17@func
        sub     cl, 48                                  ; 00000030H
        mov     bl, cl
        jmp     SHORT LN18@func
LN17@func:
        mov     al, cl
        sub     al, 97                                  ; 00000061H
        cmp     al, 5
        ja      SHORT LN15@func
        sub     cl, 87                                  ; 00000057H
        mov     bl, cl
        jmp     SHORT LN18@func
LN15@func:
        mov     al, cl
        sub     al, 65                                  ; 00000041H
        cmp     al, 5
        ja      SHORT LN13@func
        sub     cl, 55                                  ; 00000037H
        mov     bl, cl
        jmp     SHORT LN18@func
LN13@func:
        xor     bl, bl
LN18@func:

; 229  :         byte  = (high << 4) | low; // 0-3*8-80, 2-2*8-86, 4-1*8-10, 6-0-0F
; 230  :         dword |= (byte << (3 - i/2)*8);

        mov     eax, esi
        shr     eax, 1
        shl     dl, 4
        mov     ecx, 3
        sub     ecx, eax
        add     ecx, ecx
        movzx   edx, dl
        movzx   eax, bl
        add     ecx, ecx
        add     ecx, ecx
        or      edx, eax
        shl     edx, cl
        add     esi, 2
        or      ebp, edx
        cmp     esi, 8
        jl      LL22@func
        pop     edi
        pop     esi

; 231  :     }
; 232  : 
; 233  :     return dword;

        mov     eax, ebp
        pop     ebp
        pop     ebx

; 234  : }

        ret     4







