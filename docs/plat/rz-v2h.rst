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
| smarc_pmic   | Equipped with Renesas RZ/V2H SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/rzV2H-evaluation-board-kit-rzV2H-evaluation-board-kit |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__

The current TF-A port has been tested on the SMARC PMIC RZ/V2H
SoC_id  rXXXXX revision ESx.y.


::

    ARM CA55 r2p0 (ARMv8-A) 1.8 GHz Quad Core, with NEON/VFP, L1$ I/D 32K, L2$ 0MB, L3$ 1024K
    ARM M33 200 MHz single core
    Memory controller for LPDDR4-3200 4GB in 2 channels(32-bit bus mode)
    Two- and three-dimensional graphics engines,
    Video processing units,
    Display Output,
    Video Input,
    SD card host interface,
    USB2.0 interfaces,
    USB3.0 interfaces,
    CAN interfaces,
    Ethernet AVB,
    Memories
        NTERNAL 3 x 512KB SYSTEM RAM
        DDR 4 GB LPDDR4
        QSPI FLASH 256MB
        EMMC 32 GB EMMC (HS400 240 MBYTES/S)
        MICROSD-CARD SLOT (SDR104 100 MBYTES/S)

Overview
--------
On RZ/V2H SoCs the BOOTROM starts the cpu at EL3; for this port BL2
will therefore be entered at this exception level (the Renesas' ATF
reference tree [1] resets into EL1 before entering BL2 - see its
bl2.ld.S)

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

[1] https://github.com/renesas-rz/meta-rzg2/tree/BSP-1.0.5/recipes-bsp/arm-trusted-firmware/files


How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the SMARC PMIC RZ/V2H development kit used in this port.

Build Tested:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=V2H all BOARD=smarc_pmic_2

System Tested:
~~~~~~~~~~~~~~
* u-boot:
  The port has beent tested using mainline uboot with HiHope RZ/G2M board
  specific patches.

|  commit 46ce9e777c1314ccb78906992b94001194eaa87b
|  Author: Heiko Schocher <hs@denx.de>
|  Date:   Tue Nov 3 15:22:36 2020 +0100

* linux:
  The port has beent tested using mainline kernel.

|  commit f8394f232b1eab649ce2df5c5f15b0e528c92091
|  Author: Linus Torvalds <torvalds@linux-foundation.org>
|  Date:   Sun Nov 8 16:10:16 2020 -0800
|  Linux 5.10-rc3

TF-A Build Procedure
~~~~~~~~~~~~~~~~~~~~
.. code:: bash

	sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib build-essential chrpath socat cpio python python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping libsdl1.2-dev xterm p7zip-full libyaml-dev
	cd rzg_bsp_v3.0.0/
	patch -p1 < ../v300-to-v300update1.patch
	cp meta-renesas/docs/template/conf/smarc-rzV2H/*.conf ./conf/
	source poky/oe-init-build-env
	bitbake core-image-minimal

.. code:: bash

       make PLAT=V2H all BOARD=smarc_pmic_2


Install Procedure
~~~~~~~~~~~~~~~~~

- Boot the board in Mini-monitor mode and enable access to the
  QSPI flash.


- Use the flash_writer utility[2] to flash all the SREC files.

[2] https://github.com/renesas-rz/rzg2_flash_writer/tree/rz_V2H


Boot trace
----------
::

	TBD

	
