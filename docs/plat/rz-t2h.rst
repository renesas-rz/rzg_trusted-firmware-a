Renesas RZ/T2H
==============

The "RZ/T2H" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 with the addition two R52 cores, engineers can
easily implement real-time control solutions.

Plug-ins are available for multiple open-source software tools.


-----------------------------------
Renesas RZ/T2H reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board        |      Details                                                                                                                          |
+==============+===============+=======================================================================================================================+
| dev1         | Equipped with Renesas RZ/T2H SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzt-series-mpu                                        |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

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


--------
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


------------
How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the Dev_1 RZ/T2H development kit used in this port.

Base build instruction:
~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

       make PLAT=t2h all BOARD=dev_1

Build Options:
~~~~~~~~~~~~~

PLATFORM_CORE_COUNT=4
This is used to specify to build for the quad, dual or single core SoC.
PLATFORM_CORE_COUNT=4 --Quad core
PLATFORM_CORE_COUNT=2 --Dual core
PLATFORM_CORE_COUNT=1 --Single core

DEBUG=1 LOG_LEVEL=20
If a debug build with logging is required, then use these two build options.
LOG_LEVEL = 0 = LOG_LEVEL_NONE
LOG_LEVEL = 10 = LOG_LEVEL_ERROR
LOG_LEVEL = 20 = LOG_LEVEL_NOTICE
LOG_LEVEL = 30 = LOG_LEVEL_WARNING
LOG_LEVEL = 40 = LOG_LEVEL_INFO
LOG_LEVEL = 50 = LOG_LEVEL_VERBOSE

System Tested:
~~~~~~~~~~~~~~

* u-boot:
  The port has been tested using mainline uboot with Dev_1 RZ/T2H board
  specific patches.
  <URL TBD>

TF-A Build Procedure
~~~~~~~~~~~~~~~~~~~~

.. code:: bash
		cd <tfa project path>
		export CROSS_COMPILE=<path to installed toolset>/bin/aarch64-elf-

		make PLAT=t2h all BOARD=dev_1 PLATFORM_CORE_COUNT=4 DEBUG=1 LOG_LEVEL=40

		make PLAT=t2h all BOARD=dev_1 PLATFORM_CORE_COUNT=4 DEBUG=1 fip BL33=<path to u boot bin file>/u-boot.bin

		python3 tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=param_output

		make PLAT=t2h pkg BOARD=dev_1 DEBUG=1


----------------
How to load TF-A
----------------

Loading the flash writer
~~~~~~~~~~~~~~~~~~~~~~~~

1.	Set the device in SCIF mode,
2.	Connect to the COM port provided by the device via some terminal software.
3.	Set the baudrate to be 115200
4.	Set the transmit delay to be 0msec/char and 1msec/line
5.	Hit reset and the device will print a message.
6.	Send the FlashWriter mot file[1].

[1] https://github.com/renesas-rz/<TBD>

Flash Procedure for xSPI
~~~~~~~~~~~~~~~~~~~~~~~~

1.	Use the ‘Loading the flash writer' procedure.
2.	Modify the XSPIW parameter using this command: XSPIW 0 0x0 0x3c600
3.	Set the transmit delay to be 0 msec/char and 0msec/line
4.	Send the BL2 image srec file
5.	Modify the XSPIW parameter using this command: XSPIW 0 0x1FB000 0xAB900
6.	Set the transmit delay to be 0 msec/char and 1msec/line
4.	Send the FIP image srec file


----------
Boot trace
----------

::
	NOTICE:  BL2: v2.7(release):v2.5/rzg2l-1.00-2294-gff6037e88
	NOTICE:  BL2: Built : 09:42:43, Dec  5 2023
	ERROR:   Reported Core count (0) is different from PLATFORM_CORE_COUNT (4)
	NOTICE:  BL2: Booting BL31
	NOTICE:  BL31: v2.7(release):v2.5/rzg2l-1.00-2294-gff6037e88
	NOTICE:  BL31: Built : 09:42:47, Dec  5 2023


	U-Boot 2021.10-g6874bf85f1 (Nov 30 2023 - 11:20:11 +0000)

	CPU: Renesas Electronics RZ/T2H
	Model: Renesas Development EVK based on r9a09g077m44
	DRAM:  7.9 GiB
	<More U-Boot specific trace>
	
	<Boot Trace of next stage OS such as Linux, RTOS or others>


NOTE: For "Reported Core Count (n) is different from PLATFORM_CORE_COUNT", n should be 1, 2, or 4, dependent on the CPU variant. 


