Renesas RZ/T2H
==============

The "RZ/T2H" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 and in addation duo CR8 and a M33 core, engineers can
easily implement real-time control and
industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/T2H reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board        |      Details                                                                                                                          |
+==============+===============+=======================================================================================================================+
| dev1         | Equipped with Renesas RZ/T2H SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>     /TODO: Update                               |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__ //TODO: Update

The current TF-A port has been tested on the Dev1 RZ/T2H
SoC_id  r9a09g077m48gbg (Quad A55), r9a09g077m28gbg (Dual A55) or r9a09g077m08gbg (Single A55) revision ESx.y.

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
On RZ/T2H SoCs the BOOTROM starts the cpu at EL3; for this port BL2
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
the Dev_1 RZ/T2H development kit used in this port.

Build Tested:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=t2h all BOARD=dev_1 ENABLE_STACK_PROTECTOR=all

System Tested:
~~~~~~~~~~~~~~
* u-boot:
  The port has beent tested using mainline uboot with Dev_1 RZ/T2H board
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
	cp meta-renesas/docs/template/conf/dev1-rzT2H/*.conf ./conf/
	source poky/oe-init-build-env
	bitbake core-image-minimal

.. code:: bash

       make PLAT=t2h all BOARD=dev_1 ENABLE_STACK_PROTECTOR=all


Install Procedure
~~~~~~~~~~~~~~~~~

- Boot the board in Mini-monitor mode and enable access to the
  QSPI flash.


- Use the RZ/T2H flash_writer utility[2] to flash all the SREC files.

[2] https://github.com/renesas-rz/rzg2_flash_writer/tree/rz_T2H			//TODO: Update


Boot trace
----------
::
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


