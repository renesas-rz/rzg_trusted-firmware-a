Renesas RZ/G3L
==============

The "RZ/G3L" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad/dual core Arm Cortex®-A55 and an M33 core, engineers can
easily implement industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.

::

	Arm® Quad/Dual Core Cortex®-A55 1.2 GHz, Neon™, FPU, L1-caches and L3-cache
	Arm® Cortex®-M33 200MHz, FPU and DSP-extension,
	Image Scaling Unit2 (ISU2), Options:  Graphics and Video processing (GE3D/VCP)
	512KB on-chip SRAM, External DDR interface (1ch.), xSPI interface (2 ch.), SDHI (3 ch.), External Bus state controller with 16bit for SRAM
	2ch GbEthernet MAC, USB2.0 (1 ch. host and 1 ch. host/function), PCIE x 1 (1 ch. 1 lane), CAN/CANFD (3 ch.), RSCI (4 ch.), I2C (4 ch.), I3C (1ch.)
	Gen 2x1, 4-MIPI® CSI-2® camera input interface,	selectable 1 of the following: MIPI DSI(1 ch.), Parallel interface (1 ch.), LVDS (1 ch.)
	General purpose timer (8 ch.), MTU (8 ch.)
	Serial sound interface (full-duplex 4 ch), SPDIF input/output interfaces (1 ch.), Pulse density modulation (PDM) input interfaces (3 ch.)
	1.0 Msps 12-bit ADC (8 ch.)
	Internal temperature sensors (1 ch.)
	Hardware cryptographic engine

Renesas RZ/G3L reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------+
| Board        | Details                                                                                                 |
+==============+===============+=========================================================================================+
| dev1         | Equipped with Renesas RZ/G3L SoC                                                                        |
|              +---------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>     /TODO: Update |
+--------------+---------------------------------------------------------------------------------------------------------+
| smarc        | Equipped with Renesas RZ/G3L SoC                                                                        |
|              +---------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>     /TODO: Update |
+--------------+---------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__

Overview
--------

On RZ/G3L SoCs the BOOTROM starts the cpu at EL3; for this port BL2
will therefore be entered at this exception level.
EL1 is entered before exiting BL31.

BL2 initializes DDR before determining the boot reason (cold or warm).

Once BL2 boots, parameters for the subsequent images to be loaded are
stored in shared memory (PARAMS_BASE) and after which BL2 switches to
BL31.

System Tested:
--------------

The current TF-A port has been tested on the dev_1 and smarc RZ/G3L boards.
SoC_id  R9A08G046L46GBG revision ESx.y.

* u-boot:
  The port has been tested using mainline uboot with the G3L Board added.

* linux:
  The port has been tested using mainline kernel with the G3L Board added.

How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the RZ/G3L development kit used in this port.

Base build instruction:
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

    make PLAT=g3l all BOARD=dev_1
    make PLAT=g3l all BOARD=smarc

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

For example, to build with debug and verbose logging:

.. code:: bash

    DEBUG=1 LOG_LEVEL=50


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
	make PLAT=g3l BOARD=dev_1 BL33=${path_to_uboot_file}/u-boot.bin bl2 fip bptool pkg <Build Options>
	make PLAT=g3l BOARD=smarc BL33=${path_to_uboot_file}/u-boot.bin bl2 fip bptool pkg <Build Options>

How to load TF-A
----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Set the device in scif mode.
	2. Connect to the COM port provided by the device via some terminal software.
	3. Hit reset and the device will print a message.
	4. The baudrate is 115200
	5. Then send the device the FlashWriter[1] (e.g. Flash_Writer_SCIF_RZG3L_DEV_LPDDR4X_0117.mot).
	[1] https://github.com/renesas-rz			//TODO: Update

Flash Procedure for xSPI
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Use the ‘Load the flash writer procedure’.
	2. Write the BL2 srecord to the device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0x21E00
		c. QSPI Save Address: 0x00000
		d. Send the BL2 srecord
	3. Write the FIP srecord to the device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0x00000
		c. QSPI Save Address: 0x60000
		d. Send the FIP srecord

Flash Procedure for EMMC
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1.  Use the ‘Load the flash writer procedure’.
	2.  Modify the EXT_CSD registers - this step only needs to be performed the first time.
		a.  Change the First Register
			i.   Use the Flash Writer's command EM_SECSD.
			ii.  Enter the EXT_CSD index: B1
			iii. Enter the Input Value: 2
		b.  Change the Second Register
			i.  Use the Flash Writer's command EM_SECSD.
			ii.  Enter the EXT_CSD index: B3
			iii. Enter the Input Value: 8
	3. Write the BL2 srecord to the device
		a. Use the Flash Writer's command EM_W.
		b. Partition Select: 1
		c. Input Start Address in sector: 1
		d. Input Program Start Address: 21E00
		e. Send the BL2 srecord.
	4. Write the FIP srecord to the device
		a. Use the Flash Writer's command EM_W.
		b. Partition Select: 1
		c. Input Start Address in sector: 300
		d. Input Program Start Address: 0
		e. Send the FIP srecord file

