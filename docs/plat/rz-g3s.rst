Renesas RZ/G3S
==============

The "RZ/G3S" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a single Arm Cortex®-A55 core with the addition of two M33 cores, engineers can
easily implement various solutions.

Plug-ins are available for multiple open-source software tools.

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

Renesas RZ/G3S reference platforms:
-----------------------------------

+--------------+-------------------------------------------------------------------------------------------------------------------------------------+
| Board        | Details                                                                                                                             |
+==============+===============+=====================================================================================================================+
| smarc        | Equipped with Renesas RZ/G3S SoC                                                                                                    |
|              +-------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rtk9845s33s01000be-rzg3s-smarc-evaluation-board-kit |
+--------------+-------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__

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

System Tested:
--------------

The current TF-A port has been tested on the SMARC Evaluation board for RZ/G3S
SoC_id  R9A08G045S33GBG revision ESx.y.

* u-boot:
  The port has been tested using mainline uboot with the G3S Board added.

* linux:
  The port has been tested using mainline kernel with the G3S Board added.

How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the SMARC Evaluation RZ/G3S development kit used in this port.

Base build instruction:
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

       make PLAT=g3s all BOARD=smarc

Build Options:
~~~~~~~~~~~~~~

If a debug build with logging is required then set DEBUG=1 and set LOG_LEVEL to the desired verbosity.

+--------------+-------------------------------------------+
| Build option | Details                                   |
+==============+===============+===========================+
|LOG_LEVEL=0   | The log level is set to LOG_LEVEL_NONE    |
+--------------+-------------------------------------------+
|LOG_LEVEL=10  | The log level is set to LOG_LEVEL_ERROR   |
+--------------+-------------------------------------------+
|LOG_LEVEL=20  | The log level is set to LOG_LEVEL_NOTICE  |
+--------------+-------------------------------------------+
|LOG_LEVEL=30  | The log level is set to LOG_LEVEL_WARNING |
+--------------+-------------------------------------------+
|LOG_LEVEL=40  | The log level is set to LOG_LEVEL_INFO    |
+--------------+-------------------------------------------+
|LOG_LEVEL=50  | The log level is set to LOG_LEVEL_VERBOSE |
+--------------+-------------------------------------------+

.. code:: bash

    DEBUG=1 LOG_LEVEL=40

Boots the CM33 core in the PD_VCC power domain.

.. code:: bash

	PLAT_M33_BOOT_SUPPORT=1

This enables platform suspend in 'AWO mode'.
It also sets PLAT_M33_BOOT_SUPPORT=1.

.. code:: bash

	PLAT_SYSTEM_SUSPEND=awo

Platform suspend in 'VBAT mode':

.. code:: bash

	PLAT_SYSTEM_SUSPEND=vbat
Sets the stack canary to default.
The firmware is set to this value automatically.
This option is thus only required if the option should be set to a value other than default.

.. code:: bash

	ENABLE_STACK_PROTECTOR=default

NOTE:	Building with PLAT_SYSTEM_SUSPEND=awo and LOG_LEVEL=40 or above breaks the operation of the resume from suspend function.

TF-A Packaging Procedure
~~~~~~~~~~~~~~~~~~~~~~~~

This procedure packages TF-A binaries by:

* Building the relevant binaries.

* Creating the FIP binary.

* Creating the boot parameter block.

* Converting the necessary files into the srecord format.

The 'Build Options' section covers the possible values for <Build Options>.

Argument descriptions:

* path_to_cc_toolset: This is the path to the required toolset.

* path_to_tfa_project: This is where the TF-A project is located

* path_to_uboot_file: This is where the U-Boot binary used is located.

.. code:: bash

	export CROSS_COMPILE=${path_to_cc_toolset}/bin/aarch64-none-elf-
	cd ${path_to_tfa_project}
	make PLAT=g3s BOARD=dev14_1_lpddr BL33=${path_to_uboot_file}/u-boot.bin bl2 fip bptool pkg <Build Options>
	make PLAT=g3s BOARD=smarc BL33=${path_to_uboot_file}/u-boot.bin bl2 fip bptool pkg <Build Options>

