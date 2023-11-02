Renesas RZ/G2L
==============

The "RZ/G2L" high-end 64-bit Arm®-based microprocessors (MPUs)
enables the solutions required for the smart society of the future.
Through a dual core of Arm Cortex®-A55 and in addation a M33 core, engineers can
easily implement real-time control and
industrial ethernet connectivity.

Plug-ins are available for multiple open-source software tools.


Renesas RZ/G2L reference platforms:
-----------------------------------

+--------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Board        |      Details                                                                                                                          |
+==============+===============+=======================================================================================================================+
| smarc_pmic   | Equipped with Renesas RZ/G2L SoC                                                                                                      |
|              +---------------------------------------------------------------------------------------------------------------------------------------+
|              | https://www.renesas.com/jp/en/products/microcontrollers-microprocessors/rz-mpus/rzg2l-evaluation-board-kit-rzg2l-evaluation-board-kit |
+--------------+---------------------------------------------------------------------------------------------------------------------------------------+

`boards info <https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-mpus/rzg-series#evaluation_boards>`__

The current TF-A port has been tested on the SMARC PMIC RZ/G2L
SoC_id  r9a07g044l2 revision ESx.y.


::

    ARM CA55 (ARMv8) 1.2 GHz dual core, with NEON/VFP, L1$ I/D 32K, L2$ 0MB, L3$ 256K
    ARM M33 200 MHz single core
    Memory controller for LPDDR4-3200 4GB in 2 channels(32-bit bus mode)
    Two- and three-dimensional graphics engines,
    Video processing units,
    Display Output,
    Video Input,
    SD card host interface,
    USB2.0 interfaces,
    CAN interfaces,
    Ethernet AVB,
    Memories
        INTERNAL 128KB SYSTEM RAM
        DDR 4 GB LPDDR4
        QSPI FLASH 64MB
        EMMC 32 GB EMMC (HS400 240 MBYTES/S)
        MICROSD-CARD SLOT (SDR104 100 MBYTES/S)

Overview
--------
On RZ/G2L SoCs the BOOTROM starts the cpu at EL3; for this port BL2
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
the SMARC PMIC RZ/G2L development kit used in this port.

Build Tested:
~~~~~~~~~~~~~

