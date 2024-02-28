Renesas RZ/G3S
==============

The "RZ/G3S" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a single Arm Cortex®-A55 core with the addition of two M33 cores, engineers can
easily implement various solutions.

Plug-ins are available for multiple open-source software tools.



-----------------------------------
Renesas RZ/G3S reference platforms:
-----------------------------------

+--------------+------------------------------------------------------------------------------------------------------------------------------------------+
| Board             |      Details                                                                                                                        |
+==============+===============+==========================================================================================================================+
| smarc             | Equipped with Renesas RZ/G3S SoC                                                                                                    |
| (g3s_smarc)       | +-----------------------------------------------------------------------------------------------------------------------------------+
|                   | https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rtk9845s33s01000be-rzg3s-smarc-evaluation-board-kit |                                          |
+--------------+------------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__

The current TF-A port has been tested on the SMARC Evaluation board for RZ/G3S
SoC_id  R9A08G045S33GBG revision ESx.y.


::

    ARM CA55 r2p0 (ARMv8-A) 1.1 GHz Single Core, with NEON/FPU, L1$ I/D 32K, L2$ 0MB, L3$ 256K
    ARM M33 240 MHz Dual core - one with FPU function,
    Memory controller for DDR4-1600 (up to 4GB) / LPDDR4-1600 (up to 1GB),
    SD card host interface,
    USB2.0 interfaces,
    CAN interfaces,
    Ethernet AVB,
    Memories
        ACPU System RAM: 2 x 256KB
		MCPU System RAM: 2 x 256KB
        LPDDR4: 1 Gbytes
        QSPI NOR Flash: 16MB
        EMMC: 64 GB
        MICROSD-CARD SLOT: 2 Slots on the Smarc module. 1 Slot on the Carrier Board.



--------
Overview
--------
On RZ/G3S SoCs the BOOTROM starts the cpu at EL3; for this port BL2
will therefore be entered at this exception level.

This LSI has two boot methods: Cortex-M33 cold boot (Cortex-M33 w/o
FPU) and Cortex-A55 cold boot. Which CPU to cold boot from is
controlled by BOOTCPUSEL.

During the Cortex-M33 cold boot, you can choose to boot from either a
Serial Flash Memory device or an SCIF download boot.

During the Cortex-A55 cold boot, you can choose to boot from one of the
following devices: eSD, eMMC, Serial Flash Memory, or SCIF download
boot.



------------
How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the SMARC Evaluation RZ/G3S development kit used in this port.

Base build instruction:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=g3s all BOARD=smarc

Build Options:
~~~~~~~~~~~~~
PLAT_SYSTEM_SUSPEND=vbat
This enables platform suspend in the 'VBat mode'.

PLAT_SYSTEM_SUSPEND=awo
This enables platform suspend in the 'AWO mode'.
This will also enable the CM33 core.

DEBUG=1 LOG_LEVEL=20
If a debug build with logging is required, then use these two build options.
LOG_LEVEL = 0 = LOG_LEVEL_NONE
LOG_LEVEL = 10 = LOG_LEVEL_ERROR
LOG_LEVEL = 20 = LOG_LEVEL_NOTICE
LOG_LEVEL = 30 = LOG_LEVEL_WARNING
LOG_LEVEL = 40 = LOG_LEVEL_INFO
LOG_LEVEL = 50 = LOG_LEVEL_VERBOSE

PLAT_M33_BOOT_SUPPORT=1
This enables the CM33 core.

ENABLE_STACK_PROTECTOR=default
Sets the stack canary to default.
The firmware is set to this value automatically.
This option is thus only required if the option should be set to a value other than default.

NOTE:	Building with PLAT_SYSTEM_SUSPEND=awo and LOG_LEVEL=40 or above breaks the operation of the resume from suspend function.

System Tested:
~~~~~~~~~~~~~~
* u-boot:
  The port has beent tested using mainline uboot with <TBD>

