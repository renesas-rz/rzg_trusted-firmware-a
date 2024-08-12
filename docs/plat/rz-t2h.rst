Renesas RZ/T2H
==============

The "RZ/T2H" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 with the addition two R52 cores, engineers can
easily implement real-time control solutions.

Plug-ins are available for multiple open-source software tools.

::

	On-chip 64-bit Arm Cortex-A55 processor
 	Quad/Dual/Single MPCore cores
 	Maximum operating frequency:
	– Core: 600/1200 MHz
	– DSU: 500/1000 MHz
 	64-bit Arm Cortex-A55 Quad/Dual/Single MPCore cores (revision r2p0)
 	Address space: 32 Gbytes
 	L1 cache:
	– I-cache: 32 Kbytes (with parity)
	– D-cache: 32 Kbytes (with ECC)
 	L2 cache : 0 Kbytes
 	L3 cache : 1024 Kbytes (with ECC)
 	NEON/FPU supported
 	Cryptographic extension supported (Security product only)
 	Arm V8.2-A architecture  
	
	On-chip 32-bit Arm Cortex-R52 processor
 	High-speed realtime control with operating frequency of 500/1000MHz
 	On-chip Dual 32-bit Arm Cortex-R52 (revision r1p4)
 	Tightly coupled memory (TCM) with ECC
	– CPU0, CPU1: 512 KB/64 KB
 	Instruction cache/data cache with ECC
	– CPU0, CPU1: 16 KB per cache
 	High-speed interrupt
 	NEON/FPU supported
 	Harvard architecture with 8-stage pipeline
 	Supports the memory protection unit (MPU)
 	Arm CoreSight architecture, includes support for debugging
	through JTAG and SWD interfaces.
 	No DCLS (Dual Core Lock Step) support

Renesas RZ/T2H reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board        |      Details                                                                                                                          |
+==============+===============+=======================================================================================================================+
| dev1         | Equipped with Renesas RZ/T2H SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzt-series-mpu   //TODO: Update                       |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzt-series-mpu>`__   //TODO: Update

Overview
--------

On RZ/T2H SoCs the BOOTROM starts the cpu at EL3; for this port BL2
will therefore be entered at this exception level.
EL1 is entered before exiting BL31.

After reset release on the LSI, the boot function executes the boot 
processing on Cortex R52 CPU0. The loader is extracted and then the 
second boot CPU can be selected from Cortex-R52 CPU0 or Cortex-A55 Core 0. 
Note: When Cortex-A55 is selected as second boot CPU, reset of Cortex-A55 
Core0 is released and Cortex-R52 CPU0 is transitioned to the WFI state.

This LSI is intended for booting up from external flash memory, you can
choose to boot from one of the following devices: eSD, eMMC, 
Serial Flash Memory, USB or SCIF download boot.

System Tested:
--------------

The current TF-A port has been tested on the Dev1 RZ/T2H
SoC_id  r9a09g077m48gbg (Quad A55), r9a09g077m28gbg (Dual A55) or r9a09g077m08gbg (Single A55) revision ESx.y.

* u-boot:
  The port has been tested using mainline uboot with RZ/T2H Dev board specific patches.

* linux:
  The port has been tested using mainline kernel with the RZ/T2H Dev board added.


How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the dev_1 RZ/T2H development kit used in this port.

Base build instruction:
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

       make PLAT=t2h all BOARD=dev_1

Build Options:
~~~~~~~~~~~~~~

This is used to specify to build for the quad, dual or single core SoC.

+-----------------------+-----------------------------+
| Build option          | Details                     |
+=======================+=============================+
| PLATFORM_CORE_COUNT=4 | Quad core device selected   |
+-----------------------+-----------------------------+
| PLATFORM_CORE_COUNT=2 | Dual core device selected   |
+-----------------------+-----------------------------+
| PLATFORM_CORE_COUNT=1 | Single core device selected |
+-----------------------+-----------------------------+

.. code:: bash

    PLATFORM_CORE_COUNT=4

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
	make PLAT=t2h BOARD=dev_1 PLATFORM_CORE_COUNT=4 BL33=${path_to_uboot_file}/u-boot.bin bl2 fip pkg <Build Options>

How to load TF-A
----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Set the device in scif mode.
	2. Connect to the COM port provided by the device via some terminal software.
	3. Set the baudrate to be 115200
	4. Set the transmit delay to be 0msec/char and 1msec/line
	5. Hit reset and the device will print a message.
	6. Send the FlashWriter mot file[1].

	[1] https://github.com/renesas-rz/<TBD>

