Renesas RZ/N2H
==============

The "RZ/N2H" high-end 64-bit and 32-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 and additional dual core of Arm Cortex®-R52, engineers can
easily implement industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/N2H reference platforms:
-----------------------------------

+--------------+-------------------------------------------------------------------------------------------------------------------------------------+
| Board        | Details                                                                                                                             |
+==============+===============+=====================================================================================================================+
| eval         | Equipped with Renesas RZ/N2H SoC                                                                                                    |
|              |                                                                                                                                     |
|              | https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzn     /TODO: Update                               |
+--------------+-------------------------------------------------------------------------------------------------------------------------------------+


`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzn#related_boards__kits>`__ //TODO: Update

The current TF-A port has been tested on the RZ/N2H Evaluation kit
SoC_id  r9a09g087m48gbg (Quad A55), r9a09g087m28gbg (Dual A55) or r9a09g087m08gbg (Single A55) revision ESx.y.

.. code-block:: text

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
 	L2 cache: 0 bytes
 	L3 cache: 1024 Kbytes (with ECC)
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


Overview
--------
On RZ/N2H SoCs the BOOTROM starts the cpu at EL3; for this port BL2
will therefore be entered at this exception level.
EL1 is entered before exiting BL31.

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

[1] https://github.com/renesas-rz/meta-rzg2/tree/BSP-1.0.5/recipes-bsp/arm-trusted-firmware/files	//TODO: Update


How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the RZ/N2H Evaluation kit used in this port.

Build Tested:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=n2h all BOARD=eval PLATFORM_CORE_COUNT=4 DEBUG=1 LOG_LEVEL=40

System Tested:
~~~~~~~~~~~~~~
* u-boot:
  The port has beent tested using mainline uboot with RZ/N2H Evaluation board
  specific patches.

|  commit 46ce9e777c1314ccb78906992b94001194eaa87b		//TODO: Update
|  Author: Heiko Schocher <hs@denx.de>
|  Date:   Tue Nov 3 15:22:36 2020 +0100

* linux:
  The port has beent tested using mainline kernel.

|  commit f8394f232b1eab649ce2df5c5f15b0e528c92091		//TODO: Update
|  Author: Linus Torvalds <torvalds@linux-foundation.org>
|  Date:   Sun Nov 8 16:10:16 2020 -0800
|  Linux 5.10-rc3