Test script for VBAT mode
~~~~~~~~~~~~~~~~~~~~
.. code:: bash
		tfa_project_path="$1"
		bl33_file_path="$2"
		workspace_path="$3"

		bp_tool_path="$tfa_project_path/tools/renesas"
		fip_tool_path="$tfa_project_path/tools/fiptool"
		bl2_file_path="$tfa_project_path/build/g3s/debug/bl2.bin"
		bl31_file_path="$tfa_project_path/build/g3s/debug/bl31.bin"

		cd "$tfa_project_path"
		make PLAT=g3s BOARD=smarc PLAT_SYSTEM_SUSPEND=vbat all fiptool bptool LOG_LEVEL=40 DEBUG=1 PLAT_M33_BOOT_SUPPORT=1

		${fip_tool_path}/fiptool create --align 16 --soc-fw "$bl31_file_path" --nt-fw "$bl33_file_path" "$workspace_path/fip.bin"
		${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 "$workspace_path/fip.bin" "$workspace_path/fip_vbat_cm33_rzg3s_smarc.srec"

		echo "bl2 file path $bl2_file_path"
		${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_mmc.bin" 0xA3000 mmc
		cat "$workspace_path/bp_mmc.bin" "$bl2_file_path" > "$workspace_path/bl2_bp_mmc.bin"
		${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path/bl2_bp_mmc.bin" "$workspace_path/bl2_bp_mmc_vbat_rzg3s_smarc.srec"

		${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_spi.bin" 0xA3000 spi
		cat "$workspace_path/bp_spi.bin" "$bl2_file_path" > "$workspace_path/bl2_bp_spi.bin"
		${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path/bl2_bp_spi.bin" "$workspace_path/bl2_bp_spi_vbat_rzg3s_smarc.srec"

		${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_esd.bin" 0xA3000 esd
		cat "$workspace_path/bp_esd.bin" "$bl2_file_path" > "$workspace_path/bl2_bp_esd.bin"

Script argument descriptions
	tfa_project_path: This is where the tfa project is located.
	bl33_file_path: This is where the u-boot binary used is located
	workspace_path: This is the output folder of the script.


Test script for AWO mode
~~~~~~~~~~~~~~~~~~~~
.. code:: bash
		tfa_project_path="$1"
		bl33_file_path="$2"
		workspace_path="$3"

		bp_tool_path="$tfa_project_path/tools/renesas"
		fip_tool_path="$tfa_project_path/tools/fiptool"
		bl2_file_path="$tfa_project_path/build/g3s/debug/bl2.bin"
		bl31_file_path="$tfa_project_path/build/g3s/debug/bl31.bin"

		cd "$tfa_project_path"
		make PLAT=g3s BOARD=smarc PLAT_SYSTEM_SUSPEND=awo all fiptool bptool LOG_LEVEL=40 DEBUG=1

		${fip_tool_path}/fiptool create --align 16 --soc-fw "$bl31_file_path" --nt-fw "$bl33_file_path" "$workspace_path/fip.bin"
		${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 "$workspace_path/fip.bin" "$workspace_path/fip_awo_cm33_rzg3s_smarc.srec"

		echo "bl2 file path $bl2_file_path"
		${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_mmc.bin" 0xA3000 mmc
		cat "$workspace_path/bp_mmc.bin" "$bl2_file_path" > "$workspace_path/bl2_bp_mmc.bin"
		${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path/bl2_bp_mmc.bin" "$workspace_path/bl2_bp_mmc_awo_rzg3s_smarc.srec"

		${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_spi.bin" 0xA3000 spi
		cat "$workspace_path/bp_spi.bin" "$bl2_file_path" > "$workspace_path/bl2_bp_spi.bin"
		${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path/bl2_bp_spi.bin" "$workspace_path/bl2_bp_spi_awo_rzg3s_smarc.srec"

Script argument descriptions
	tfa_project_path: This is where the tfa project is located.
	bl33_file_path: This is where the u-boot binary used is located
	workspace_path: This is the output folder of the script.



----------------
How to load TF-A
----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~
Set the device in SCIF mode,
Connect to the COM port provided by the device via some terminal software.
Hit reset and the device will print a message.
The baudrate is 115200
Then it is time to send the device the FlashWriter (Flash_Writer_SCIF_RZG3S_DEV_INTERNAL_MEMORY_0127.mot).

Flash Procedure for EMMC
~~~~~~~~~~~~~~~~~~~~~~~~

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



Flash Procedure for xSPI
~~~~~~~~~~~~~~~~~~~~~~~~
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
4.	Write the CM33 srecord file to device SPI flash
  a.	Enter: XLS2
  b.	Program Top Address: 0x23000
  c.	QSPI Save Address: 0x200000
  d.	Send the CM33 srecord

Flash Procedure for SD
~~~~~~~~~~~~~~~~~~~~~~
1.	Enter fdisk
		sudo fdisk /dev/sdb

		Welcome to fdisk (util-linux 2.37.2).
		Changes will remain in memory only, until you decide to write them.
		Be careful before using the write command.

2.	Remove the existing partitions
		Command (m for help): d
		Partition number (1,2, default 2):

		Partition 2 has been deleted.

		Command (m for help): d
		Selected partition 1
		Partition 1 has been deleted.

3.	Create partitions
		Command (m for help): n
		Partition type
		p   primary (0 primary, 0 extended, 4 free)
		e   extended (container for logical partitions)
		Select (default p):

		Using default response p.
		Partition number (1-4, default 1):
		First sector (2048-30449663, default 2048): 4096
		Last sector, +/-sectors or +/-size{K,M,G,T,P} (4096-30449663, default 30449663): +512M

		Created a new partition 1 of type 'Linux' and of size 512 MiB.

		Command (m for help): n
		Partition type
		p   primary (1 primary, 0 extended, 3 free)
		e   extended (container for logical partitions)
		Select (default p):

		Using default response p.
		Partition number (2-4, default 2):
		First sector (2048-30449663, default 2048): 1052672
		Last sector, +/-sectors or +/-size{K,M,G,T,P} (1052672-30449663, default 30449663):

		Created a new partition 2 of type 'Linux' and of size 14 GiB.

4.	If the signature removal prompt appears after creating either partition, then removed the signature as shown.
		Partition #2 contains a ext4 signature.

		Do you want to remove the signature? [Y]es/[N]o: y

		The signature will be removed by a write command.

5.	Write partitions to disk
		Command (m for help): w
		The partition table has been altered.
		Calling ioctl() to re-read partition table.
		Syncing disks

6.	Remount the SD card by removing it then, plugging it back in.

7.	Format the partitions
		sudo mkfs.ext4 /dev/sdb1
		mke2fs 1.46.5 (30-Dec-2021)
		Creating filesystem with 131072 4k blocks and 32768 inodes
		Filesystem UUID: cb9d787a-fb33-43f2-9a81-2b2049fe6f9d
		Superblock backups stored on blocks:
				32768, 98304

		Allocating group tables: done
		Writing inode tables: done
		Creating journal (4096 blocks): done
		Writing superblocks and filesystem accounting information: done

		sudo mkfs.ext4 /dev/sdb2
		mke2fs 1.46.5 (30-Dec-2021)
		Creating filesystem with 364928 4k blocks and 91392 inodes
		Filesystem UUID: fbd4caa0-690b-43e8-9e67-43e43edf3fa4
		Superblock backups stored on blocks:
				32768, 98304, 163840, 229376, 294912

		Allocating group tables: done
		Writing inode tables: done
		Creating journal (8192 blocks): done
		Writing superblocks and filesystem accounting information: done

8. Remount the SD card by removing it then, plugging it back in.

9. Check partitions were created properly.
		lsblk
		...
		sdb      8:16   1  14.5G  0 disk
		├─sdb1   8:17   1   512M  0 part /media/user/79273262-4ff6-424f-9e7e-a
		└─sdb2   8:18   1    14G  0 part /media/user/c18b1089-2298-40fe-b5eb-c
		...

10. Write TF-A to SD card
		sudo dd if=bp_esd_bl2.bin of=/dev/sdb seek=1
		269+1 records in
		269+1 records out
		137746 bytes (138 kB, 135 KiB) copied, 0.481328 s, 286 kB/s

		sudo dd if=fip.bin of=/dev/sdb seek=800
		1775+1 records in
		1775+1 records out
		908864 bytes (909 kB, 888 KiB) copied, 2.69016 s, 338 kB/s

11. Write Linux files to the SD card
		sudo cp ./Image-r9a08g045s33-smarc.dtb /media/user/79273262-4ff6-424f-9e7e-a
		sudo cp ./Image-smarc-rzg3s.bin /media/user/79273262-4ff6-424f-9e7e-a
		sudo tar -jxvf core-image-bsp-smarc-rzg3s.tar.bz2 -C /media/user/c18b1089-2298-40fe-b5eb-c


----------
Boot trace
----------
::
NOTICE:  BL2: v2.7(debug):v2.5/rzg2l-1.00-2269-g0f3063027-dirty
NOTICE:  BL2: Built : 15:20:23, Oct 26 2023
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
INFO:    Load dst=0x50000000 src=(p:1)0x9e090(1264) len=0xa3c48(1311)
INFO:    Image id=5 loaded: 0x50000000 - 0x500a3c48
INFO:    BL2: Loading image id 36
INFO:    eMMC boot from partition 1
INFO:    Loading image id=36 at address 0x23000
INFO:    emmcdrv_block_len: len: 0x00030000
INFO:    Load dst=0x23000 src=(p:1)0x200000(4096) len=0x30000(384)
INFO:    Image id=36 loaded: 0x23000 - 0x53000
NOTICE:  BL2: Booting BL31
INFO:    Entry point address = 0x44000000
INFO:    SPSR = 0x3cd
NOTICE:  BL31: v2.7(debug):v2.5/rzg2l-1.00-2269-g0f3063027-dirty
NOTICE:  BL31: Built : 15:20:26, Oct 26 2023
INFO:    GICv3 without legacy support detected.
INFO:    ARM GICv3 driver initialized in EL3
INFO:    Maximum SPI INTID supported: 511
INFO:    BL31: Initializing runtime services
INFO:    BL31: cortex_a55: CPU workaround for 1530923 was applied
INFO:    BL31: Preparing for EL3 exit to normal world
INFO:    Entry point address = 0x50000000
INFO:    SPSR = 0x3c5


U-Boot 2021.10 (Aug 29 2023 - 03:07:30 +0000)

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
switch to partitions #0, OK
mmc1 is current device
17695232 bytes read in 1174 ms (14.4 MiB/s)
34365 bytes read in 4 ms (8.2 MiB/s)
Moving Image from 0x48080000 to 0x48200000, end=49350000
## Flattened Device Tree blob at 48000000
   Booting using the fdt blob at 0x48000000
   Loading Device Tree to 0000000057ff4000, end 0000000057fff63c ... OK

Starting kernel ...

######
The kernel starts up and the login prompt is shown.
######

