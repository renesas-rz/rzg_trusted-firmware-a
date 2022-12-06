Renesas RZ/G3S
==============

The "RZ/G3S" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a quad core of Arm Cortex®-A55 and in addation duo CR8 and a M33 core, engineers can
easily implement real-time control and
industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/G3S reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board             |      Details                                                                                                                     |
+==============+===============+=======================================================================================================================+
| smarc_evk_1       | Equipped with Renesas RZ/G3S SoC                                                                                                 |
| (g3s_smarc_evk_1) | +--------------------------------------------------------------------------------------------------------------------------------+
|                   | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/<TBD>                                            |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__<TBD>

The current TF-A port has been tested on the SMARC Evaluation board for RZ/G3S
SoC_id  rXXXXX revision ESx.y.


::

    ARM CA55 r2p0 (ARMv8-A) 1.2 GHz Single Core, with NEON/VFP, L1$ I/D 32K, L2$ 0MB, L3$ 256K
    ARM M33 240 MHz Dual core
    Memory controller for LPDDR4-3200 4GB in 2 channels(32-bit bus mode)
    SD card host interface,
    USB2.0 interfaces,
    CAN interfaces,
    Ethernet AVB,
    Memories
        INTERNAL 3 x 256KB SYSTEM RAM
        DDR 4 GB LPDDR4
        QSPI FLASH 256MB  <TBC>
        EMMC 32 GB EMMC (HS400 240 MBYTES/S) <TBC>
        MICROSD-CARD SLOT (SDR104 100 MBYTES/S) <TBC>

Overview
--------
On RZ/G3S SoCs the BOOTROM starts the cpu at EL3; for this port BL2
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

[1] https://github.com/renesas-rz/meta-rzg3l/tree/BSP-1.0.5/recipes-bsp/arm-trusted-firmware/files<TBD>


How to build
------------

The TF-A build options depend on the target board so you will have to
refer to those specific instructions. What follows is customized to
the SMARC Evaluation RZ/G3S development kit used in this port.

Build Tested:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=g3s all BOARD=smarc_evk_1 ENABLE_STACK_PROTECTOR=default DEBUG=1 DEBUG_FPGA=1 LOG_LEVEL=40
       

System Tested:
~~~~~~~~~~~~~~
* u-boot:
  The port has beent tested using mainline uboot with <TBD>

TF-A Build Procedure
~~~~~~~~~~~~~~~~~~~~
.. code:: bash

	<TBD>

.. code:: bash

       make PLAT=g3s all BOARD=smarc_evk_1 ENABLE_STACK_PROTECTOR=default DEBUG=1 DEBUG_FPGA=1 LOG_LEVEL=40


Install Procedure
~~~~~~~~~~~~~~~~~

- Boot the board in Mini-monitor mode and enable access to the
  QSPI flash.


- Use the flash_writer utility[2] to flash all the SREC files.

[2] https://github.com/renesas-rz/rzg3s_flash_writer/tree/rz_g3s


Boot trace
----------
::

	TBD

	
