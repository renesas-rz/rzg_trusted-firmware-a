Renesas RZ/V2N
==============

The "RZ/V2N" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 and in addition a M33 core, engineers can
easily implement real-time control and
industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.

+----------------+------------------+------------------+
|  Part Number   |  GE3D            |  Security        |
+================+==================+==================+
| R9A09G056N48GBG|  N/A             |  N/A             |
+                +                  +                  +
| R9A09G056N47GBG|  GE3D            |  (Mali-G31)      |
+                +                  +                  +
| TODO - add more|  N/A             |  Available       |
+                +                  +                  +
| R9A09G056N41GBG|  GE3D (Mali-G31) |                  |
+----------------+------------------+------------------+

::

	Arm® Cortex®-A55 1.8 GHz Quad Core, FPU, Neon™, L1-caches and L3-cache
	Arm® Cortex®-M33 200MHz, FPU and DSP-extension,
	DRP-AI, Mali™-G31 (GE3D), Mali™-C55 (ISP), 6 MB of on-chip SRAM,
	2ch GbEthernet MAC,
	USB2.0, USB3.2
	Gen 2x1, 4-MIPI® CSI-2® camera input interface,
	1-MIPI® DSI® video output interface,
	PCIe® Gen3 4Lane or 2-2Lane
	(EP/RC), various communication interfaces such as an xSPI, eMMC™, I2S (TDM), I3C®, PDM, and security functions.

