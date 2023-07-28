Renesas RZ/G3S
==============

The "RZ/G3S" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 and in addation duo CR8 and a M33 core, engineers can
easily implement real-time control and
industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/G3S reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board             |      Details                                                                                                                     |
+==============+===============+=======================================================================================================================+
| smarc             | Equipped with Renesas RZ/G3S SoC                                                                                                 |
| (g3s_smarc)       | +--------------------------------------------------------------------------------------------------------------------------------+
|                   | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>                                            |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__<TBD>

The current TF-A port has been tested on the SMARC Evaluation board for RZ/G3S
SoC_id  rXXXXX revision ESx.y.


::

    ARM CA55 r2p0 (ARMv8-A) 1.2 GHz Single Core, with NEON/VFP, L1$ I/D 32K, L2$ 0MB, L3$ 256K
    ARM M33 240 MHz Dual core
    Memory controller for LPDDR4-3200 4GB in 2 channels(32-bit bus mode)
    SD card host interface,
    USB2.0 interfaces,
    CAN interfaces,
    Ethernet AVB,
    Memories
        INTERNAL 3 x 256KB SYSTEM RAM
        DDR 4 GB LPDDR4
        QSPI FLASH 256MB  <TBC>
        EMMC 32 GB EMMC (HS400 240 MBYTES/S) <TBC>
        MICROSD-CARD SLOT (SDR104 100 MBYTES/S) <TBC>

Overview
--------
On RZ/G3S SoCs the BOOTROM starts the cpu at EL3; for this port BL2
will therefore be entered at this exception level (the Renesas' ATF
reference tree [1] resets into EL1 before entering BL2 - see its
bl2.ld.S)

BL2 initializes DDR before determining the boot reason (cold or warm).

Once BL2 boots, it determines the boot reason, writes it to shared
memory (BOOT_KIND_BASE) together with the BL31 parameters
(PARAMS_BASE) and jumps to BL31.

To all effects, BL31 is as if it is being entered in reset mode since
it still needs to initialize the rest of the cores; this is the reason
behind using direct shared memory access to  BOOT_KIND_BASE _and_
PARAMS_BASE instead of using registers to get to those locations (see
el3_common_macros.S and bl31_entrypoint.S for the RESET_TO_BL31 use
case).

[1] https://github.com/renesas-rz/meta-rzg3l/tree/BSP-1.0.5/recipes-bsp/arm-trusted-firmware/files<TBD>


How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the SMARC Evaluation RZ/G3S development kit used in this port.

Build Tested:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=g3s all BOARD=smarc ENABLE_STACK_PROTECTOR=default DEBUG=1 LOG_LEVEL=40
       

System Tested:
~~~~~~~~~~~~~~
* u-boot:
  The port has beent tested using mainline uboot with <TBD>

