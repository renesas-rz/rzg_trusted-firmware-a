Renesas RZ/G3E
==============

The "RZ/G3E" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad/dual core Arm Cortex®-A55 and a M33 core, engineers can
easily implement industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/G3E reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------+
| Board        | Details                                                                                                 |
+==============+===============+=========================================================================================+
| dev1         | Equipped with Renesas RZ/G3E SoC                                                                        |
|              +---------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>     /TODO: Update |
+--------------+---------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__ //TODO: Update

The current TF-A port has been tested on the Dev1 RZ/G3E
SoC_id  R9A09G047E53GBG revision ESx.y.


::

	Arm® Quad/Dual Core Cortex®-A55 1.8 GHz, Neon™, FPU, L1-caches and L3-cache
		Options:  Security extensions, NPU (Ethos-U55)
	Arm® Cortex®-M33 200MHz, FPU and DSP-extension,
	512KB on-chip SRAM, External DDR interface, xSPI interface, SDHI
	Mali-G52 (GE3D), Image Scaling Unit (VSP), Fine Display Processor (FDP), H.264/H.265 Video codec unit (VCD),
	2ch GbEthernet MAC, USB2.0 (1 ch. host and 1 ch. host/function), USB3.2 ( 1 ch. host only),
		PCIE x 1 (1 or 2 lanes), CAN/CANFD (6 ch.), SCI (10 ch.), SPI (3 ch.), I2C (9 ch.), I3C (1ch.)
	Gen 2x1, 4-MIPI® CSI-2® camera input interface,
		2 of the following: MIPI DSI(1 ch.), Parallel interface (1 ch.), LVDS (2 ch.)
	General purpose timer (16 ch.), CMTW (8 ch.)


Overview
--------
On RZ/G3E SoCs the BOOTROM starts the cpu at EL3; for this port BL2
will therefore be entered at this exception level.
EL1 is entered before exiting BL31.

BL2 initializes DDR before determining the boot reason (cold or warm).

Once BL2 boots, parameters for the subsequent images to be loaded are
stored in shared memory (PARAMS_BASE) and after which BL2 switches to
BL31.

How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the Dev_1 RZ/G3E development kit used in this port.

Build Tested:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=g3e all BOARD=dev_1 ENABLE_STACK_PROTECTOR=default
       make PLAT=g3e BOARD=dev_1 bptool all ENABLE_STACK_PROTECTOR=default DEBUG=1 LOG_LEVEL=40

TF-A Build Procedure
~~~~~~~~~~~~~~~~~~~~
.. code:: bash

       make PLAT=g3e BOARD=dev_1 BL33=${path_to_u-boot_file}/u-boot.bin bl2 fip bptool
       ./tools/renesas/bptool build/g3e/release/bl2.bin build/g3e/release/bp_spi.bin 0x08004000 spi
       cat build/g3e/release/bp_spi.bin build/g3e/release/bl2.bin > build/g3e/release/bp_spi_bl2.bin
       ${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x08003600 --srec-forceS3 build/g3e/release/bp_spi_bl2.bin build/g3e/release/bp_spi_bl2.srec
       ${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 build/g3e/release/fip.bin build/g3e/release/fip.srec


How to load TF-A
----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~
Set the device in SCIF mode,
Connect to the COM port provided by the device via some terminal software.
Hit reset and the device will print a message.
The baudrate is 115200
Then send the device the FlashWriter[1] (e.g. Flash_Writer_SCIF_RZG3E_DEV_LPDDR4X_0117.mot).
[1] https://github.com/renesas-rz			//TODO: Update

Flash Procedure for xSPI
~~~~~~~~~~~~~~~~~~~~~~~~
.. code-block:: text

	1.	Use the ‘Load the flash writer procedure’.
	2.	Write the BL2 srecord to the device SPI flash
		a.	Enter: XLS2
		b.	Program Top Address: 0x8003600
		c.	QSPI Save Address: 0x00000
		d.	Send the BL2 srecord
		e.	The output should show as follows:
	3.	Write the BL31 srecord to the device SPI flash
		a.	Enter: XLS2
		b.	Program Top Address: 0x00000
		c.	QSPI Save Address: 0x60000
		d.	Send the BL31 srecord

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
	        i.	 Use the Flash Writer's command EM_SECSD.
	        ii.	 Enter the EXT_CSD index: B3
	        iii. Enter the Input Value: 8
	3.	Write the bl2 srecord to the device
		a.	Use the Flash Writer's command EM_W.
		b.	Partition Select: 1
		c.	Input Start Address in sector: 1
		d.	Input Program Start Address: 8003600
		e.	Send the bl2 srecord.
	4.	Write the fip srecord to the device
		a.	Use the Flash Writer's command EM_W.
		b.	Partition Select: 1
		c.	Input Start Address in sector: 300
		d.	Input Program Start Address: 8003600
		e.	Send the fip srecord file

Flash Procedure for SD
~~~~~~~~~~~~~~~~~~~~~~
.. code-block:: text

	Steps 1 to 9 only needs to be performed once.
	1.	Enter fdisk
			sudo fdisk /dev/<sd device>

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
			sudo cp ./<g3e device tree>.dtb /media/user/79273262-4ff6-424f-9e7e-a
			sudo cp ./<g3e kernel image>.bin /media/user/79273262-4ff6-424f-9e7e-a
			sudo tar -jxvf <g3e root file system>.tar.bz2 -C /media/user/c18b1089-2298-40fe-b5eb-c

Boot trace
----------
.. code-block:: text

	NOTICE:  BL2: v2.7(debug):2.7.0/rz_soc_dev-46-g77ba4630a-dirty
	NOTICE:  BL2: Built : 11:27:10, Mar  6 2024
	INFO:    BL2: Doing platform setup
	INFO:    Configuring TrustZone Controller
	INFO:    Total 1 regions set.
	INFO:    Configuring TrustZone Controller
	INFO:    Total 2 regions set.
	INFO:    Configuring TrustZone Controller
	INFO:    Total 1 regions set.
	INFO:    Configuring TrustZone Controller
	INFO:    Total 1 regions set.
	INFO:    Configuring TrustZone Controller
	INFO:    Total 1 regions set.
	INFO:    Configuring TrustZone Controller
	INFO:    Total 1 regions set.
	INFO:    DDR: Setup (Rev. 1.20)
	INFO:    BL2: Loading image id 3
	INFO:    Loading image id=3 at address 0x44000000
	INFO:    Image id=3 loaded: 0x44000000 - 0x4400f0c9
	INFO:    BL2: Loading image id 5
	INFO:    Loading image id=5 at address 0x50000000
	INFO:    Image id=5 loaded: 0x50000000 - 0x500a40d0
	NOTICE:  BL2: Booting BL31
	INFO:    Entry point address = 0x44000000
	INFO:    SPSR = 0x3cd
	NOTICE:  BL31: v2.7(debug):2.7.0/rz_soc_dev-46-g77ba4630a-dirty
	NOTICE:  BL31: Built : 11:27:11, Mar  6 2024
	INFO:    GICv3 without legacy support detected.
	INFO:    ARM GICv3 driver initialized in EL3
	INFO:    Maximum SPI INTID supported: 991
	INFO:    BL31: Initializing runtime services
	INFO:    BL31: cortex_a55: CPU workaround for 1530923 was applied
	INFO:    BL31: Preparing for EL3 exit to normal world
	INFO:    Entry point address = 0x50000000
	INFO:    SPSR = 0x3c5


	<U-Boot starts up and begins to load the Linux kernel.>

	<The kernel starts up and the login prompt is shown.>