How to load TF-A
----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Set the device in scif mode.
	2. Connect to the COM port provided by the device via some terminal software.
	3. Hit reset and the device will print a message.
	4. The baudrate is 115200
	5. Then it is time to send the device the FlashWriter[1] (e.g. Flash_Writer_SCIF_RZG3S_DEV_INTERNAL_MEMORY_0127.mot).

	[1] https://github.com/renesas-rz			//TODO: Update

Flash Procedure for EMMC
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Use the ‘Load the flash writer procedure’.
	2. Modify the EXT_CSD registers - this step only needs to be performed the first time.
		a. Change the First Register
			i.    Use the Flash Writer's command EM_SECSD.
			ii.   Enter the EXT_CSD index: B1
			iii.  Enter the Input Value: 2
		b. Change the Second Register
			i.    Use the Flash Writer's command EM_SECSD.
			ii.   Enter the EXT_CSD index: B3
			iii.  Enter the Input Value: 8
	3. Write the BL2 srecord to the device
		a. Use the Flash Writer's command EM_W.
		b. Partition Select: 1
		c. Input Start Address in sector: 1
		d. Input Program Start Address: a1e00
		e. Send the BL2 srecord.
	4. Write the FIP srecord to the device
		a. Use the Flash Writer's command EM_W.
		b. Partition Select: 1
		c. Input Start Address in sector: 320
		d. Input Program Start Address: 00000
		e. Send the FIP srecord file
	5. Write the CM33 srecord file to device eMMC
		a. Use the Flash Writer's command EM_W.
		b. Partition Select: 1
		c. Input Start Address in sector: 1000
		d. Input Program Start Address: 23000
		e. Send the CM33 srecord file

Flash Procedure for xSPI
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	Steps 1 to 9 only needs to be performed once.
	1. Use the ‘Load the flash writer procedure’.
	2. Write the BL2 srecord to the device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0xA1E00
		c. QSPI Save Address: 0x00000
		d. Send the BL2 srecord
	3. Write the FIP srecord to the device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0x00000
		c. QSPI Save Address: 0x64000
		d. Send the FIP srecord
	4. Write the CM33 srecord file to device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0x23000
		c. QSPI Save Address: 0x200000
		d. Send the CM33 srecord

Flash Procedure for SD
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	Steps 1 to 9 only needs to be performed once.
	1. Enter fdisk
		sudo fdisk /dev/sdb

		Welcome to fdisk (util-linux 2.37.2).
		Changes will remain in memory only, until you decide to write them.
		Be careful before using the write command.

	2. Remove the existing partitions
		Command (m for help): d
		Partition number (1,2, default 2):

		Partition 2 has been deleted.

		Command (m for help): d
		Selected partition 1
		Partition 1 has been deleted.

	3. Create partitions
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

	4. If the signature removal prompt appears after creating either partition, then removed the signature as shown.
		Partition #2 contains a ext4 signature.

		Do you want to remove the signature? [Y]es/[N]o: y

		The signature will be removed by a write command.

	5. Write partitions to disk
		Command (m for help): w
		The partition table has been altered.
		Calling ioctl() to re-read partition table.
		Syncing disks

	6. Remount the SD card by removing it then, plugging it back in.

	7. Format the partitions
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

	Note: To boot Linux from SD0, the U-Boot environment variables may require a change.
	setenv bootargs 'rw rootwait earlycon root=/dev/mmcblk0p2'
	setenv bootcmd 'ext4load mmc 0:1 0x48080000 Image-smarc-rzg3s.bin; ext4load mmc 0:1 0x48000000 Image-r9a08g045s33-smarc.dtb; booti 0x48080000 - 0x48000000'
	boot

Boot trace
----------

.. code-block:: text

	NOTICE:  BL2: v2.7(release): <git describe description>
	NOTICE:  BL2: Built :  <build time and date>
	NOTICE:  BL2: Booting BL31
	NOTICE:  BL31: v2.7(release):<git describe description>
	NOTICE:  BL31: Built : <build time and date>

	######
	U-Boot starts up and the Linux Kernel is loaded.
	######

	######
	The kernel starts up and the login prompt is shown.
	######