Renesas RZ/V2N reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board        | Details                                                                                                                               |
+==============+=======================================================================================================================================+
| evk_1        | Equipped with Renesas RZ/V2N SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/en/design-resources/boards-kits/rz-v2n-evk                                                                    |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzv-embedded-ai-mpus>`__

Overview
--------

On RZ/V2N SoCs the BOOTROM starts the CPU at EL3; for this port BL2
will therefore be entered at this exception level.

EL1 is entered before exiting BL31.

BL2 initializes DDR before determining the boot reason (cold or warm).

Once BL2 boots, it determines the boot reason, writes it to shared
memory (BOOT_KIND_BASE) together with the BL31 parameters
(PARAMS_BASE) and jumps to BL31.

To all effects, BL31 is as if it is being entered in reset mode since
it still needs to initialize the rest of the cores, this is the reason
behind using direct shared memory access to BOOT_KIND_BASE and
PARAMS_BASE instead of using registers to get to those locations (see
el3_common_macros.S and bl31_entrypoint.S for the RESET_TO_BL31 use
case).

System Tested:
--------------

The current TF-A port has been tested on the Evaluation Kit RZ/V2N platforms.
SoC_id r9a09g056h4 revision ESx.y.

* u-boot:
  The port has been tested using mainline uboot with the appropriate RZ/V2N evk board specific patches.

* linux:
  The port has been tested using mainline kernel with the RZ/V2N evk board added.

How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions.

What follows is customized to the RZ/V2N evaluation kits used in these ports.

Base build instruction:
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

	make PLAT=v2n all BOARD=evk_1 ENABLE_STACK_PROTECTOR=all

Build Options:
~~~~~~~~~~~~~~

Modify LOG_LEVEL to change to the desired verbosity.

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

For example, to set the output to verbose logging:

.. code:: bash

    LOG_LEVEL=50

To enable debugging set the following option:

.. code:: bash

	DEBUG=1

To enable basic DDR ECC set the following option:

.. code:: bash

	PLAT_DDR_ECC=1

Boots the CM33 core

.. code:: bash

	PLAT_M33_BOOT_SUPPORT=1

This enables platform suspend in 'AWO mode with DDR Retention'.
Follow the 'Build TF-A for Suspend Procedure' (shown below) to use this option.

.. code:: bash

	PLAT_SYSTEM_SUSPEND=1

This enables stack protection for all BL stages.

.. code:: bash

	ENABLE_STACK_PROTECTOR=all

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
	make PLAT=v2n BOARD=evk_1 ENABLE_STACK_PROTECTOR=all BL33=${path_to_uboot_file}/u-boot.bin bl2 fip bptool pkg <Build Options>

Build TF-A for Suspend Procedure
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This procedure builds TF-A with platform suspend support enabled.

.. code:: bash

	export CROSS_COMPILE=${path_to_cc_toolset}/bin/aarch64-none-elf-
	cd ${path_to_tfa_project}
	make PLAT=v2n BOARD=evk_1 BL33=${path_to_uboot_file}/u-boot.bin bl2 fip bptool pkg PLAT_SYSTEM_SUSPEND=1

The build option LOG_LEVEL can be added to the above command to set the desired logging level.
It is recommended to avoid using other build options when building for suspend support.
Only the evk_2 board supports the enabling of the suspend procedure using PLAT_SYSTEM_SUSPEND=1.
Use the 'Flash Procedure for xSPI CM33 boot' (which is shown below) to flash the device.

How to flash TF-A
-----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Set the device in scif mode.
	2. Connect to the COM port provided by the device via some terminal software.
	3. Set the baudrate to be 115200
	4. Set the transmit delay to be 0msec/char and 1msec/line
	5. Hit reset and the device will print a message.
	6. Send the FlashWriter .mot file[1].

	[1] https://github.com/renesas-rz/<TBD>

Flash Procedure for xSPI
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Use the ‘Load the flash writer procedure’.
	2. Write the BL2 srecord to the device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0x8101e00
		c. QSPI Save Address: 0x0
		d. Send the BL2 srecord
	3. Write the FIP srecord to the device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0x0
		c. QSPI Save Address: 0x60000
		d. Send the FIP srecord

Flash Procedure for EMMC
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

	1. Use the ‘Load the flash writer procedure’.
	2. Modify the EXT_CSD registers - this step only needs to be performed the first time.
		a. Change the First Register
			i. Use the Flash Writer's command EM_SECSD.
			ii. Enter the EXT_CSD index: B1
			iii.  Enter the Input Value: 2
		b. Change the Second Register
			i. Use the Flash Writer's command EM_SECSD.
			ii. Enter the EXT_CSD index: B3
			iii.  Enter the Input Value: 8
	3. Write the BL2 srecord to the device
		a. Use the Flash Writer's command EM_W.
		b. Partition Select: 1
		c. Input Start Address in sector: 1
		d. Input Program Start Address: 8101e00
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

	Steps 1 to 11 only needs to be performed once.
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
		sudo fdisk /dev/<sd device>

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

	13. Write Linux files to the SD card (only required if booting Linux from the same card)
		sudo cp ./<v2n device tree>.dtb /media/user/79273262-4ff6-424f-9e7e-a
		sudo cp ./<v2n kernel image>.bin /media/user/79273262-4ff6-424f-9e7e-a
		sudo tar -jxvf <v2n root file system>.tar.bz2 -C /media/user/c18b1089-2298-40fe-b5eb-c

Flash Procedure for xSPI CM33 boot
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This procedure is required when implementing the system suspend functionality.

.. code-block:: text

	1. Use the ‘Load the flash writer procedure’.
	2. Write the BL2 srecord to the device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0x8101e00
		c. QSPI Save Address: 0x100000
		d. Send the BL2 srecord
	3. Write the FIP srecord to the device SPI flash
		a. Enter: XLS2
		b. Program Top Address: 0x0
		c. QSPI Save Address: 0x280000
		d. Send the FIP srecord
	4. Flash the CM33 program to the device SPI flash
		a. Follow the procedure provided with the CM33 program to flash the program for the CM33 core.

Boot trace
----------

.. code-block:: text

	NOTICE:  BL2: v2.10.5(release): <git describe description>
	NOTICE:  BL2: Built :  <build time and date>
	NOTICE:  BL2: Booting BL31
	NOTICE:  BL31: v2.10.5(release):<git describe description>
	NOTICE:  BL31: Built : <build time and date>

	######
	U-Boot starts up and the Linux Kernel is loaded.
	######

	######
	The kernel starts up and the login prompt is shown.
	######