Flash Procedure for xSPI0
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Use the ‘Loading the flash writer' procedure.
	2. Modify the XSPIW parameter using this command: XSPIW 0 0x0 0
	3. Set the transmit delay to be 0 msec/char and 0msec/line
	4. Send the BL2 image srec file
	5. Modify the XSPIW parameter using this command: XSPIW 0 0x60000 0
	6. Set the transmit delay to be 0 msec/char and 1msec/line
	7. Send the FIP image srec file

Flash Procedure for xSPI1
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Use the ‘Loading the flash writer' procedure.
	2. Modify the XSPIW parameter using this command: XSPIW 1 0x0 0
	3. Set the transmit delay to be 0 msec/char and 0msec/line
	4. Send the BL2 image srec file
	5. Modify the XSPIW parameter using this command: XSPIW 1 0x60000 0
	6. Set the transmit delay to be 0 msec/char and 1msec/line
	7. Send the FIP image srec file

Flash Procedure for EMMC
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Use the ‘Loading the flash writer’.
	2. Modify the EXT_CSD registers - this step only needs to be performed the first time.
		a. Change the First Register: emmcwecsd 177 2
		b. Change the Second Register: emmcwecsd 179 8
		c. Print the values: emmcrecsd
	3. Write the BL2 srecord to the device
		a. Change the emmc register: emmcwecsd 179 9
		b. Use the emmc write command: emmcw 1 0
		c. Send the BL2 srecord.
		d. Change the emmc register: emmcwecsd 179 8
	4. Write the FIP srecord to the device
		a. Change the emmc register: emmcwecsd 179 9
		b. Use the emmc write command: emmcw 0x300 0
		c. Send the FIP srecord.
		d. Change the emmc register: emmcwecsd 179 8

Flash Procedure for SD
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	Steps 1 to 9 only needs to be performed once.
	1. Enter fdisk
		sudo fdisk /dev/<sd device>

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
		First sector (2048-7744511, default 2048): 4096
		Last sector, +/-sectors or +/-size{K,M,G,T,P} (4096-7744511, default 7744511): +512M

		Created a new partition 1 of type 'Linux' and of size 512 MiB.

		Command (m for help): n
		Partition type
		p   primary (1 primary, 0 extended, 3 free)
		e   extended (container for logical partitions)
		Select (default p):

		Using default response p.
		Partition number (2-4, default 2):
		First sector (1052672-7744511, default 1052672):
		Last sector, +/-sectors or +/-size{K,M,G,T,P} (1052672-7744511, default 7744511):

		Created a new partition 2 of type 'Linux' and of size 3.2 GiB.

		Command (m for help): p
		Disk /dev/sdd: 3.71 GiB, 3965190144 bytes, 7744512 sectors
		Disk model: STORAGE DEVICE
		Units: sectors of 1 * 512 = 512 bytes
		Sector size (logical/physical): 512 bytes / 512 bytes
		I/O size (minimum/optimal): 512 bytes / 512 bytes
		Disklabel type: dos
		Disk identifier: 0x00000000

		Device     Boot   Start     End Sectors  Size Id Type
		/dev/sdd1          4096 1052671 1048576  512M 83 Linux
		/dev/sdd2       1052672 7744511 6691840  3.2G 83 Linux

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
		sudo mkfs.ext4 /dev/<Partition of size 512>
		mke2fs 1.46.5 (30-Dec-2021)
		Creating filesystem with 131072 4k blocks and 32768 inodes
		Filesystem UUID: cb9d787a-fb33-43f2-9a81-2b2049fe6f9d
		Superblock backups stored on blocks:
				32768, 98304

		Allocating group tables: done
		Writing inode tables: done
		Creating journal (4096 blocks): done
		Writing superblocks and filesystem accounting information: done

		sudo mkfs.ext4 /dev/<the other partition>
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
		sudo dd if=bl2_bp_esd.bin of=/dev/sdb seek=1
		269+1 records in
		269+1 records out
		137746 bytes (138 kB, 135 KiB) copied, 0.481328 s, 286 kB/s

		sudo dd if=fip.bin of=/dev/sdb seek=768
		1775+1 records in
		1775+1 records out
		908864 bytes (909 kB, 888 KiB) copied, 2.69016 s, 338 kB/s

	11. Write Linux files to the SD card
		sudo cp ./<t2h device tree>.dtb /media/user/79273262-4ff6-424f-9e7e-a
		sudo cp ./<t2h kernel image>.bin /media/user/79273262-4ff6-424f-9e7e-a
		sudo tar -jxvf <t2h root file system>.tar.bz2 -C /media/user/c18b1089-2298-40fe-b5eb-c

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

	NOTE: For "Reported Core Count (n) is different from PLATFORM_CORE_COUNT", n should be 1, 2, or 4, dependent on the CPU variant.
