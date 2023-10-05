Renesas RZ/V2H
==============

The "RZ/V2H" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 and in addation duo CR8 and a M33 core, engineers can
easily implement real-time control and
industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/V2H reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board        |      Details                                                                                                                          |
+==============+===============+=======================================================================================================================+
| dev1         | Equipped with Renesas RZ/V2H SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>     /TODO: Update                               |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__ //TODO: Update

The current TF-A port has been tested on the Dev1 RZ/V2H
SoC_id  r9a09g057h4 revision ESx.y.


::

    Arm® Cortex®-A55 1.8 GHz Quad Core, FPU, Neon™, L1-caches and L3-cache
    Arm® Cortex®-R8 800 MHz Dual Core, FPU, TCM, and L1-cache
    Arm® Cortex®-M33 200MHz, FPU and DSP-extension,
    DRP-AI, Mali™-G31 (GE3D), Mali™-C55 (ISP), 6 MB of on-chip SRAM, 
    2ch GbEthernet MAC, 
    USB2.0, USB3.2
    Gen 2x1, 4-MIPI® CSI-2® camera input interface, 
    1-MIPI® DSI® video output interface, 
    PCIe® Gen3 4Lane or 2-2Lane
    (EP/RC), various communication interfaces such as an xSPI, eMMC™, I2S (TDM), I3C®, PDM, and security functions.


Overview
--------
On RZ/V2H SoCs the BOOTROM starts the cpu at EL3; for this port BL2
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
the Dev_1 RZ/V2H development kit used in this port.

Build Tested:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=v2h all BOARD=dev_1 ENABLE_STACK_PROTECTOR=all

System Tested:
~~~~~~~~~~~~~~
* u-boot:
  The port has beent tested using mainline uboot with Dev_1 RZ/V2H board
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
	cp meta-renesas/docs/template/conf/dev1-rzV2H/*.conf ./conf/
	source poky/oe-init-build-env
	bitbake core-image-minimal

.. code:: bash

       make PLAT=v2h all BOARD=dev_1 ENABLE_STACK_PROTECTOR=all


Install Procedure
~~~~~~~~~~~~~~~~~

- Boot the board in Mini-monitor mode and enable access to the
  QSPI flash.


- Use the RZ/V2H flash_writer utility[2] to flash all the SREC files.

[2] https://github.com/renesas-rz/rzg2_flash_writer/tree/rz_V2H			//TODO: Update


Boot trace
----------
::
	NOTICE:  BL2: v2.7(debug):V2.7/RZV2H-1.00-BETA-83-gea81b2135
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
	NOTICE:  BL31: v2.7(debug):V2.7/RZV2H-1.00-BETA-82-g0af46693b-dirty
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


