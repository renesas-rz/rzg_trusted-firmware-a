Renesas RZ/V2H
==============

The "RZ/V2H" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 and in addition duo CR8 and a M33 core, engineers can
easily implement real-time control and
industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/V2H reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board        |      Details                                                                                                                          |
+==============+===============+=======================================================================================================================+
| dev_1        | Equipped with Renesas RZ/V2H SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzt-series-mpu                                        |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| evk_apha     | Equipped with Renesas RZ/V2H SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>     /TODO: Update                               |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

The current TF-A port has been tested on the Development and Evaluation Alpha RZ/V2H platforms.
SoC_id r9a09g057h4 revision ESx.y.

+----------------+------------------+------------------+
|  Part Number   |  GE3D            |  Security        |
+================+==================+==================+
| R9A09G057H41GBG|  N/A             |  N/A             |
+                +                  +                  +
| R9A09G057H42GBG|  GE3D            |  (Mali-G31)      |
+                +                  +                  +
| R9A09G057H45GBG|  N/A             |  Available       |
+                +                  +                  +
| R9A09G057H46GBG|  GE3D (Mali-G31) |                  |
+----------------+------------------+------------------+

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


--------
Overview
--------
On RZ/V2H SoCs the BOOTROM starts the CPU at EL3; for this port BL2
will therefore be entered at this exception level.

EL1 is entered before exiting BL31.

BL2 initializes DDR before determining the boot reason (cold or warm).

Once BL2 boots, it determines the boot reason, writes it to shared
memory (BOOT_KIND_BASE) together with the BL31 parameters
(PARAMS_BASE) and jumps to BL31.

To all effects, BL31 is as if it is being entered in reset mode since
it still needs to initialize the rest of the cores, this is the reason
behind using direct shared memory access to BOOT_KIND_BASE _and_
PARAMS_BASE instead of using registers to get to those locations (see
el3_common_macros.S and bl31_entrypoint.S for the RESET_TO_BL31 use
case).


------------
How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. 

What follows is customized to
the Development and Evaluation Alpha RZ/V2H development kit used in these ports.

Base build instruction:
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

	make PLAT=v2h all BOARD=evk_alpha ENABLE_STACK_PROTECTOR=all

Build Options:
~~~~~~~~~~~~~~
.. code:: bash

    DEBUG=1 LOG_LEVEL=20

If a debug build with logging is required, then use these two build options.

.. code:: bash

	LOG_LEVEL = 0 = LOG_LEVEL_NONE
	LOG_LEVEL = 10 = LOG_LEVEL_ERROR
	LOG_LEVEL = 20 = LOG_LEVEL_NOTICE
	LOG_LEVEL = 30 = LOG_LEVEL_WARNING
	LOG_LEVEL = 40 = LOG_LEVEL_INFO
	LOG_LEVEL = 50 = LOG_LEVEL_VERBOSE

System Tested:
~~~~~~~~~~~~~~

* u-boot:
  The port has been tested using mainline uboot with the appropriate dev_1 or evk_alpha RZ/V2H board specific patches.
  <URL TBD>


TF-A Build Procedure
~~~~~~~~~~~~~~~~~~~~

.. code:: bash

	cd <tfa project path>/tf-a
	export CROSS_COMPILE=<path to installed toolset>/bin/aarch64-elf-
	make PLAT=v2h all BOARD=dev_1 ENABLE_STACK_PROTECTOR=all LOG_LEVEL=40
	make PLAT=v2h all BOARD=dev_1 ENABLE_STACK_PROTECTOR=all DEBUG=1 LOG_LEVEL=40
	make PLAT=v2h all BOARD=evk_alpha ENABLE_STACK_PROTECTOR=all DEBUG=1 LOG_LEVEL=40
	make PLAT=v2h all BOARD=evk_alpha ENABLE_STACK_PROTECTOR=default DEBUG=1 PLAT_SYSTEM_SUSPEND=1
	make PLAT=v2h all BOARD=evk_alpha ENABLE_STACK_PROTECTOR=default DEBUG=1 PLAT_SYSTEM_SUSPEND=1 BL33=u-boot.bin bptool fip pkg


----------------
How to load TF-A
----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~

1.	Set the device in SCIF mode,
2.	Connect to the COM port provided by the device via some terminal \ software.
3.	Set the baudrate to be 115200
4.	Set the transmit delay to be 0msec/char and 1msec/line
5.	Hit reset and the device will print a message.
6.	Send the FlashWriter .mot file[1].

[1] https://github.com/renesas-rz/<TBD>


Flash Procedure for xSPI
~~~~~~~~~~~~~~~~~~~~~~~~

1.	Use the ‘Loading the flash writer' procedure.
2.	Modify the XSPIW parameter using this command: XSPIW 0 0x0 0x3c600
3.	Set the transmit delay to be 0 msec/char and 0msec/line
4.	Send the BL2 image srec file
5.	Modify the XSPIW parameter using this command: XSPIW 0 0x1FB000 0xAB900
6.	Set the transmit delay to be 0 msec/char and 1msec/line
7.	Send the FIP image srec file

----------
Boot trace
----------

::

	NOTICE:  BL2: v2.7(release):v2.7/rzv2h-1.00-2312-ga552b0f46
	NOTICE:  BL2: Built : 12:43:11, Dec 17 2023
	NOTICE:  BL2: Booting BL31
	NOTICE:  BL31: v2.7(release):v2.7/rzv2h-1.00-2312-ga552b0f46
	NOTICE:  BL31: Built : 12:43:16, Dec 17 2023

	U-Boot 2021.10-geba1f3bfcb (Dec 14 2023 - 16:52:50 +0000)

	CPU:   Renesas Electronics CPU rev 9.0
	Model: Renesas EVK Alpha based on r9a09g057h4
	DRAM:  15.9 GiB
	<More U-Boot specific trace>

	<Boot Trace of next stage OS such as Linux, RTOS or others>
