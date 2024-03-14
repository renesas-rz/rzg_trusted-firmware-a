Renesas RZ/G3E
==============

The "RZ/G3E" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad/dual core Arm Cortex®-A55 and a M33 core, engineers can
easily implement industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/G3E reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board        |      Details                                                                                                                          |
+==============+===============+=======================================================================================================================+
| dev1         | Equipped with Renesas RZ/G3E SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>     /TODO: Update                               |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

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

Install Procedure
~~~~~~~~~~~~~~~~~

- Boot the board in Mini-monitor mode and enable access to the
  QSPI flash.


- Use the RZ/G3E flash_writer utility[2] to flash all the SREC files.

[2] https://github.com/renesas-rz/rzg2_flash_writer/tree/rz_G3E			//TODO: Update


Boot trace
----------
::
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

######
U-Boot starts up and begins to load the Linux kernel.
######

######
The kernel starts up and the login prompt is shown.
######