Flash Procedure for SD
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	Steps 1 to 9 only needs to be performed once.
	1. Find the SD card partitions. In this case it they are sdb1 and sdb2, however they may be different on your system.
		lsblk
		...
		sdb      8:16   1  14.5G  0 disk
		├─sdb1   8:17   1   512M  0 part /media/user/79273262-4ff6-424f-9e7e-a
		└─sdb2   8:18   1    14G  0 part /media/user/c18b1089-2298-40fe-b5eb-c
		...

	2. Unmount all of the SD card partitions
		sudo umount /dev/sdb1
		sudo umount /dev/sdb2

	3. Enter fdisk
		sudo fdisk /dev/sdb

		Welcome to fdisk (util-linux 2.37.2).
		Changes will remain in memory only, until you decide to write them.
		Be careful before using the write command.

	4. Remove the existing partitions
		Command (m for help): d
		Partition number (1,2, default 2):

		Partition 2 has been deleted.

		Command (m for help): d
		Selected partition 1
		Partition 1 has been deleted.

	5. Create partitions
		Command (m for help): n
		Partition type
		p   primary (0 primary, 0 extended, 4 free)
		e   extended (container for logical partitions)
		Select (default p):

		Using default response p.
		Partition number (1-4, default 1):
		First sector (2048-7744511, default 2048): 6144
		Last sector, +/-sectors or +/-size{K,M,G,T,P} (6144-62333951, default 62333951): +512M

		Created a new partition 1 of type 'Linux' and of size 512 MiB.

		Command (m for help): n
		Partition type
		p   primary (1 primary, 0 extended, 3 free)
		e   extended (container for logical partitions)
		Select (default p):

		Using default response p.
		Partition number (2-4, default 2):
		First sector (1054720-7744511, default 1054720): 1054720
		Last sector, +/-sectors or +/-size{K,M,G,T,P} (1054720-62333951, default 62333951):

		Created a new partition 2 of type 'Linux' and of size 3.2 GiB.

		Command (m for help): p
		Disk /dev/sdd: 3.71 GiB, 3965190144 bytes, 7744512 sectors
		Disk model: STORAGE DEVICE
		Units: sectors of 1 * 512 = 512 bytes
		Sector size (logical/physical): 512 bytes / 512 bytes
		I/O size (minimum/optimal): 512 bytes / 512 bytes
		Disklabel type: dos
		Disk identifier: 0x00000000

		Device     Boot   Start      End  Sectors  Size Id Type
		/dev/sdb1          6144  1054719  1048576  512M 83 Linux
		/dev/sdb2       1054720 62333951 61279232 29.2G 83 Linux

	6. If the signature removal prompt appears after creating either partition, then removed the signature as shown.
		Partition #2 contains a ext4 signature.

		Do you want to remove the signature? [Y]es/[N]o: y

		The signature will be removed by a write command.

	7. Write partitions to disk
		Command (m for help): w
		The partition table has been altered.
		Calling ioctl() to re-read partition table.
		Syncing disks

	8. Remount the SD card by removing it then, plugging it back in.

	9. Format the partitions
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

	10. Remount the SD card by removing it then, plugging it back in.

	11. Check partitions were created properly.
		lsblk
		...
		sdb      8:16   1  14.5G  0 disk
		├─sdb1   8:17   1   512M  0 part /media/user/79273262-4ff6-424f-9e7e-a
		└─sdb2   8:18   1    14G  0 part /media/user/c18b1089-2298-40fe-b5eb-c
		...

	12. Write TF-A to SD card
		sudo dd if=bl2_bp_esd.bin of=/dev/sdb seek=1
		269+1 records in
		269+1 records out
		137746 bytes (138 kB, 135 KiB) copied, 0.481328 s, 286 kB/s

		sudo dd if=fip.bin of=/dev/sdb seek=768
		1775+1 records in
		1775+1 records out
		908864 bytes (909 kB, 888 KiB) copied, 2.69016 s, 338 kB/s

	13. If Linux is required on this SD card, then follow the steps below.
		sudo cp ./<g3l device tree>.dtb /media/user/79273262-4ff6-424f-9e7e-a
		sudo cp ./<g3l kernel image>.bin /media/user/79273262-4ff6-424f-9e7e-a
		sudo tar -jxvf <g3l root file system>.tar.bz2 -C /media/user/c18b1089-2298-40fe-b5eb-c


Boot trace
----------

.. code-block:: text

	NOTICE:  BL2: v2.10.5(release): <git describe description>
	NOTICE:  BL2: Built :  <build time and date>
	NOTICE:  BL2: SYS_LSI_MODE: <contents of register SYS_LSI_MODE>
	NOTICE:  BL2: SYS_LSI_DEVID: <contents of register SYS_LSI_DEVID>
	NOTICE:  BL2: SYS_LSI_PRR: <contents of register SYS_LSI_PRR>
	NOTICE:  BL2: Booting BL31
	NOTICE:  BL31: v2.10.5(release):<git describe description>
	NOTICE:  BL31: Built : <build time and date>


	######
	U-Boot starts up and the Linux Kernel is loaded.
	######

	######
	The kernel starts up and the login prompt is shown.
	######