TF-A Build Procedure for VBAT mode
~~~~~~~~~~~~~~~~~~~~
.. code:: bash
      tfa_project_path="$1"
      bl33_file_path="$2"
      workspace_path="$3"
      
      bp_tool_path="$tfa_project_path/tools/renesas/rz_boot_param"
      fip_tool_path="$tfa_project_path/tools/fiptool"
      bl2_file_path="$tfa_project_path/build/g3s/debug/bl2.bin"
      bl31_file_path="$tfa_project_path/build/g3s/debug/bl31.bin"
      
      cd "$tfa_project_path"
      make PLAT=g3s BOARD=smarc PLAT_SYSTEM_SUSPEND=vbat all LOG_LEVEL=40 DEBUG=1 PLAT_M33_BOOT_SUPPORT=1
        
      cd "$fip_tool_path"
      make fiptool
      ./fiptool create --align 16 --soc-fw "$bl31_file_path" --nt-fw "$bl33_file_path" "$workspace_path"/fip.bin
      ${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 "${workspace_path}"/fip.bin "${workspace_path}"/fip_vbat_cm33_rzg3s_smarc.srec
        
      cd "$bp_tool_path"
      make PLAT=g3s bptool BOARD=smarc
      
      echo "bl2 file path $bl2_file_path"
      ${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_mmc.bin" 0xA3000 mmc
      cat "$workspace_path"/bp_mmc.bin "$bl2_file_path" > "$workspace_path"/bl2_bp_mmc.bin
      ${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path"/bl2_bp_mmc.bin "$workspace_path"/bl2_bp_mmc_vbat_cm33_rzg3s_smarc.srec
        
      ${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_spi.bin" 0xA3000 spi
      cat "$workspace_path"/bp_spi.bin "$bl2_file_path" > "$workspace_path"/bl2_bp_spi.bin

tfa_project_path= This is where the tfa project is located.
bl33_file_path=This is where the u-boot binary used is located
workspace_path=This is the output folder of the script.


TF-A Build Procedure for AWO mode
~~~~~~~~~~~~~~~~~~~~
.. code:: bash
      tfa_project_path="$1"
      bl33_file_path="$2"
      workspace_path="$3"
      
      bp_tool_path="$tfa_project_path/tools/renesas/rz_boot_param"
      fip_tool_path="$tfa_project_path/tools/fiptool"
      bl2_file_path="$tfa_project_path/build/g3s/debug/bl2.bin"
      bl31_file_path="$tfa_project_path/build/g3s/debug/bl31.bin"
      
      cd "$tfa_project_path"
      make PLAT=g3s BOARD=smarc PLAT_SYSTEM_SUSPEND=vbat all LOG_LEVEL=40 DEBUG=1 PLAT_M33_BOOT_SUPPORT=1
        
      cd "$fip_tool_path"
      make fiptool
      ./fiptool create --align 16 --soc-fw "$bl31_file_path" --nt-fw "$bl33_file_path" "$workspace_path"/fip.bin
      ${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 "${workspace_path}"/fip.bin "${workspace_path}"/fip_vbat_cm33_rzg3s_smarc.srec
        
      cd "$bp_tool_path"
      make PLAT=g3s bptool BOARD=smarc
      
      echo "bl2 file path $bl2_file_path"
      ${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_mmc.bin" 0xA3000 mmc
      cat "$workspace_path"/bp_mmc.bin "$bl2_file_path" > "$workspace_path"/bl2_bp_mmc.bin
      ${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path"/bl2_bp_mmc.bin "$workspace_path"/bl2_bp_mmc_vbat_cm33_rzg3s_smarc.srec
        
      ${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_spi.bin" 0xA3000 spi
      cat "$workspace_path"/bp_spi.bin "$bl2_file_path" > "$workspace_path"/bl2_bp_spi.bin
      ${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path"/bl2_bp_spi.bin "$workspace_path"/bl2_bp_spi_vbat_cm33_rzg3s_smarc.srec

tfa_project_path= This is where the tfa project is located.
bl33_file_path=This is where the u-boot binary used is located
workspace_path=This is the output folder of the script.

Loading the flash writer
~~~~~~~~~~~~~~~~~
Set the device in SCIF mode,
Connect to the COM port provided by the device via some terminal software.
Hit reset and the device will print a message.
Then it is time to send the device the FlashWriter (Flash_Writer_SCIF_RZG3S_DEV_INTERNAL_MEMORY_0127.mot).

Install Procedure for EMMC
~~~~~~~~~~~~~~~~~

1.	Use the ‘Load the flash writer procedure’.
2.	Modify the EXT_CSD registers
  a.	Change the First Register
    i.	Use the Flash Writer's command EM_SECSD.
    ii.	Enter the EXT_CSD index: B1
    iii.	Enter the Input Value: 2
  b.	Change the Second Register
    i.	Use the Flash Writer's command EM_SECSD.
    ii.	Enter the EXT_CSD index: B3
    iii.	Enter the Input Value: 8
  c.	The output should show as follows:
3.	Write the bl2 srecord to the device
  a.	Use the Flash Writer's command EM_W.
  b.	Partition Select: 1
  c.	Input Start Address in sector: 1
  d.	Input Program Start Address: a1e00
  e.	Send the bl2 srecord.
4.	Write the fip srecord to the device
  a.	Use the Flash Writer's command EM_W.
  b.	Partition Select: 1
  c.	Input Start Address in sector: 320
  d.	Input Program Start Address: 00000
  e.	Send the fip srecord file
5.	Write the CM33 srecord file to device eMMC
  a.	Use the Flash Writer's command EM_W.
  b.	Partition Select: 1
  c.	Input Start Address in sector: 1000
  d.	Input Program Start Address: 23000
  e.	Send the CM33 srecord file



Install Procedure for xSPI
~~~~~~~~~~~~~~~~~
1.	Use the ‘Load the flash writer procedure’.
2.	Write the BL2 srecord to the device SPI flash
  a.	Enter: XLS2
  b.	Program Top Address: 0xA1E00
  c.	QSPI Save Address: 0x00000
  d.	Send the BL2 srecord
  e.	The output should show as follows:
3.	Write the BL31 srecord to the device SPI flash
  a.	Enter: XLS2
  b.	Program Top Address: 0x00000
  c.	QSPI Save Address: 0x64000
  d.	Send the BL2 srecord



[2] https://github.com/renesas-rz/rzg3s_flash_writer/tree/rz_g3s


Boot trace
----------
::
NOTICE:  BL2: v2.7(debug):V2.7/RZG3S-1.00-BETA-1227-ge48d3c423-dirty
NOTICE:  BL2: Built : 18:38:22, Jul 25 2023
INFO:    BL2: Doing platform setup
INFO:    Configuring TrustZone Controller
INFO:    Total 1 regions set.
INFO:    Configuring TrustZone Controller
INFO:    Total 1 regions set.
INFO:    Configuring TrustZone Controller
INFO:    Total 4 regions set.
INFO:    Configuring TrustZone Controller
INFO:    Total 1 regions set.
INFO:    DDR: Setup (Rev. 5.21)
INFO:    eMMC boot from partition 1
INFO:    Saving DDR retention info.
INFO:    emmcdrv_block_len: len: 0x00001000
INFO:    DDR Retention Info saved.
INFO:    BL2: Loading image id 3
INFO:    eMMC boot from partition 1
INFO:    Load dst=0xc3b20 src=(p:1)0x64000(800) len=0x10(1)
INFO:    eMMC boot from partition 1
INFO:    Load dst=0xc3e50 src=(p:1)0x64010(800) len=0x28(1)
INFO:    Loading image id=3 at address 0x44000000
INFO:    eMMC boot from partition 1
INFO:    Load dst=0x44000000 src=(p:1)0x64090(800) len=0x3a000(465)
INFO:    Image id=3 loaded: 0x44000000 - 0x4403a000
INFO:    BL2: Loading image id 5
INFO:    eMMC boot from partition 1
INFO:    Load dst=0xc3b20 src=(p:1)0x64000(800) len=0x10(1)
INFO:    eMMC boot from partition 1
INFO:    Load dst=0xc3e50 src=(p:1)0x64010(800) len=0x28(1)
INFO:    Load dst=0xc3e50 src=(p:1)0x64038(800) len=0x28(1)
INFO:    Loading image id=5 at address 0x50000000
INFO:    eMMC boot from partition 1
INFO:    Load dst=0x50000000 src=(p:1)0x9e090(1264) len=0xa3da8(1312)
INFO:    Image id=5 loaded: 0x50000000 - 0x500a3da8
INFO:    BL2: Loading image id 36
INFO:    eMMC boot from partition 1
INFO:    Loading image id=36 at address 0x23000
INFO:    emmcdrv_block_len: len: 0x00030000
INFO:    Load dst=0x23000 src=(p:1)0x200000(4096) len=0x30000(384)
INFO:    Image id=36 loaded: 0x23000 - 0x53000
NOTICE:  BL2: Booting BL31
INFO:    Entry point address = 0x44000000
INFO:    SPSR = 0x3cd
NOTICE:  BL31: v2.7(debug):V2.7/RZG3S-1.00-BETA-1227-ge48d3c423-dirty
NOTICE:  BL31: Built : 18:38:25, Jul 25 2023
INFO:    GICv3 without legacy support detected.
INFO:    ARM GICv3 driver initialized in EL3
INFO:    Maximum SPI INTID supported: 511
INFO:    BL31: Initializing runtime services
INFO:    BL31: cortex_a55: CPU workaround for 1530923 was applied
INFO:    BL31: Preparing for EL3 exit to normal world
INFO:    Entry point address = 0x50000000
INFO:    SPSR = 0x3c5


U-Boot 2021.10 (Mar 31 2023 - 03:35:07 +0000)

CPU:   Renesas Electronics CPU rev 1.0
Model: smarc-rzg3s
DRAM:  896 MiB
MMC:   sd@11c00000: 0, sd@11c10000: 1, sd@11c20000: 2
Loading Environment from MMC... OK
In:    serial@1004b800
Out:   serial@1004b800
Err:   serial@1004b800
Net:
Error: ethernet@11c30000 address not set.
No ethernet found.

Hit any key to stop autoboot:  0

	