TF-A Build Procedure
~~~~~~~~~~~~~~~~~~~~
.. code:: bash

				//TODO: Update
	sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib build-essential chrpath socat cpio python python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping libsdl1.2-dev xterm p7zip-full libyaml-dev
	cd rzg_bsp_v3.0.0/
	patch -p1 < ../v300-to-v300update1.patch
	cp meta-renesas/docs/template/conf/eval-rzN2H/*.conf ./conf/
	source poky/oe-init-build-env
	bitbake core-image-minimal

.. code:: bash

       make PLAT=n2h all BOARD=eval PLATFORM_CORE_COUNT=4 DEBUG=1 LOG_LEVEL=40


How to load TF-A
----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~

1.	Set the device in SCIF mode,
2.	Connect to the COM port provided by the device via some terminal software.
3.	Set the baudrate to be 115200
4.	Set the transmit delay to be 0msec/char and 1msec/line
5.	Hit reset and the device will print a message.
6.	Send the FlashWriter mot file[2].

[2] https://github.com/renesas-rz/rzg2_flash_writer/tree/rz_T2H			//TODO: Update

Flash Procedure for xSPI
~~~~~~~~~~~~~~~~~~~~~~~~
.. code-block:: text

	1.	Use the ‘Loading the flash writer' procedure.
	2.	Modify the XSPIW parameter using this command: XSPIW 0 0x0 0
	3.	Set the transmit delay to be 0 msec/char and 0msec/line
	4.	Send the BL2 image srec file
	5.	Modify the XSPIW parameter using this command: XSPIW 0 0x60000 0
	6.	Set the transmit delay to be 0 msec/char and 1msec/line
	7.	Send the FIP image srec file

Flash Procedure for EMMC
~~~~~~~~~~~~~~~~~~~~~~~~
.. code-block:: text

	1.	Use the ‘Loading the flash writer’.
	2.	Modify the EXT_CSD registers - this step only needs to be performed the first time.
		a.	Change the First Register: emmcwecsd 177 2
		b.	Change the Second Register: emmcwecsd 179 8
		c.	Print the values: emmcrecsd
	3.	Write the bl2 srecord to the device
		a.	Change the emmc register: emmcwecsd 179 9
		b.	Use the emmc write command: emmcw 1 0
		c.	Send the bl2 srecord.
		d.	Change the emmc register: emmcwecsd 179 8
	4.	Write the fip srecord to the device
		a.	Change the emmc register: emmcwecsd 179 9
		b.	Use the emmc write command: emmcw 0x300 0
		c.	Send the fip srecord.
		d.	Change the emmc register: emmcwecsd 179 8

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
			sudo cp ./<n2h device tree>.dtb /media/user/79273262-4ff6-424f-9e7e-a
			sudo cp ./<n2h kernel image>.bin /media/user/79273262-4ff6-424f-9e7e-a
			sudo tar -jxvf <n2h root file system>.tar.bz2 -C /media/user/c18b1089-2298-40fe-b5eb-c

	Boot trace
	----------
.. code-block:: text

	NOTICE:  BL2: v2.7(debug):V2.7/RZT2H-1.00-BETA-83-gea81b2135
	NOTICE:  BL2: Built : 21:32:28, Mar 23 2023
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
	INFO:    Configuring TrustZone Controller
	INFO:    Total 1 regions set.
	INFO:    Configuring TrustZone Controller
	INFO:    Total 1 regions set.
	INFO:    BL2: Loading image id 3
	INFO:    Loading image id=3 at address 0x44000000
	INFO:    Image id=3 loaded: 0x44000000 - 0x4400f0c9
	INFO:    BL2: Loading image id 5
	INFO:    Loading image id=5 at address 0x50000000
	INFO:    Image id=5 loaded: 0x50000000 - 0x5009a5d8
	NOTICE:  BL2: Booting BL31
	INFO:    Entry point address = 0x44000000
	INFO:    SPSR = 0x3cd
	NOTICE:  BL31: v2.7(debug):V2.7/RZT2H-1.00-BETA-82-g0af46693b-dirty
	NOTICE:  BL31: Built : 20:12:16, Mar 23 2023
	INFO:    GICv3 without legacy support detected.
	INFO:    ARM GICv3 driver initialized in EL3
	INFO:    Maximum SPI INTID supported: 991
	INFO:    BL31: Initializing runtime services
	INFO:    BL31: cortex_a55: CPU workaround for 1530923 was applied
	INFO:    BL31: Preparing for EL3 exit to normal world
	INFO:    Entry point address = 0x50000000
	INFO:    SPSR = 0x3c5


	U-Boot 2021.10-g5502146d18 (Feb 28 2023 - 19:48:45 +0000)

	CPU:   Renesas Electronics CPU rev 1.0
	Model: Renesas Development EVK based on r9a09g057h4
	DRAM:  1.9 GiB
	MMC:   mmc@15c00000: 0, mmc@15c20000: 1
	Loading Environment from MMC... OK
	In:    serial@11c01400
	Out:   serial@11c01400
	Err:   serial@11c01400
	Net:   No ethernet found.
	Hit any key to stop autoboot:  0
	Card did not respond to voltage select! : -110				//TODO: Update
	Card did not respond to voltage select! : -110
	Couldn't find partition mmc 1:1
	Can't set block device
	Card did not respond to voltage select! : -110
	Couldn't find partition mmc 1:1
	Can't set block device
	Bad Linux ARM64 Image magic!
	=>