.. code:: bash

       make PLAT=g2l all BOARD=smarc_pmic_2

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
	cp meta-renesas/docs/template/conf/smarc-rzg2l/*.conf ./conf/
	source poky/oe-init-build-env
	bitbake core-image-minimal

.. code:: bash

       make PLAT=g2l all BOARD=smarc_pmic_2


Install Procedure
~~~~~~~~~~~~~~~~~

- Boot the board in Mini-monitor mode and enable access to the
  QSPI flash.


- Use the flash_writer utility[2] to flash all the SREC files.

[2] https://github.com/renesas-rz/rzg2_flash_writer/tree/rz_g2l


Boot trace
----------
::

	NOTICE:  BL2: v2.5(release):v2.5/rzg2l-1.00-1-gdca9ee0c2
	NOTICE:  BL2: Built : 04:57:21, Sep 17 2021
	NOTICE:  BL2: Booting BL31
	NOTICE:  BL31: v2.5(release):v2.5/rzg2l-1.00-1-gdca9ee0c2
	NOTICE:  BL31: Built : 04:57:21, Sep 17 2021


	U-Boot 2020.10 (Oct 22 2021 - 08:49:18 +0000)

	CPU: Renesas Electronics E rev 16.15
	Model: smarc-rzg2l
	DRAM:  1.9 GiB
	MMC:   sh-sdhi: 0, sh-sdhi: 1
	Loading Environment from MMC... *** Warning - bad CRC, using default environment

	In:    serial@1004b800
	Out:   serial@1004b800
	Err:   serial@1004b800
	Net:   
	Error: ethernet@11c20000 address not set.
	No ethernet found.

	Hit any key to stop autoboot:  2 <0x08><0x08><0x08> 1 <0x08><0x08><0x08> 0 
	## Resetting to default environment
	Card did not respond to voltage select!
	6334692 bytes read in 214 ms (28.2 MiB/s)
	27587 bytes read in 10 ms (2.6 MiB/s)
	Uncompressed size: 13672960 = 0xD0A200
	## Flattened Device Tree blob at 48000000
	   Booting using the fdt blob at 0x48000000
	   Loading Device Tree to 0000000057ff6000, end 0000000057fffbc2 ... OK

	Starting kernel ...

	[    0.000000] Booting Linux on physical CPU 0x0000000000 [0x412fd050]
	[    0.000000] Linux version 4.19.165-cip41-yocto-standard (oe-user@oe-host) (gcc version 8.3.0 (GCC)) #1 SMP PREEMPT Mon Mar 1 11:11:39 UTC 2021
	[    0.000000] Machine model: Evaluation board based on r9a07g044l2
	[    0.000000] earlycon: scif0 at MMIO 0x000000001004b800 (options '115200n8')
	[    0.000000] bootconsole [scif0] enabled
	[    0.000000] efi: Getting EFI parameters from FDT:
	[    0.000000] efi: UEFI not found.
	[    0.000000] Reserved memory: created CMA memory pool at 0x0000000058000000, size 256 MiB
	[    0.000000] OF: reserved mem: initialized node linux,cma@58000000, compatible id shared-dma-pool
	[    0.000000] Reserved memory: created CMA memory pool at 0x0000000068000000, size 128 MiB
	[    0.000000] OF: reserved mem: initialized node linux,multimedia, compatible id shared-dma-pool
	[    0.000000] psci: probing for conduit method from DT.
	[    0.000000] psci: PSCIv1.1 detected in firmware.
	[    0.000000] psci: Using standard PSCI v0.2 function IDs
	[    0.000000] psci: MIGRATE_INFO_TYPE not supported.
	[    0.000000] psci: SMC Calling Convention v1.0
	[    0.000000] random: get_random_bytes called from start_kernel+0x94/0x3f0 with crng_init=0
	[    0.000000] percpu: Embedded 22 pages/cpu s52376 r8192 d29544 u90112
	[    0.000000] Detected VIPT I-cache on CPU0
	[    0.000000] CPU features: detected: Hardware dirty bit management
	[    0.000000] CPU features: detected: Speculative Store Bypassing Safe (SSBS)
	[    0.000000] Built 1 zonelists, mobility grouping on.  Total pages: 483840
	[    0.000000] Kernel command line: rw rootwait earlycon root=/dev/mmcblk0p1
	[    0.000000] Dentry cache hash table entries: 262144 (order: 9, 2097152 bytes)
	[    0.000000] Inode-cache hash table entries: 131072 (order: 8, 1048576 bytes)
	[    0.000000] Memory: 1522704K/1966080K available (8828K kernel code, 616K rwdata, 3288K rodata, 576K init, 358K bss, 50160K reserved, 393216K cma-reserved)
	[    0.000000] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=2, Nodes=1
	[    0.000000] rcu: Preemptible hierarchical RCU implementation.
	[    0.000000] rcu: 	RCU restricting CPUs from NR_CPUS=8 to nr_cpu_ids=2.
	[    0.000000] 	Tasks RCU enabled.
	[    0.000000] rcu: Adjusting geometry for rcu_fanout_leaf=16, nr_cpu_ids=2
	[    0.000000] NR_IRQS: 64, nr_irqs: 64, preallocated irqs: 0
	[    0.000000] GICv3: Distributor has no Range Selector support
	[    0.000000] GICv3: no VLPI support, no direct LPI support
	[    0.000000] GICv3: CPU0: found redistributor 0 region 0:0x0000000011940000
	[    0.000000] arch_timer: cp15 timer(s) running at 24.00MHz (virt).
	[    0.000000] clocksource: arch_sys_counter: mask: 0xffffffffffffff max_cycles: 0x588fe9dc0, max_idle_ns: 440795202592 ns
	[    0.000004] sched_clock: 56 bits at 24MHz, resolution 41ns, wraps every 4398046511097ns
	[    0.008174] Console: colour dummy device 80x25
	[    0.012638] console [tty0] enabled
	[    0.016051] bootconsole [scif0] disabled
	[    0.000000] Booting Linux on physical CPU 0x0000000000 [0x412fd050]
	[    0.000000] Linux version 4.19.165-cip41-yocto-standard (oe-user@oe-host) (gcc version 8.3.0 (GCC)) #1 SMP PREEMPT Mon Mar 1 11:11:39 UTC 2021
	[    0.000000] Machine model: Evaluation board based on r9a07g044l2
	[    0.000000] earlycon: scif0 at MMIO 0x000000001004b800 (options '115200n8')
	[    0.000000] bootconsole [scif0] enabled
	[    0.000000] efi: Getting EFI parameters from FDT:
	[    0.000000] efi: UEFI not found.
	[    0.000000] Reserved memory: created CMA memory pool at 0x0000000058000000, size 256 MiB
	[    0.000000] OF: reserved mem: initialized node linux,cma@58000000, compatible id shared-dma-pool
	[    0.000000] Reserved memory: created CMA memory pool at 0x0000000068000000, size 128 MiB
	[    0.000000] OF: reserved mem: initialized node linux,multimedia, compatible id shared-dma-pool
	[    0.000000] psci: probing for conduit method from DT.
	[    0.000000] psci: PSCIv1.1 detected in firmware.
	[    0.000000] psci: Using standard PSCI v0.2 function IDs
	[    0.000000] psci: MIGRATE_INFO_TYPE not supported.
	[    0.000000] psci: SMC Calling Convention v1.0
	[    0.000000] random: get_random_bytes called from start_kernel+0x94/0x3f0 with crng_init=0
	[    0.000000] percpu: Embedded 22 pages/cpu s52376 r8192 d29544 u90112
	[    0.000000] Detected VIPT I-cache on CPU0
	[    0.000000] CPU features: detected: Hardware dirty bit management
	[    0.000000] CPU features: detected: Speculative Store Bypassing Safe (SSBS)
	[    0.000000] Built 1 zonelists, mobility grouping on.  Total pages: 483840
	[    0.000000] Kernel command line: rw rootwait earlycon root=/dev/mmcblk0p1
	[    0.000000] Dentry cache hash table entries: 262144 (order: 9, 2097152 bytes)
	[    0.000000] Inode-cache hash table entries: 131072 (order: 8, 1048576 bytes)
	[    0.000000] Memory: 1522704K/1966080K available (8828K kernel code, 616K rwdata, 3288K rodata, 576K init, 358K bss, 50160K reserved, 393216K cma-reserved)
	[    0.000000] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=2, Nodes=1
	[    0.000000] rcu: Preemptible hierarchical RCU implementation.
	[    0.000000] rcu: 	RCU restricting CPUs from NR_CPUS=8 to nr_cpu_ids=2.
	[    0.000000] 	Tasks RCU enabled.
	[    0.000000] rcu: Adjusting geometry for rcu_fanout_leaf=16, nr_cpu_ids=2
	[    0.000000] NR_IRQS: 64, nr_irqs: 64, preallocated irqs: 0
	[    0.000000] GICv3: Distributor has no Range Selector support
	[    0.000000] GICv3: no VLPI support, no direct LPI support
	[    0.000000] GICv3: CPU0: found redistributor 0 region 0:0x0000000011940000
	[    0.000000] arch_timer: cp15 timer(s) running at 24.00MHz (virt).
	[    0.000000] clocksource: arch_sys_counter: mask: 0xffffffffffffff max_cycles: 0x588fe9dc0, max_idle_ns: 440795202592 ns
	[    0.000004] sched_clock: 56 bits at 24MHz, resolution 41ns, wraps every 4398046511097ns
	[    0.008174] Console: colour dummy device 80x25
	[    0.012638] console [tty0] enabled
	[    0.016051] bootconsole [scif0] disabled
	[    0.020011] Calibrating delay loop (skipped), value calculated using timer frequency.. 48.00 BogoMIPS (lpj=96000)
	[    0.020029] pid_max: default: 32768 minimum: 301
	[    0.020135] Mount-cache hash table entries: 4096 (order: 3, 32768 bytes)
	[    0.020157] Mountpoint-cache hash table entries: 4096 (order: 3, 32768 bytes)
	[    0.021323] ASID allocator initialised with 32768 entries
	[    0.021412] rcu: Hierarchical SRCU implementation.
	[    0.021928] Detected Renesas RZ/G2L r9a07g044l2 ES5.7
	[    0.022206] EFI services will not be available.
	[    0.022391] smp: Bringing up secondary CPUs ...
	[    0.022773] Detected VIPT I-cache on CPU1
	[    0.022809] GICv3: CPU1: found redistributor 100 region 0:0x0000000011960000
	[    0.022848] CPU1: Booted secondary processor 0x0000000100 [0x412fd050]
	[    0.022948] smp: Brought up 1 node, 2 CPUs
	[    0.022983] SMP: Total of 2 processors activated.
	[    0.022995] CPU features: detected: GIC system register CPU interface
	[    0.023005] CPU features: detected: Privileged Access Never
	[    0.023014] CPU features: detected: User Access Override
	[    0.023023] CPU features: detected: 32-bit EL0 Support
	[    0.023389] CPU: All CPU(s) started at EL1
	[    0.023409] alternatives: patching kernel code
	[    0.025217] devtmpfs: initialized
	[    0.029350] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 7645041785100000 ns
	[    0.029395] futex hash table entries: 512 (order: 3, 32768 bytes)
	[    0.038373] pinctrl core: initialized pinctrl subsystem
	[    0.038944] DMI not present or invalid.
	[    0.039222] NET: Registered protocol family 16
	[    0.039539] audit: initializing netlink subsys (disabled)
	[    0.039732] audit: type=2000 audit(0.028:1): state=initialized audit_enabled=0 res=1
	[    0.040360] cpuidle: using governor menu
	[    0.040523] hw-breakpoint: found 6 breakpoint and 4 watchpoint registers.
	[    0.041400] DMA: preallocated 256 KiB pool for atomic allocations
	[    0.054681] HugeTLB registered 2.00 MiB page size, pre-allocated 0 pages
	[    0.055096] cryptd: max_cpu_qlen set to 1000
	[    0.064048] vgaarb: loaded
	[    0.064272] SCSI subsystem initialized
	[    0.064728] usbcore: registered new interface driver usbfs
	[    0.064773] usbcore: registered new interface driver hub
	[    0.064836] usbcore: registered new device driver usb
	[    0.065466] media: Linux media interface: v0.10
	[    0.065496] videodev: Linux video capture interface: v2.00
	[    0.065553] pps_core: LinuxPPS API ver. 1 registered
	[    0.065564] pps_core: Software ver. 5.3.6 - Copyright 2005-2007 Rodolfo Giometti <giometti@linux.it>
	[    0.065586] PTP clock support registered
	[    0.065615] EDAC MC: Ver: 3.0.0
	[    0.066351] rzg2l_mtu3 10001200.timer: ch0: used as clock source
	[    0.066374] clocksource: 10001200.timer: mask: 0xffff max_cycles: 0xffff, max_idle_ns: 18711154 ns
	[    0.066416] rzg2l_mtu3 10001200.timer: ch1: used for clock events
	[    0.066887] Advanced Linux Sound Architecture Driver Initialized.
	[    0.071076] GPIO line 458 (canfd0_en) hogged as output/high
	[    0.071118] GPIO line 459 (canfd1_en) hogged as output/high
	[    0.071146] GPIO line 153 (sd0_pwr_en) hogged as output/high
	[    0.071168] GPIO line 434 (sd1_pwr_en) hogged as output/high
	[    0.072068] rzg2l-pinctrl 11030000.pin-controller: rzg2l-pinctrl support registered
	[    0.072732] clocksource: Switched to clocksource arch_sys_counter
	[    0.072908] VFS: Disk quotas dquot_6.6.0
	[    0.072964] VFS: Dquot-cache hash table entries: 512 (order 0, 4096 bytes)
	[    0.077875] NET: Registered protocol family 2
	[    0.078429] tcp_listen_portaddr_hash hash table entries: 1024 (order: 2, 16384 bytes)
	[    0.078478] TCP established hash table entries: 16384 (order: 5, 131072 bytes)
	[    0.078582] TCP bind hash table entries: 16384 (order: 6, 262144 bytes)
	[    0.078863] TCP: Hash tables configured (established 16384 bind 16384)
	[    0.079013] UDP hash table entries: 1024 (order: 3, 32768 bytes)
	[    0.079055] UDP-Lite hash table entries: 1024 (order: 3, 32768 bytes)
	[    0.079211] NET: Registered protocol family 1
	[    0.079634] RPC: Registered named UNIX socket transport module.
	[    0.079652] RPC: Registered udp transport module.
	[    0.079662] RPC: Registered tcp transport module.
	[    0.079672] RPC: Registered tcp NFSv4.1 backchannel transport module.
	[    0.080300] kvm [1]: HYP mode not available
	[    0.081864] workingset: timestamp_bits=46 max_order=19 bucket_order=0
	[    0.086656] squashfs: version 4.0 (2009/01/31) Phillip Lougher
	[    0.087287] NFS: Registering the id_resolver key type
	[    0.087328] Key type id_resolver registered
	[    0.087338] Key type id_legacy registered
	[    0.087354] nfs4filelayout_init: NFSv4 File Layout Driver Registering...
	[    0.087380] jffs2: version 2.2. (NAND) <0xc2><0xa9> 2001-2006 Red Hat, Inc.
	[    0.090501] Block layer SCSI generic (bsg) driver version 0.4 loaded (major 245)
	[    0.090535] io scheduler noop registered
	[    0.090546] io scheduler deadline registered
	[    0.090711] io scheduler cfq registered (default)
	[    0.090726] io scheduler mq-deadline registered
	[    0.090736] io scheduler kyber registered
	[    0.092057] phy_rcar_gen3_usb2 11c50200.usb-phy: Linked as a consumer to regulator.3
	[    0.123631] SuperH (H)SCI(F) driver initialized
	[    0.124015] 1004b800.serial: ttySC0 at MMIO 0x1004b800 (irq = 42, base_baud = 0) is a scif
	[    0.858155] console [ttySC0] enabled
	[    0.862890] [drm] Supports vblank timestamp caching Rev 2 (21.10.2013).
	[    0.869515] [drm] No driver support for vblank timestamp query.
	[    0.883275] loop: module loaded
	[    0.889034] m25p80 spi3.0: unrecognized JEDEC id bytes: 00, 00, 00
	[    0.895241] m25p80: probe of spi3.0 failed with error -2
	[    0.901026] libphy: Fixed MDIO Bus: probed
	[    0.905408] tun: Universal TUN/TAP device driver, 1.6
	[    0.910644] CAN device driver interface
	[    0.916800] rcar_canfd 10050000.can: can_clk rate is 49875000
	[    0.922887] rcar_canfd 10050000.can: device registered (channel 0)
	[    0.929164] rcar_canfd 10050000.can: can_clk rate is 49875000
	[    0.935224] rcar_canfd 10050000.can: device registered (channel 1)
	[    0.941467] rcar_canfd 10050000.can: global operational state (clk 0, fdmode 1)
	[    0.948950] e1000e: Intel(R) PRO/1000 Network Driver - 3.2.6-k
	[    0.954764] e1000e: Copyright(c) 1999 - 2015 Intel Corporation.
	[    0.961481] ravb 11c20000.ethernet: no valid MAC address supplied, using a random one
	[    0.969450] libphy: ravb_mii: probed
	[    0.974212] ravb 11c20000.ethernet eth0: Base address at 0x11c20000, 7a:f1:2e:f4:12:be, IRQ 100.
	[    0.983748] ravb 11c30000.ethernet: no valid MAC address supplied, using a random one
	[    0.991731] libphy: ravb_mii: probed
	[    0.996460] ravb 11c30000.ethernet eth1: Base address at 0x11c30000, b2:70:5c:e3:20:53, IRQ 103.
	[    1.005708] VFIO - User Level meta-driver version: 0.3
	[    1.011043] ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
	[    1.017563] ehci-pci: EHCI PCI platform driver
	[    1.022056] ehci-platform: EHCI generic platform driver
	[    1.052819] ehci-platform 11c50100.usb: EHCI Host Controller
	[    1.058486] ehci-platform 11c50100.usb: new USB bus registered, assigned bus number 1
	[    1.066612] ehci-platform 11c50100.usb: irq 107, io mem 0x11c50100
	[    1.088733] ehci-platform 11c50100.usb: USB 2.0 started, EHCI 1.10
	[    1.095471] hub 1-0:1.0: USB hub found
	[    1.099246] hub 1-0:1.0: 1 port detected
	[    1.103705] ehci-platform 11c70100.usb: EHCI Host Controller
	[    1.109382] ehci-platform 11c70100.usb: new USB bus registered, assigned bus number 2
	[    1.117498] ehci-platform 11c70100.usb: irq 109, io mem 0x11c70100
	[    1.136737] ehci-platform 11c70100.usb: USB 2.0 started, EHCI 1.10
	[    1.143434] hub 2-0:1.0: USB hub found
	[    1.147207] hub 2-0:1.0: 1 port detected
	[    1.151538] ohci_hcd: USB 1.1 'Open' Host Controller (OHCI) Driver
	[    1.157734] ohci-pci: OHCI PCI platform driver
	[    1.162236] ohci-platform: OHCI generic platform driver
	[    1.167726] ohci-platform 11c50000.usb: Generic Platform OHCI controller
	[    1.174428] ohci-platform 11c50000.usb: new USB bus registered, assigned bus number 3
	[    1.182496] ohci-platform 11c50000.usb: irq 106, io mem 0x11c50000
	[    1.275275] hub 3-0:1.0: USB hub found
	[    1.279056] hub 3-0:1.0: 1 port detected
	[    1.283421] ohci-platform 11c70000.usb: Generic Platform OHCI controller
	[    1.290133] ohci-platform 11c70000.usb: new USB bus registered, assigned bus number 4
	[    1.298245] ohci-platform 11c70000.usb: irq 108, io mem 0x11c70000
	[    1.391285] hub 4-0:1.0: USB hub found
	[    1.395063] hub 4-0:1.0: 1 port detected
	[    1.399873] usbcore: registered new interface driver usb-storage
	[    1.406353] renesas_usbhs 11c60000.usb: host probed
	[    1.411240] renesas_usbhs 11c60000.usb: no transceiver found
	[    1.416992] renesas_usbhs 11c60000.usb: gadget probed
	[    1.422245] renesas_usbhs 11c60000.usb: probed
	[    1.427614] g_serial gadget: Gadget Serial v2.4
	[    1.432148] g_serial gadget: g_serial ready
	[    1.436461] usbcore: registered new interface driver usbtouchscreen
	[    1.442730] i2c /dev entries driver
	[    1.446990] i2c-riic 10058000.i2c: registered with 100000Hz bus speed
	[    1.454279] adv7511 1-003d: Linked as a consumer to regulator.1
	[    1.460350] adv7511 1-003d: Linked as a consumer to regulator.2
	[    1.473729] i2c-riic 10058400.i2c: registered with 100000Hz bus speed
	[    1.483288] random: fast init done
	[    1.487249] i2c-riic 10058c00.i2c: registered with 400000Hz bus speed
	[    1.494428] ov5645 0-003c: Linked as a consumer to regulator.4
	[    1.500310] ov5645 0-003c: Linked as a consumer to regulator.6
	[    1.506189] ov5645 0-003c: Linked as a consumer to regulator.5
	[    1.552861] ov5645 0-003c: ov5645_write_reg: write reg error -6: reg=3103, val=11
	[    1.560325] ov5645 0-003c: could not set init registers
	[    1.565540] ov5645 0-003c: could not power up OV5645
	[    1.570555] ov5645 0-003c: Dropping the link to regulator.5
	[    1.576174] ov5645 0-003c: Dropping the link to regulator.6
	[    1.581798] ov5645 0-003c: Dropping the link to regulator.4
	[    1.590445] rzg2l-csi2 10830400.csi2: 2 lanes found
	[    1.603735] renesas_sdhi_internal_dmac 11c00000.sd: Linked as a consumer to regulator.2
	[    1.611806] renesas_sdhi_internal_dmac 11c00000.sd: Linked as a consumer to regulator.1
	[    1.684909] renesas_sdhi_internal_dmac 11c00000.sd: mmc0 base at 0x11c00000 max clock rate 133 MHz
	[    1.695057] renesas_sdhi_internal_dmac 11c10000.sd: Linked as a consumer to regulator.2
	[    1.703355] renesas_sdhi_internal_dmac 11c10000.sd: Dropping the link to regulator.2
	[    1.711716] ledtrig-cpu: registered to indicate activity on CPUs
	[    1.718224] clocksource: ostm: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19160505731 ns
	[    1.727503] sched_clock: 32 bits at 99MHz, resolution 10ns, wraps every 21528657914ns
	[    1.735323] ostm: used for clocksource
	[    1.739314] ostm: used for clock events
	[    1.743734] usbcore: registered new interface driver usbhid
	[    1.749350] usbhid: USB HID core driver
	[    1.762742] ssi-pcm-audio 10049c00.ssi: DMA enalbed
	[    1.768152] ssi-pcm-audio 1004a000.ssi: DMA enalbed
	[    1.773491] NET: Registered protocol family 17
	[    1.778032] can: controller area network core (rev 20170425 abi 9)
	[    1.784440] NET: Registered protocol family 29
	[    1.788974] can: raw protocol (rev 20170425)
	[    1.793255] can: broadcast manager protocol (rev 20170425 t)
	[    1.798920] can: netlink gateway (rev 20170425) max_hops=1
	[    1.804617] Key type dns_resolver registered
	[    1.809508] registered taskstats version 1
	[    1.819977] thermal emergency: not set value
	[    1.825469] [drm] Supports vblank timestamp caching Rev 2 (21.10.2013).
	[    1.832168] [drm] No driver support for vblank timestamp query.
	[    1.840119] renesas_sdhi_internal_dmac 11c10000.sd: Linked as a consumer to regulator.2
	[    1.848265] renesas_sdhi_internal_dmac 11c10000.sd: Linked as a consumer to regulator.8
	[    1.882278] mmc0: new HS200 MMC card at address 0001
	[    1.887966] mmcblk0: mmc0:0001 G1J38E 59.3 GiB 
	[    1.892846] mmcblk0boot0: mmc0:0001 G1J38E partition 1 31.5 MiB
	[    1.899073] mmcblk0boot1: mmc0:0001 G1J38E partition 2 31.5 MiB
	[    1.905094] renesas_sdhi_internal_dmac 11c10000.sd: mmc1 base at 0x11c10000 max clock rate 133 MHz
	[    1.905212] mmcblk0rpmb: mmc0:0001 G1J38E partition 3 4.00 MiB, chardev (242:0)
	[    1.922412] asoc-simple-card sound: wm8978-hifi <-> 10049c00.ssi mapping ok
	[    1.929476] asoc-simple-card sound: ASoC: no DMI vendor name!
	[    1.945462]  mmcblk0: p1
	[    2.037880] asoc-audio-graph-card sound_card: i2s-hifi <-> 1004a000.ssi mapping ok
	[    2.045501] asoc-audio-graph-card sound_card: ASoC: no DMI vendor name!
	[    2.053070] [drm] Supports vblank timestamp caching Rev 2 (21.10.2013).
	[    2.059753] [drm] No driver support for vblank timestamp query.
	[    2.066585] [drm] Initialized rcar-du 1.0.0 20130110 for 10890000.display on minor 0
	[    2.074346] [drm] Device 10890000.display probed
	[    2.079770] [drm] Cannot find any crtc or sizes
	[    2.084799] hctosys: unable to open rtc device (rtc0)
	[    2.085193] [drm] Cannot find any crtc or sizes
	[    2.090197] ALSA device list:
	[    2.097374]   #0: ssi-dai-wm8978-hifi
	[    2.101040]   #1: sound_card
	[    2.163048] EXT4-fs (mmcblk0p1): recovery complete
	[    2.168404] EXT4-fs (mmcblk0p1): mounted filesystem with ordered data mode. Opts: (null)
	[    2.176567] VFS: Mounted root (ext4 filesystem) on device 179:1.
	[    2.203234] devtmpfs: mounted
	[    2.206632] Freeing unused kernel memory: 576K
	[    2.211170] Run /sbin/init as init process
	[    2.383719] systemd[1]: System time before build time, advancing clock.
	[    2.466948] systemd[1]: systemd 244.5+ running in system mode. (+PAM -AUDIT -SELINUX +IMA -APPARMOR -SMACK +SYSVINIT +UTMP -LIBCRYPTSETUP -GCRYPT -GNUTLS +ACL +XZ -LZ4 -SECCOMP +BLKID -ELFUTILS +KMOD -IDN2 -IDN -PCRE2 default-hierarchy=hybrid)
	[    2.489036] systemd[1]: Detected architecture arm64.

	Welcome to <0x1b>[1mPoky (Yocto Project Reference Distro) 3.1.5 (dunfell)<0x1b>[0m!

	[    2.555542] systemd[1]: Set hostname to <smarc-rzg2l>.
	[    4.046654] random: systemd: uninitialized urandom read (16 bytes read)
	[    4.053564] systemd[1]: system-getty.slice: unit configures an IP firewall, but the local system does not support BPF/cgroup firewalling.
	[    4.065933] systemd[1]: (This warning is only shown for the first unit using IP firewalling.)
	[    4.077145] systemd[1]: Created slice system-getty.slice.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Created slice <0x1b>[0;1;39msystem-getty.slice<0x1b>[0m.
	[    4.098947] random: systemd: uninitialized urandom read (16 bytes read)
	[    4.106892] systemd[1]: Created slice system-serial\x2dgetty.slice.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Created slice <0x1b>[0;1;39msystem-serial\x2dgetty.slice<0x1b>[0m.
	[    4.130945] random: systemd: uninitialized urandom read (16 bytes read)
	[    4.138815] systemd[1]: Created slice User and Session Slice.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Created slice <0x1b>[0;1;39mUser and Session Slice<0x1b>[0m.
	[    4.159329] systemd[1]: Started Dispatch Password Requests to Console Directory Watch.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mDispatch Password <0xe2><0x80><0xa6>ts to Console Directory Watch<0x1b>[0m.
	[    4.183317] systemd[1]: Started Forward Password Requests to Wall Directory Watch.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mForward Password R<0xe2><0x80><0xa6>uests to Wall Directory Watch<0x1b>[0m.
	[    4.207276] systemd[1]: Reached target Paths.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mPaths<0x1b>[0m.
	[    4.227230] systemd[1]: Reached target Remote File Systems.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mRemote File Systems<0x1b>[0m.
	[    4.247238] systemd[1]: Reached target Slices.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mSlices<0x1b>[0m.
	[    4.267320] systemd[1]: Reached target Swap.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mSwap<0x1b>[0m.
	[    4.309340] systemd[1]: Listening on Syslog Socket.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39mSyslog Socket<0x1b>[0m.
	[    4.327846] systemd[1]: Listening on initctl Compatibility Named Pipe.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39minitctl Compatibility Named Pipe<0x1b>[0m.
	[    4.352275] systemd[1]: Listening on Journal Audit Socket.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39mJournal Audit Socket<0x1b>[0m.
	[    4.372068] systemd[1]: Listening on Journal Socket (/dev/log).
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39mJournal Socket (/dev/log)<0x1b>[0m.
	[    4.396191] systemd[1]: Listening on Journal Socket.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39mJournal Socket<0x1b>[0m.
	[    4.416432] systemd[1]: Listening on Network Service Netlink Socket.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39mNetwork Service Netlink Socket<0x1b>[0m.
	[    4.440329] systemd[1]: Listening on udev Control Socket.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39mudev Control Socket<0x1b>[0m.
	[    4.460220] systemd[1]: Listening on udev Kernel Socket.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39mudev Kernel Socket<0x1b>[0m.
	[    4.484124] systemd[1]: Mounting Huge Pages File System...
		 Mounting <0x1b>[0;1;39mHuge Pages File System<0x1b>[0m...
	[    4.508388] systemd[1]: Mounting POSIX Message Queue File System...
		 Mounting <0x1b>[0;1;39mPOSIX Message Queue File System<0x1b>[0m...
	[    4.539124] systemd[1]: Mounting Kernel Debug File System...
		 Mounting <0x1b>[0;1;39mKernel Debug File System<0x1b>[0m...
	[    4.584455] systemd[1]: Mounting Temporary Directory (/tmp)...
		 Mounting <0x1b>[0;1;39mTemporary Directory (/tmp)<0x1b>[0m...
	[    4.604365] systemd[1]: Condition check resulted in Create list of static device nodes for the current kernel being skipped.
	[    4.640918] systemd[1]: Started Hardware RNG Entropy Gatherer Daemon.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mHardware RNG Entropy Gatherer Daemon<0x1b>[0m.
	[    4.664603] systemd[1]: Condition check resulted in File System Check on Root Device being skipped.
	[    4.679373] systemd[1]: Starting Journal Service...
		 Starting <0x1b>[0;1;39mJournal Service<0x1b>[0m...
	[    4.709649] random: crng init done
	[    4.713691] systemd[1]: Condition check resulted in Load Kernel Modules being skipped.
	[    4.721929] random: 7 urandom warning(s) missed due to ratelimiting
	[    4.728953] systemd[1]: Condition check resulted in FUSE Control File System being skipped.
	[    4.743273] systemd[1]: Mounting Kernel Configuration File System...
		 Mounting <0x1b>[0;1;39mKernel Configuration File System<0x1b>[0m...
	[    4.772662] systemd[1]: Starting Remount Root and Kernel File Systems...
		 Starting <0x1b>[0;1;39mRemount Root and Kernel File Systems<0x1b>[0m...
	[    4.801184] systemd[1]: Starting Apply Kernel Variables...
		 Starting <0x1b>[0;1;39mApply Kernel Variables<0x1b>[0m...
	[    4.825054] systemd[1]: Starting udev Coldplug all Devices...
		 Starting <0x1b>[0;1;39mudev Coldplug all Devices<0x1b>[0m...
	[    4.842122] EXT4-fs (mmcblk0p1): re-mounted. Opts: (null)
	[    4.867359] systemd[1]: Started Journal Service.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mJournal Service<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Mounted <0x1b>[0;1;39mHuge Pages File System<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Mounted <0x1b>[0;1;39mPOSIX Message Queue File System<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Mounted <0x1b>[0;1;39mKernel Debug File System<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Mounted <0x1b>[0;1;39mTemporary Directory (/tmp)<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Mounted <0x1b>[0;1;39mKernel Configuration File System<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mRemount Root and Kernel File Systems<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mApply Kernel Variables<0x1b>[0m.
		 Starting <0x1b>[0;1;39mFlush Journal to Persistent Storage<0x1b>[0m...
		 Starting <0x1b>[0;1;39mCreate Static Device Nodes in /dev<0x1b>[0m...
	[    5.084590] systemd-journald[122]: Received client request to flush runtime journal.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mFlush Journal to Persistent Storage<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mCreate Static Device Nodes in /dev<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mLocal File Systems (Pre)<0x1b>[0m.
		 Mounting <0x1b>[0;1;39m/var/volatile<0x1b>[0m...
		 Starting <0x1b>[0;1;39mudev Kernel Device Manager<0x1b>[0m...
	[<0x1b>[0;32m  OK  <0x1b>[0m] Mounted <0x1b>[0;1;39m/var/volatile<0x1b>[0m.
		 Starting <0x1b>[0;1;39mLoad/Save Random Seed<0x1b>[0m...
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mLocal File Systems<0x1b>[0m.
		 Starting <0x1b>[0;1;39mCreate Volatile Files and Directories<0x1b>[0m...
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mLoad/Save Random Seed<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mCreate Volatile Files and Directories<0x1b>[0m.
		 Starting <0x1b>[0;1;39mNetwork Time Synchronization<0x1b>[0m...
		 Starting <0x1b>[0;1;39mUpdate UTMP about System Boot/Shutdown<0x1b>[0m...
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mudev Coldplug all Devices<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mUpdate UTMP about System Boot/Shutdown<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mudev Kernel Device Manager<0x1b>[0m.
		 Starting <0x1b>[0;1;39mNetwork Service<0x1b>[0m...
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mNetwork Time Synchronization<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mSystem Initialization<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mDaily Cleanup of Temporary Directories<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mSystem Time Set<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mSystem Time Synchronized<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mTimers<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39mD-Bus System Message Bus Socket<0x1b>[0m.
		 Starting <0x1b>[0;1;39msshd.socket<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mNetwork Service<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Listening on <0x1b>[0;1;39msshd.socket<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mSockets<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mBasic System<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mKernel Logging Service<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mSystem Logging Service<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mD-Bus System Message Bus<0x1b>[0m.
		 Starting <0x1b>[0;1;39mrng-tools.service<0x1b>[0m...
		 Starting <0x1b>[0;1;39mLogin Service<0x1b>[0m...
		 Starting <0x1b>[0;1;39mNetwork Name Resolution<0x1b>[0m...
	[    6.354800] Microchip KSZ9131 Gigabit PHY 11c30000.ethernet-ffffffff:07: attached PHY driver [Microchip KSZ9131 Gigabit PHY] (mii_bus:phy_addr=11c30000.ethernet-ffffffff:07, irq=142)
	[    6.521585] Microchip KSZ9131 Gigabit PHY 11c20000.ethernet-ffffffff:07: attached PHY driver [Microchip KSZ9131 Gigabit PHY] (mii_bus:phy_addr=11c20000.ethernet-ffffffff:07, irq=141)
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mrng-tools.service<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mHardware activated USB gadget<0x1b>[0m.
		 Starting <0x1b>[0;1;39mSave/Restore Sound Card State<0x1b>[0m...
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mNetwork Name Resolution<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mNetwork<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mHost and Network Name Lookups<0x1b>[0m.
		 Starting <0x1b>[0;1;39mPermit User Sessions<0x1b>[0m...
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mPermit User Sessions<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mGetty on tty1<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mSerial Getty on ttySC0<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mLogin Prompts<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mLogin Service<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mSave/Restore Sound Card State<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mMulti-User System<0x1b>[0m.
	[<0x1b>[0;32m  OK  <0x1b>[0m] Reached target <0x1b>[0;1;39mSound Card<0x1b>[0m.
		 Starting <0x1b>[0;1;39mUpdate UTMP about System Runlevel Changes<0x1b>[0m...
	[<0x1b>[0;32m  OK  <0x1b>[0m] Started <0x1b>[0;1;39mUpdate UTMP about System Runlevel Changes<0x1b>[0m.

	Poky (Yocto Project Reference Distro) 3.1.5 smarc-rzg2l ttySC0

	BSP: RZG2L/RZG2L-SMARC-EVK/1.3
	LSI: RZG2L
	Version: 1.3
	smarc-rzg2l login: root

