/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <common/debug.h>

#include "ddr_private.h"

#define DDR_LOG_EN		(0)

static void decode_streaming_message(uint8_t sel_train);
static void decode_streaming_message_ddr4(uint32_t codede_message_hex, uint16_t *args_list);
static void decode_streaming_message_ddr4_2d(uint32_t codede_message_hex, uint16_t *args_list);

void decode_major_message(uint32_t mail, uint8_t sel_train)
{
#if (DDR_LOG_EN && DEBUG)
	switch (mail) {
	case 0x00:
		INFO("PMU Major Msg: End of initialization\n");
		break;
	case 0x01:
		INFO("PMU Major Msg: End of fine write leveling\n");
		break;
	case 0x02:
		INFO("PMU Major Msg: End of read enable training\n");
		break;
	case 0x03:
		INFO("PMU Major Msg: End of read delay center optimization\n");
		break;
	case 0x04:
		INFO("PMU Major Msg: End of write delay center optimization\n");
		break;
	case 0x05:
		INFO("PMU Major Msg: End of 2D read delay/voltage center optimization\n");
		break;
	case 0x06:
		INFO("PMU Major Msg: End of 2D write delay /voltage center optimization\n");
		break;
	case 0x07:
		INFO("PMU Major Msg: Training run has completed\n");
		break;
	case 0x08:
		INFO("PMU Major Msg: Enter streaming message mode\n");
		break;
	case 0x09:
		INFO("PMU Major Msg: End of max read latency training\n");
		break;
	case 0x0a:
		INFO("PMU Major Msg: End of read dq deskew training\n");
		break;
	case 0x0b:
		INFO("PMU Major Msg: End of LCDL offset calibration\n");
		break;
	case 0x0c:
		INFO("PMU Major Msg: End of LRDIMM Specific training (DWL, MREP, MRD and MWD)\n");
		break;
	case 0x0d:
		INFO("PMU Major Msg: End of CA training\n");
		break;
	case 0xfd:
		INFO("PMU Major Msg: End of MPR read delay center optimization\n");
		break;
	case 0xfe:
		INFO("PMU Major Msg: End of Write leveling coarse delay\n");
		break;
	case 0xff:
		INFO("PMU Major Msg: FATAL ERROR.\n");
		break;
	default:
		INFO("PMU Major Msg: Un-recognized message... !\n");
	}
#endif
	if (mail == 0x08)
		decode_streaming_message(sel_train);
}

static void decode_streaming_message(uint8_t sel_train)
{
	int i;
	uint32_t codede_message_hex;
	uint16_t num_args;
	uint16_t args_list[31];

	codede_message_hex = get_mail(1);

	num_args = codede_message_hex & 0xFFFF;

	for (i = 0; i < num_args; i++)
		args_list[i] = get_mail(1);

	switch (sel_train) {
	case 1:
		decode_streaming_message_ddr4(codede_message_hex, args_list);
		break;
	case 2:
		decode_streaming_message_ddr4_2d(codede_message_hex, args_list);
		break;
	}
}

static void decode_streaming_message_ddr4(uint32_t codede_message_hex, uint16_t *args_list)
{
#if (DDR_LOG_EN && DEBUG)
	switch (codede_message_hex) {
	case 0x00000001:
		INFO("PMU1:prbsGenCtl:%x\n", args_list[0]);
		break;
	case 0x00010000:
		INFO("PMU1: loading 2D acsm sequence\n");
		break;
	case 0x00020000:
		INFO("PMU1: loading 1D acsm sequence\n");
		break;
	case 0x00030002:
		INFO("PMU3: %d memclocks @ %d to get half of 300ns\n", args_list[0], args_list[1]);
		break;
	case 0x00040000:
		INFO("PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n");
		break;
	case 0x00050000:
		INFO("PMU3: Running 1D search for left eye edge\n");
		break;
	case 0x00060001:
		INFO("PMU1: In Phase Left Edge Search cs %d\n", args_list[0]);
		break;
	case 0x00070001:
		INFO("PMU1: Out of Phase Left Edge Search cs %d\n", args_list[0]);
		break;
	case 0x00080000:
		INFO("PMU3: Running 1D search for right eye edge\n");
		break;
	case 0x00090001:
		INFO("PMU1: In Phase Right Edge Search cs %d\n", args_list[0]);
		break;
	case 0x000a0001:
		INFO("PMU1: Out of Phase Right Edge Search cs %d\n", args_list[0]);
		break;
	case 0x000b0001:
		INFO("PMU1: mxRdLat training pstate %d\n", args_list[0]);
		break;
	case 0x000c0001:
		INFO("PMU1: mxRdLat search for cs %d\n", args_list[0]);
		break;
	case 0x000d0001:
		INFO("PMU0: MaxRdLat non consistant DtsmLoThldXingInd 0x%x\n", args_list[0]);
		break;
	case 0x000e0003:
		INFO("PMU4: CS %d Dbyte %d worked with DFIMRL = %d DFICLKs\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x000f0004:
		INFO("PMU3: MaxRdLat Read Lane err mask for csn %d, DFIMRL %d DFIClks, dbyte %d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00100003:
		INFO("PMU3: MaxRdLat Read Lane err mask for csn %d DFIMRL %d, All dbytes = 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00110001:
		INFO("PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n", args_list[0]);
		break;
	case 0x00120002:
		INFO("PMU3: Smallest passing DFIMRL for all dbytes in CS%d = %d DFIClks\n", args_list[0], args_list[1]);
		break;
	case 0x00130000:
		INFO("PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n");
		break;
	case 0x00140003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00150006:
		INFO("PMU4: Adjusting rxclkdly db %d nib %d from %d+%d=%d->%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00160000:
		INFO("PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x00170005:
		INFO("PMU4: DB %d Lane %d: %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00180002:
		INFO("PMU2: TXDQ delayLeft[%d] = %d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00190004:
		INFO("PMU2: TXDQ delayLeft[%d] = %d oopScaled = %d selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x001a0002:
		INFO("PMU2: TXDQ delayRight[%d] = %d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x001b0004:
		INFO("PMU2: TXDQ delayRight[%d] = %d oopScaled = %d selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x001c0003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x001d0000:
		INFO("PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x001e0002:
		INFO("PMU4: DB %d Lane %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x001f0005:
		INFO("PMU4: DB %d Lane %d: %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00200002:
		INFO("PMU3: Running 1D search csn %d for DM Right/NotLeft(%d) eye edge\n", args_list[0], args_list[1]);
		break;
	case 0x00210002:
		INFO("PMU3: WrDq DM byte%d with Errcnt %d\n", args_list[0], args_list[1]);
		break;
	case 0x00220002:
		INFO("PMU3: WrDq DM byte%d avgDly 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00230002:
		INFO("PMU1: WrDq DM byte%d with Errcnt %d\n", args_list[0], args_list[1]);
		break;
	case 0x00240001:
		INFO("PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n", args_list[0]);
		break;
	case 0x00250000:
		INFO("PMU4: DM TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x00260002:
		INFO("PMU4: DB %d Lane %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00270005:
		INFO("PMU4: DB %d Lane %d: %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00280003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00290004:
		INFO("PMU3: Errcnt for MRD/MWD search nib %d delay = (%d, 0x%x) = %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x002a0000:
		INFO("PMU3: Precharge all open banks\n");
		break;
	case 0x002b0002:
		INFO("PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n", args_list[0], args_list[1]);
		break;
	case 0x002c0000:
		INFO("PMU4: MRD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n");
		break;
	case 0x002d0000:
		INFO("PMU4: MWD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n");
		break;
	case 0x002e0004:
		INFO("PMU10: Warning: DB %d nibble %d has multiple working coarse positions, %d and %d, choosing the smaller delay\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x002f0003:
		INFO("PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00300006:
		INFO("PMU4: DB %d nibble %d: %d, %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00310002:
		INFO("PMU1: Start MRD/nMWD %d for csn %d\n", args_list[0], args_list[1]);
		break;
	case 0x00320002:
		INFO("PMU2: RXDQS delayLeft[%d] = %d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00330006:
		INFO("PMU2: RXDQS delayLeft[%d] = %d delayOop[%d] = %d OopScaled %d, selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00340002:
		INFO("PMU2: RXDQS delayRight[%d] = %d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00350006:
		INFO("PMU2: RXDQS delayRight[%d] = %d delayOop[%d] = %d OopScaled %d, selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00360000:
		INFO("PMU4: RxClkDly Passing Regions (EyeLeft EyeRight -> EyeCenter)\n");
		break;
	case 0x00370002:
		INFO("PMU4: DB %d nibble %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00380005:
		INFO("PMU4: DB %d nibble %d: %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00390003:
		INFO("PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x003a0002:
		INFO("PMU0: goodbar = %d for RDWR_BLEN %d\n", args_list[0], args_list[1]);
		break;
	case 0x003b0001:
		INFO("PMU3: RxClkDly = %d\n", args_list[0]);
		break;
	case 0x003c0005:
		INFO("PMU0: db %d l %d absLane %d -> bottom %d top %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x003d0009:
		INFO("PMU3: BYTE %d - %d %d %d %d %d %d %d %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x003e0002:
		INFO("PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n", args_list[0], args_list[1]);
		break;
	case 0x003f0004:
		INFO("PMU0: db%d l%d - %d %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00400002:
		INFO("PMU0: goodbar = %d for RDWR_BLEN %d\n", args_list[0], args_list[1]);
		break;
	case 0x00410004:
		INFO("PMU3: db%d l%d saw %d issues at rxClkDly %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00420003:
		INFO("PMU3: db%d l%d first saw a pass->fail edge at rxClkDly %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00430002:
		INFO("PMU3: lane %d PBD = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00440003:
		INFO("PMU3: db%d l%d first saw a DBI pass->fail edge at rxClkDly %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00450003:
		INFO("PMU2: db%d l%d already passed rxPBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00460003:
		INFO("PMU0: db%d l%d, PBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00470002:
		INFO("PMU: Error: dbyte %d lane %d failed read deskew\n", args_list[0], args_list[1]);
		break;
	case 0x00480003:
		INFO("PMU0: db%d l%d, inc PBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00490003:
		INFO("PMU1: Running lane deskew on pstate %d csn %d rdDBIEn %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x004a0000:
		INFO("PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n");
		break;
	case 0x004b0002:
		INFO("PMU1: AcsmCsMapCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x004c0002:
		INFO("PMU1: AcsmCsMapCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x004d0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D3U Type\n", args_list[0]);
		break;
	case 0x004e0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D3R Type\n", args_list[0]);
		break;
	case 0x004f0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D4U Type\n", args_list[0]);
		break;
	case 0x00500001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D4R Type\n", args_list[0]);
		break;
	case 0x00510001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D4LR Type\n", args_list[0]);
		break;
	case 0x00520000:
		INFO("PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n");
		break;
	case 0x00530003:
		INFO("PMU10: PHY TOTALS - NUM_DBYTES %d NUM_NIBBLES %d NUM_ANIBS %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00540006:
		INFO("PMU10: CSA=0x%x, CSB=0x%x, TSTAGES=0x%x, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR3\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00550006:
		INFO("PMU10: CSA=0x%x, CSB=0x%x, TSTAGES=0x%x, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR4\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00560008:
		INFO("PMU10: CS=0x%x, TSTAGES=0x%x, HDTOUT=%d, 2T=%d, MMISC=%d AddrMirror=%d DRAMFreq=%dMT DramType=%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7]);
		break;
	case 0x00570004:
		INFO("PMU10: Pstate%d MR0=0x%x MR1=0x%x MR2=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00580008:
		INFO("PMU10: Pstate%d MRS MR0=0x%x MR1=0x%x MR2=0x%x MR3=0x%x MR4=0x%x MR5=0x%x MR6=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7]);
		break;
	case 0x00590005:
		INFO("PMU10: Pstate%d MRS MR1_A0=0x%x MR2_A0=0x%x MR3_A0=0x%x MR11_A0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005a0000:
		INFO("PMU10: UseBroadcastMR set. All ranks and channels use MRXX_A0 for MR settings.\n");
		break;
	case 0x005b0005:
		INFO("PMU10: Pstate%d MRS MR01_A0=0x%x MR02_A0=0x%x MR03_A0=0x%x MR11_A0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005c0005:
		INFO("PMU10: Pstate%d MRS MR12_A0=0x%x MR13_A0=0x%x MR14_A0=0x%x MR22_A0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005d0005:
		INFO("PMU10: Pstate%d MRS MR01_A1=0x%x MR02_A1=0x%x MR03_A1=0x%x MR11_A1=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005e0005:
		INFO("PMU10: Pstate%d MRS MR12_A1=0x%x MR13_A1=0x%x MR14_A1=0x%x MR22_A1=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005f0005:
		INFO("PMU10: Pstate%d MRS MR01_B0=0x%x MR02_B0=0x%x MR03_B0=0x%x MR11_B0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00600005:
		INFO("PMU10: Pstate%d MRS MR12_B0=0x%x MR13_B0=0x%x MR14_B0=0x%x MR22_B0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00610005:
		INFO("PMU10: Pstate%d MRS MR01_B1=0x%x MR02_B1=0x%x MR03_B1=0x%x MR11_B1=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00620005:
		INFO("PMU10: Pstate%d MRS MR12_B1=0x%x MR13_B1=0x%x MR14_B1=0x%x MR22_B1=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00630002:
		INFO("PMU1: AcsmOdtCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00640002:
		INFO("PMU1: AcsmCsMapCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00650002:
		INFO("PMU1: AcsmCsMapCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00660000:
		INFO("PMU1: HwtCAMode set\n");
		break;
	case 0x00670001:
		INFO("PMU3: DDR4 infinite preamble enter/exit mode %d\n", args_list[0]);
		break;
	case 0x00680002:
		INFO("PMU1: In rxenb_train() csn=%d pstate=%d\n", args_list[0], args_list[1]);
		break;
	case 0x00690000:
		INFO("PMU3: Finding DQS falling edge\n");
		break;
	case 0x006a0000:
		INFO("PMU3: Searching for DDR3/LPDDR3/LPDDR4 read preamble\n");
		break;
	case 0x006b0009:
		INFO("PMU3: dtsm fails Even Nibbles : %x %x %x %x %x %x %x %x %x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x006c0009:
		INFO("PMU3: dtsm fails Odd  Nibbles : %x %x %x %x %x %x %x %x %x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x006d0002:
		INFO("PMU3: Preamble search pass=%d anyfail=%d\n", args_list[0], args_list[1]);
		break;
	case 0x006e0000:
		INFO("PMU: Error: RxEn training preamble not found\n");
		break;
	case 0x006f0000:
		INFO("PMU3: Found DQS pre-amble\n");
		break;
	case 0x00700001:
		INFO("PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n", args_list[0]);
		break;
	case 0x00710000:
		INFO("PMU3: RxEn aligning to first rising edge of burst\n");
		break;
	case 0x00720001:
		INFO("PMU3: Decreasing RxEn delay by %d fine step to allow full capture of reads\n", args_list[0]);
		break;
	case 0x00730001:
		INFO("PMU3: MREP Delay = %d\n", args_list[0]);
		break;
	case 0x00740003:
		INFO("PMU3: Errcnt for MREP nib %d delay = %d is %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00750002:
		INFO("PMU3: MREP nibble %d sampled a 1 at data buffer delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x00760002:
		INFO("PMU3: MREP nibble %d saw a 0 to 1 transition at data buffer delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x00770000:
		INFO("PMU2:  MREP did not find a 0 to 1 transition for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n");
		break;
	case 0x00780002:
		INFO("PMU2:  Rising edge found in alias window, setting rxDly for nibble %d = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00790002:
		INFO("PMU: Error: Failed MREP for nib %d with %d one\n", args_list[0], args_list[1]);
		break;
	case 0x007a0003:
		INFO("PMU2:  Rising edge not found in alias window with %d one, leaving rxDly for nibble %d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x007b0002:
		INFO("PMU3: Training DIMM %d CSn %d\n", args_list[0], args_list[1]);
		break;
	case 0x007c0001:
		INFO("PMU3: exitCAtrain_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x007d0001:
		INFO("PMU3: enterCAtrain_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x007e0001:
		INFO("PMU3: CAtrain_switchmsb_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x007f0001:
		INFO("PMU3: CATrain_rdwr_lp3 looking for pattern %x\n", args_list[0]);
		break;
	case 0x00800000:
		INFO("PMU3: exitCAtrain_lp4\n");
		break;
	case 0x00810001:
		INFO("PMU3: DEBUG enterCAtrain_lp4 1: cs 0x%x\n", args_list[0]);
		break;
	case 0x00820001:
		INFO("PMU3: DEBUG enterCAtrain_lp4 3: Put dbyte %d in async mode\n", args_list[0]);
		break;
	case 0x00830000:
		INFO("PMU3: DEBUG enterCAtrain_lp4 5: Send MR13 to turn on CA training\n");
		break;
	case 0x00840003:
		INFO("PMU3: DEBUG enterCAtrain_lp4 7: idx = %d vref = %x mr12 = %x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00850001:
		INFO("PMU3: CATrain_rdwr_lp4 looking for pattern %x\n", args_list[0]);
		break;
	case 0x00860004:
		INFO("PMU3: Phase %d CAreadbackA db:%d %x xo:%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00870005:
		INFO("PMU3: DEBUG lp4SetCatrVref 1: cs=%d chan=%d mr12=%x vref=%d.%d%%\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00880003:
		INFO("PMU3: DEBUG lp4SetCatrVref 3: mr12 = %x send vref= %x to db=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00890000:
		INFO("PMU10:Optimizing vref\n");
		break;
	case 0x008a0004:
		INFO("PMU4:mr12:%x cs:%d chan %d r:%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x008b0005:
		INFO("PMU3: i:%d bstr:%d bsto:%d st:%d r:%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x008c0002:
		INFO("Failed to find sufficient CA Vref Passing Region for CS %d channel %d\n", args_list[0], args_list[1]);
		break;
	case 0x008d0005:
		INFO("PMU3:Found %d.%d%% MR12:%x for cs:%d chan %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x008e0002:
		INFO("PMU3:Calculated %d for AtxImpedence from acx %d.\n", args_list[0], args_list[1]);
		break;
	case 0x008f0000:
		INFO("PMU3:CA Odt impedence ==0.  Use default vref.\n");
		break;
	case 0x00900003:
		INFO("PMU3:Calculated %d.%d%% for Vref MR12=0x%x.\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00910000:
		INFO("PMU3: CAtrain_lp\n");
		break;
	case 0x00920000:
		INFO("PMU3: CAtrain Begins.\n");
		break;
	case 0x00930001:
		INFO("PMU3: CAtrain_lp testing dly %d\n", args_list[0]);
		break;
	case 0x00940001:
		INFO("PMU5: CA bitmap dump for cs %x\n", args_list[0]);
		break;
	case 0x00950001:
		INFO("PMU5: CAA%d ", args_list[0]);
		break;
	case 0x00960001:
		INFO("%x", args_list[0]);
		break;
	case 0x00970000:
		INFO("\n");
		break;
	case 0x00980001:
		INFO("PMU5: CAB%d ", args_list[0]);
		break;
	case 0x00990001:
		INFO("%x", args_list[0]);
		break;
	case 0x009a0000:
		INFO("\n");
		break;
	case 0x009b0003:
		INFO("PMU3: anibi=%d, anibichan[anibi]=%d ,chan=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x009c0001:
		INFO("%x", args_list[0]);
		break;
	case 0x009d0001:
		INFO("\nPMU3:Raw CA setting :%x", args_list[0]);
		break;
	case 0x009e0002:
		INFO("\nPMU3:ATxDly setting:%x margin:%d\n", args_list[0], args_list[1]);
		break;
	case 0x009f0002:
		INFO("\nPMU3:InvClk ATxDly setting:%x margin:%d\n", args_list[0], args_list[1]);
		break;
	case 0x00a00000:
		INFO("\nPMU3:No Range found!\n");
		break;
	case 0x00a10003:
		INFO("PMU3: 2 anibi=%d, anibichan[anibi]=%d ,chan=%d", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00a20002:
		INFO("\nPMU3: no neg clock => CA setting anib=%d, :%d\n", args_list[0], args_list[1]);
		break;
	case 0x00a30001:
		INFO("PMU3:Normal margin:%d\n", args_list[0]);
		break;
	case 0x00a40001:
		INFO("PMU3:Inverted margin:%d\n", args_list[0]);
		break;
	case 0x00a50000:
		INFO("PMU3:Using Inverted clock\n");
		break;
	case 0x00a60000:
		INFO("PMU3:Using normal clk\n");
		break;
	case 0x00a70003:
		INFO("PMU3: 3 anibi=%d, anibichan[anibi]=%d ,chan=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00a80002:
		INFO("PMU3: Setting ATxDly for anib %x to %x\n", args_list[0], args_list[1]);
		break;
	case 0x00a90000:
		INFO("PMU: Error: CA Training Failed.\n");
		break;
	case 0x00aa0000:
		INFO("PMU1: Writing MRs\n");
		break;
	case 0x00ab0000:
		INFO("PMU4:Using MR12 values from 1D CA VREF training.\n");
		break;
	case 0x00ac0000:
		INFO("PMU3:Writing all MRs to fsp 1\n");
		break;
	case 0x00ad0000:
		INFO("PMU10:Lp4Quickboot mode.\n");
		break;
	case 0x00ae0000:
		INFO("PMU3: Writing MRs\n");
		break;
	case 0x00af0001:
		INFO("PMU10: Setting boot clock divider to %d\n", args_list[0]);
		break;
	case 0x00b00000:
		INFO("PMU3: Resetting DRAM\n");
		break;
	case 0x00b10000:
		INFO("PMU3: setup for RCD initalization\n");
		break;
	case 0x00b20000:
		INFO("PMU3: pmu_exit_SR from dev_init()\n");
		break;
	case 0x00b30000:
		INFO("PMU3: initializing RCD\n");
		break;
	case 0x00b40000:
		INFO("PMU10: **** Executing 2D Image ****\n");
		break;
	case 0x00b50001:
		INFO("PMU10: **** Start DDR4 Training. PMU Firmware Revision 0x%x ****\n", args_list[0]);
		break;
	case 0x00b60001:
		INFO("PMU10: **** Start DDR3 Training. PMU Firmware Revision 0x%x ****\n", args_list[0]);
		break;
	case 0x00b70001:
		INFO("PMU10: **** Start LPDDR3 Training. PMU Firmware Revision 0x%x ****\n", args_list[0]);
		break;
	case 0x00b80001:
		INFO("PMU10: **** Start LPDDR4 Training. PMU Firmware Revision 0x%x ****\n", args_list[0]);
		break;
	case 0x00b90000:
		INFO("PMU: Error: Mismatched internal revision between DCCM and ICCM images\n");
		break;
	case 0x00ba0001:
		INFO("PMU10: **** Testchip %d Specific ****\n", args_list[0]);
		break;
	case 0x00bb0000:
		INFO("PMU1: LRDIMM with EncodedCS mode, one DIMM\n");
		break;
	case 0x00bc0000:
		INFO("PMU1: LRDIMM with EncodedCS mode, two DIMMs\n");
		break;
	case 0x00bd0000:
		INFO("PMU1: RDIMM with EncodedCS mode, one DIMM\n");
		break;
	case 0x00be0000:
		INFO("PMU2: Starting LRDIMM MREP training for all ranks\n");
		break;
	case 0x00bf0000:
		INFO("PMU199: LRDIMM MREP training for all ranks completed\n");
		break;
	case 0x00c00000:
		INFO("PMU2: Starting LRDIMM DWL training for all ranks\n");
		break;
	case 0x00c10000:
		INFO("PMU199: LRDIMM DWL training for all ranks completed\n");
		break;
	case 0x00c20000:
		INFO("PMU2: Starting LRDIMM MRD training for all ranks\n");
		break;
	case 0x00c30000:
		INFO("PMU199: LRDIMM MRD training for all ranks completed\n");
		break;
	case 0x00c40000:
		INFO("PMU2: Starting RXEN training for all ranks\n");
		break;
	case 0x00c50000:
		INFO("PMU2: Starting write leveling fine delay training for all ranks\n");
		break;
	case 0x00c60000:
		INFO("PMU2: Starting LRDIMM MWD training for all ranks\n");
		break;
	case 0x00c70000:
		INFO("PMU199: LRDIMM MWD training for all ranks completed\n");
		break;
	case 0x00c80000:
		INFO("PMU2: Starting write leveling fine delay training for all ranks\n");
		break;
	case 0x00c90000:
		INFO("PMU2: Starting read deskew training\n");
		break;
	case 0x00ca0000:
		INFO("PMU2: Starting SI friendly 1d RdDqs training for all ranks\n");
		break;
	case 0x00cb0000:
		INFO("PMU2: Starting write leveling coarse delay training for all ranks\n");
		break;
	case 0x00cc0000:
		INFO("PMU2: Starting 1d WrDq training for all ranks\n");
		break;
	case 0x00cd0000:
		INFO("PMU2: Running DQS2DQ Oscillator for all ranks\n");
		break;
	case 0x00ce0000:
		INFO("PMU2: Starting again read deskew training but with PRBS\n");
		break;
	case 0x00cf0000:
		INFO("PMU2: Starting 1d RdDqs training for all ranks\n");
		break;
	case 0x00d00000:
		INFO("PMU2: Starting again 1d WrDq training for all ranks\n");
		break;
	case 0x00d10000:
		INFO("PMU2: Starting MaxRdLat training\n");
		break;
	case 0x00d20000:
		INFO("PMU2: Starting 2d RdDqs training for all ranks\n");
		break;
	case 0x00d30000:
		INFO("PMU2: Starting 2d WrDq training for all ranks\n");
		break;
	case 0x00d40000:
		INFO("PMU2: Starting 2d RdDqs training for all ranks\n");
		break;
	case 0x00d50002:
		INFO("PMU3:read_fifo %x %x\n", args_list[0], args_list[1]);
		break;
	case 0x00d60001:
		INFO("PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x00d70001:
		INFO("PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x00d80001:
		INFO("PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x00d90005:
		INFO("PMU3: fixRxEnBackOff csn:%d db:%d dn:%d bo:%d dly:%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00da0001:
		INFO("PMU3: fixRxEnBackOff dly:%x\n", args_list[0]);
		break;
	case 0x00db0000:
		INFO("PMU3: Entering setupPpt\n");
		break;
	case 0x00dc0000:
		INFO("PMU3: Start lp4PopulateHighLowBytes\n");
		break;
	case 0x00dd0002:
		INFO("PMU3:Dbyte Detect: db%d received %x\n", args_list[0], args_list[1]);
		break;
	case 0x00de0002:
		INFO("PMU3:getDqs2Dq read %x from dbyte %d\n", args_list[0], args_list[1]);
		break;
	case 0x00df0002:
		INFO("PMU3:getDqs2Dq(2) read %x from dbyte %d\n", args_list[0], args_list[1]);
		break;
	case 0x00e00001:
		INFO("PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n", args_list[0]);
		break;
	case 0x00e10002:
		INFO("PMU4: Dbyte %d dqs2dq = %d/32 UI\n", args_list[0], args_list[1]);
		break;
	case 0x00e20003:
		INFO("PMU3:getDqs2Dq set dqs2dq:%d/32 ui (%d ps) from dbyte %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e30003:
		INFO("PMU3: Setting coarse delay in AtxDly chiplet %d from 0x%x to 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e40003:
		INFO("PMU3: Clearing coarse delay in AtxDly chiplet %d from 0x%x to 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e50000:
		INFO("PMU3: Performing DDR4 geardown sync sequence\n");
		break;
	case 0x00e60000:
		INFO("PMU1: Enter self refresh\n");
		break;
	case 0x00e70000:
		INFO("PMU1: Exit self refresh\n");
		break;
	case 0x00e80000:
		INFO("PMU: Error: No dbiEnable with lp4\n");
		break;
	case 0x00e90000:
		INFO("PMU: Error: No dbiDisable with lp4\n");
		break;
	case 0x00ea0001:
		INFO("PMU1: DDR4 update Rx DBI Setting disable %d\n", args_list[0]);
		break;
	case 0x00eb0001:
		INFO("PMU1: DDR4 update 2nCk WPre Setting disable %d\n", args_list[0]);
		break;
	case 0x00ec0005:
		INFO("PMU1: read_delay: db%d lane%d delays[%d] = 0x%x (max 0x%x)\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00ed0004:
		INFO("PMU1: write_delay: db%d lane%d delays[%d] = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00ee0001:
		INFO("PMU5: ID=%d -- db0  db1  db2  db3  db4  db5  db6  db7  db8  db9 --\n", args_list[0]);
		break;
	case 0x00ef000b:
		INFO("PMU5: [%d]:0x %x %x %x %x %x %x %x %x %x %x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8], args_list[9], args_list[10]);
		break;
	case 0x00f00003:
		INFO("PMU2: dump delays - pstate=%d dimm=%d csn=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00f10000:
		INFO("PMU3: Printing Mid-Training Delay Information\n");
		break;
	case 0x00f20001:
		INFO("PMU5: CS%d <<KEY>> 0 TrainingCntr <<KEY>> coarse(15:10) fine(9:0)\n", args_list[0]);
		break;
	case 0x00f30001:
		INFO("PMU5: CS%d <<KEY>> 0 RxEnDly, 1 RxClkDly <<KEY>> coarse(10:6) fine(5:0)\n", args_list[0]);
		break;
	case 0x00f40001:
		INFO("PMU5: CS%d <<KEY>> 0 TxDqsDly, 1 TxDqDly <<KEY>> coarse(9:6) fine(5:0)\n", args_list[0]);
		break;
	case 0x00f50001:
		INFO("PMU5: CS%d <<KEY>> 0 RxPBDly <<KEY>> 1 Delay Unit ~= 7ps\n", args_list[0]);
		break;
	case 0x00f60000:
		INFO("PMU5: all CS <<KEY>> 0 DFIMRL <<KEY>> Units = DFI clocks\n");
		break;
	case 0x00f70000:
		INFO("PMU5: all CS <<KEY>> VrefDACs <<KEY>> DAC(6:0)\n");
		break;
	case 0x00f80000:
		INFO("PMU1: Set DMD in MR13 and wrDBI in MR3 for training\n");
		break;
	case 0x00f90000:
		INFO("PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n");
		break;
	case 0x00fa0003:
		INFO("PMU2: getMaxRxen(): maxDly %d maxTg %d maxNib %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00fb0003:
		INFO("PMU2: getRankMaxRxen(): maxDly %d Tg %d maxNib %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00fc0000:
		INFO("PMU1: skipping CDD calculation in 2D image\n");
		break;
	case 0x00fd0001:
		INFO("PMU3: Calculating CDDs for pstate %d\n", args_list[0]);
		break;
	case 0x00fe0003:
		INFO("PMU3: rxFromDly[%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00ff0003:
		INFO("PMU3: rxToDly  [%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01000003:
		INFO("PMU3: rxDly	[%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01010003:
		INFO("PMU3: txDly	[%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01020003:
		INFO("PMU3: allFine CDD_RR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01030003:
		INFO("PMU3: allFine CDD_WW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01040003:
		INFO("PMU3: CDD_RR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01050003:
		INFO("PMU3: CDD_WW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01060003:
		INFO("PMU3: allFine CDD_RW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01070003:
		INFO("PMU3: allFine CDD_WR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01080003:
		INFO("PMU3: CDD_RW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01090003:
		INFO("PMU3: CDD_WR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x010a0004:
		INFO("PMU3: F%dBC2x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010b0004:
		INFO("PMU3: F%dBC3x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010c0004:
		INFO("PMU3: F%dBC4x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010d0004:
		INFO("PMU3: F%dBC5x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010e0004:
		INFO("PMU3: F%dBC8x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010f0004:
		INFO("PMU3: F%dBC9x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01100004:
		INFO("PMU3: F%dBCAx_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01110004:
		INFO("PMU3: F%dBCBx_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01120000:
		INFO("PMU10: Entering context_switch_postamble\n");
		break;
	case 0x01130003:
		INFO("PMU10: context_switch_postamble is enabled for DIMM %d, RC0A=0x%x, RC3x=0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01140000:
		INFO("PMU10: Setting bcw fspace 0\n");
		break;
	case 0x01150001:
		INFO("PMU10: Sending BC0A = 0x%x\n", args_list[0]);
		break;
	case 0x01160001:
		INFO("PMU10: Sending BC6x = 0x%x\n", args_list[0]);
		break;
	case 0x01170001:
		INFO("PMU10: Sending RC0A = 0x%x\n", args_list[0]);
		break;
	case 0x01180001:
		INFO("PMU10: Sending RC3x = 0x%x\n", args_list[0]);
		break;
	case 0x01190001:
		INFO("PMU10: Sending RC0A = 0x%x\n", args_list[0]);
		break;
	case 0x011a0001:
		INFO("PMU1: enter_lp3: DEBUG: pstate = %d\n", args_list[0]);
		break;
	case 0x011b0001:
		INFO("PMU1: enter_lp3: DEBUG: dfifreqxlat_pstate = %d\n", args_list[0]);
		break;
	case 0x011c0001:
		INFO("PMU1: enter_lp3: DEBUG: pllbypass = %d\n", args_list[0]);
		break;
	case 0x011d0001:
		INFO("PMU1: enter_lp3: DEBUG: forcecal = %d\n", args_list[0]);
		break;
	case 0x011e0001:
		INFO("PMU1: enter_lp3: DEBUG: pllmaxrange = 0x%x\n", args_list[0]);
		break;
	case 0x011f0001:
		INFO("PMU1: enter_lp3: DEBUG: dacval_out = 0x%x\n", args_list[0]);
		break;
	case 0x01200001:
		INFO("PMU1: enter_lp3: DEBUG: pllctrl3 = 0x%x\n", args_list[0]);
		break;
	case 0x01210000:
		INFO("PMU3: Loading DRAM with BIOS supplied MR values and entering self refresh prior to exiting PMU code.\n");
		break;
	case 0x01220002:
		INFO("PMU3: Setting DataBuffer function space of dimmcs 0x%x to %d\n", args_list[0], args_list[1]);
		break;
	case 0x01230002:
		INFO("PMU4: Setting RCW FxRC%xx = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01240002:
		INFO("PMU4: Setting RCW FxRC%x = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01250001:
		INFO("PMU1: DDR4 update Rd Pre Setting disable %d\n", args_list[0]);
		break;
	case 0x01260002:
		INFO("PMU2: Setting BCW FxBC%xx = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01270002:
		INFO("PMU2: Setting BCW BC%x = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01280002:
		INFO("PMU2: Setting BCW PBA mode FxBC%xx = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01290002:
		INFO("PMU2: Setting BCW PBA mode BC%x = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x012a0003:
		INFO("PMU4: BCW value for dimm %d, fspace %d, addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x012b0002:
		INFO("PMU4: DB %d, value 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x012c0000:
		INFO("PMU6: WARNING MREP underflow, set to min value -2 coarse, 0 fine\n");
		break;
	case 0x012d0004:
		INFO("PMU6: LRDIMM Writing final data buffer fine delay value nib %d, trainDly %d, fineDly code %d, new MREP fine %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x012e0003:
		INFO("PMU6: LRDIMM Writing final data buffer fine delay value nib %d, trainDly %d, fineDly code %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x012f0003:
		INFO("PMU6: LRDIMM Writing data buffer fine delay type %d nib %d, code %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01300002:
		INFO("PMU6: Writing final data buffer coarse delay value dbyte %d, coarse = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01310003:
		INFO("PMU4: data 0x%x at MB addr 0x%x saved at CSR addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01320003:
		INFO("PMU4: data 0x%x at MB addr 0x%x restored from CSR addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01330003:
		INFO("PMU4: data 0x%x at MB addr 0x%x saved at CSR addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01340003:
		INFO("PMU4: data 0x%x at MB addr 0x%x restored from CSR addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01350001:
		INFO("PMU3: Update BC00, BC01, BC02 for rank-dimm 0x%x\n", args_list[0]);
		break;
	case 0x01360000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F0RC00 -> F0RC0F\n");
		break;
	case 0x01370000:
		INFO("PMU3: Disable parity in F0RC0E\n");
		break;
	case 0x01380000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F1RC00 -> F1RC05\n");
		break;
	case 0x01390000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F1RC1x -> F1RC9x\n");
		break;
	case 0x013a0000:
		INFO("PMU3: Writing D4 Data buffer Control words BC00 -> BC0E\n");
		break;
	case 0x013b0002:
		INFO("PMU1: setAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x013c0002:
		INFO("PMU1: restoreFromAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x013d0002:
		INFO("PMU1: restoreAcsmFromAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x013e0002:
		INFO("PMU2: Setting D3R RC%d = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x013f0000:
		INFO("PMU3: Writing D3 RDIMM RCD Control words RC0 -> RC11\n");
		break;
	case 0x01400002:
		INFO("PMU0: VrefDAC0/1 vddqStart %d dacToVddq %d\n", args_list[0], args_list[1]);
		break;
	case 0x01410001:
		INFO("PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n", args_list[0]);
		break;
	case 0x01420001:
		INFO("PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n", args_list[0]);
		break;
	case 0x01430001:
		INFO("PMU0: PHY VREF @ (%d/1000) VDDQ\n", args_list[0]);
		break;
	case 0x01440002:
		INFO("PMU0: initalizing phy vrefDacs to %d ExtVrefRange %x\n", args_list[0], args_list[1]);
		break;
	case 0x01450002:
		INFO("PMU0: initalizing global vref to %d range %d\n", args_list[0], args_list[1]);
		break;
	case 0x01460002:
		INFO("PMU4: Setting initial device vrefDQ for CS%d to MR6 = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01470003:
		INFO("PMU1: In write_level_fine() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01480000:
		INFO("PMU3: Fine write leveling hardware search increasing TxDqsDly until full bursts are seen\n");
		break;
	case 0x01490000:
		INFO("PMU4: WL normalized pos   : ........................|........................\n");
		break;
	case 0x014a0007:
		INFO("PMU4: WL margin for nib %d: %x%x%x%x%x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6]);
		break;
	case 0x014b0000:
		INFO("PMU4: WL normalized pos   : ........................|........................\n");
		break;
	case 0x014c0000:
		INFO("PMU3: Exiting write leveling mode\n");
		break;
	case 0x014d0001:
		INFO("PMU3: got %d for cl in load_wrlvl_acsm\n", args_list[0]);
		break;
	case 0x014e0003:
		INFO("PMU1: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x014f0003:
		INFO("PMU3: left eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01500003:
		INFO("PMU3: right eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01510004:
		INFO("PMU3: eye center db:%d ln:%d dly:0x%x (maxdq:%x)\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01520003:
		INFO("PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01530003:
		INFO("PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01540002:
		INFO("PMU3: Coarse write leveling dbyte%d is still failing for TxDqsDly=0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01550002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x01560000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x01570001:
		INFO("PMU3: got %d for cl in load_wrlvl_acsm\n", args_list[0]);
		break;
	case 0x01580003:
		INFO("PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01590003:
		INFO("PMU3: left eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x015a0003:
		INFO("PMU3: right eye edge search db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x015b0004:
		INFO("PMU3: eye center db: %d ln: %d dly: 0x%x (maxdq: 0x%x)\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x015c0003:
		INFO("PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x015d0003:
		INFO("PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x015e0002:
		INFO("PMU3: Coarse write leveling nibble%d is still failing for TxDqsDly=0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x015f0002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x01600000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x01610000:
		INFO("PMU4: WL normalized pos   : ................................|................................\n");
		break;
	case 0x01620009:
		INFO("PMU4: WL margin for nib %d: %x%x%x%x%x%x%x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x01630000:
		INFO("PMU4: WL normalized pos   : ................................|................................\n");
		break;
	case 0x01640001:
		INFO("PMU8: Adjust margin after WL coarse to be larger than %d\n", args_list[0]);
		break;
	case 0x01650001:
		INFO("PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n", args_list[0]);
		break;
	case 0x01660002:
		INFO("PMU8: Decrement nib %d TxDqsDly by %d fine step\n", args_list[0], args_list[1]);
		break;
	case 0x01670003:
		INFO("PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01680005:
		INFO("PMU2: Write level: dbyte %d nib%d dq/dmbi %d dqsfine 0x%x dqDly 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x01690002:
		INFO("PMU3: Coarse write leveling nibble%d is still failing for TxDqsDly=0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x016a0002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x016b0000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x016c0001:
		INFO("PMU3: DWL delay = %d\n", args_list[0]);
		break;
	case 0x016d0003:
		INFO("PMU3: Errcnt for DWL nib %d delay = %d is %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x016e0002:
		INFO("PMU3: DWL nibble %d sampled a 1 at delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x016f0003:
		INFO("PMU3: DWL nibble %d passed at delay %d. Rising edge was at %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01700000:
		INFO("PMU2: DWL did nto find a rising edge of memclk for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n");
		break;
	case 0x01710002:
		INFO("PMU2:  Rising edge found in alias window, setting wrlvlDly for nibble %d = %d\n", args_list[0], args_list[1]);
		break;
	case 0x01720002:
		INFO("PMU: Error: Failed DWL for nib %d with %d one\n", args_list[0], args_list[1]);
		break;
	case 0x01730003:
		INFO("PMU2:  Rising edge not found in alias window with %d one, leaving wrlvlDly for nibble %d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04000000:
		INFO("PMU: Error:Mailbox Buffer Overflowed.\n");
		break;
	case 0x04010000:
		INFO("PMU: Error:Mailbox Buffer Overflowed.\n");
		break;
	case 0x04020000:
		INFO("PMU: ***** Assertion Error - terminating *****\n");
		break;
	case 0x04030002:
		INFO("PMU1: swapByte db %d by %d\n", args_list[0], args_list[1]);
		break;
	case 0x04040003:
		INFO("PMU3: get_cmd_dly max(%d ps, %d memclk) = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04050002:
		INFO("PMU0: Write CSR 0x%x 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x04060002:
		INFO("PMU0: hwt_init_ppgc_prbs(): Polynomial: %x, Deg: %d\n", args_list[0], args_list[1]);
		break;
	case 0x04070001:
		INFO("PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n", args_list[0]);
		break;
	case 0x04080001:
		INFO("PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n", args_list[0]);
		break;
	case 0x0409000c:
		INFO("PMU1: acsm_addr %x, acsm_flgs %x, ddr_cmd %x, cmd_dly %x, ddr_addr %x, ddr_bnk %x, ddr_cs %x, cmd_rcnt %x, AcsmSeq0/1/2/3 %x %x %x %x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8], args_list[9], args_list[10], args_list[11]);
		break;
	case 0x040a0000:
		INFO("PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n");
		break;
	case 0x040b0000:
		INFO("PMU1: acsm RUN\n");
		break;
	case 0x040c0000:
		INFO("PMU1: acsm STOPPED\n");
		break;
	case 0x040d0002:
		INFO("PMU1: acsm_init: acsm_mode %x mxrdlat %x\n", args_list[0], args_list[1]);
		break;
	case 0x040e0002:
		INFO("PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n", args_list[0], args_list[1]);
		break;
	case 0x040f0002:
		INFO("PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n", args_list[0], args_list[1]);
		break;
	case 0x04100002:
		INFO("PMU1: setAcsmCLCWL: CASL %d WCASL %d\n", args_list[0], args_list[1]);
		break;
	case 0x04110001:
		INFO("PMU: Error: Reserved value of register F0RC0F found in message block: 0x%x\n", args_list[0]);
		break;
	case 0x04120001:
		INFO("PMU3: Written MRS to CS=0x%x\n", args_list[0]);
		break;
	case 0x04130001:
		INFO("PMU3: Written MRS to CS=0x%x\n", args_list[0]);
		break;
	case 0x04140000:
		INFO("PMU3: Entering Boot Freq Mode.\n");
		break;
	case 0x04150001:
		INFO("PMU: Error: Boot clock divider setting of %d is too small\n", args_list[0]);
		break;
	case 0x04160000:
		INFO("PMU3: Exiting Boot Freq Mode.\n");
		break;
	case 0x04170002:
		INFO("PMU3: Writing MR%d OP=%x\n", args_list[0], args_list[1]);
		break;
	case 0x04180000:
		INFO("PMU: Error: Delay too large in slomo\n");
		break;
	case 0x04190001:
		INFO("PMU3: Written MRS to CS=0x%x\n", args_list[0]);
		break;
	case 0x041a0000:
		INFO("PMU3: Enable Channel A\n");
		break;
	case 0x041b0000:
		INFO("PMU3: Enable Channel B\n");
		break;
	case 0x041c0000:
		INFO("PMU3: Enable All Channels\n");
		break;
	case 0x041d0002:
		INFO("PMU2: Use PDA mode to set MR%d with value 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x041e0001:
		INFO("PMU3: Written Vref with PDA to CS=0x%x\n", args_list[0]);
		break;
	case 0x041f0000:
		INFO("PMU1: start_cal: DEBUG: setting CalRun to 1\n");
		break;
	case 0x04200000:
		INFO("PMU1: start_cal: DEBUG: setting CalRun to 0\n");
		break;
	case 0x04210001:
		INFO("PMU1: lock_pll_dll: DEBUG: pstate = %d\n", args_list[0]);
		break;
	case 0x04220001:
		INFO("PMU1: lock_pll_dll: DEBUG: dfifreqxlat_pstate = %d\n", args_list[0]);
		break;
	case 0x04230001:
		INFO("PMU1: lock_pll_dll: DEBUG: pllbypass = %d\n", args_list[0]);
		break;
	case 0x04240001:
		INFO("PMU3: SaveLcdlSeed: Saving seed %d\n", args_list[0]);
		break;
	case 0x04250000:
		INFO("PMU1: in phy_defaults()\n");
		break;
	case 0x04260003:
		INFO("PMU3: ACXConf:%d MaxNumDbytes:%d NumDfi:%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04270005:
		INFO("PMU1: setAltAcsmCLCWL setting cl=%d cwl=%d %d %d %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	default:
		INFO("PMU Streaming Msg: Debug message not recognized !!  code: %x", codede_message_hex);
		panic();
	}
#endif
}

static void decode_streaming_message_ddr4_2d(uint32_t codede_message_hex, uint16_t *args_list)
{
#if (DDR_LOG_EN && DEBUG)
	switch (codede_message_hex) {
	case 0x00000001:
		INFO("PMU0: Converting %d into an MR\n", args_list[0]);
		break;
	case 0x00010003:
		INFO("PMU DEBUG: vref_idx %d -= %d, range_idx = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00020002:
		INFO("PMU0: vrefIdx. Passing range %d, remaining vrefidx = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00030002:
		INFO("PMU0: VrefIdx %d -> MR[6:0] 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00040001:
		INFO("PMU0: Converting MR 0x%x to vrefIdx\n", args_list[0]);
		break;
	case 0x00050002:
		INFO("PMU0: DAC %d Range %d\n", args_list[0], args_list[1]);
		break;
	case 0x00060003:
		INFO("PMU0: Range %d, Range_idx %d, vref_idx offset %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00070002:
		INFO("PMU0: MR 0x%x -> VrefIdx %d\n", args_list[0], args_list[1]);
		break;
	case 0x00080001:
		INFO("PMU: Error: Illegal timing group number ,%d, in getPtrVrefDq\n", args_list[0]);
		break;
	case 0x00090003:
		INFO("PMU1: VrefDqR%dNib%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x000a0003:
		INFO("PMU0: VrefDqR%dNib%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x000b0000:
		INFO("PMU0: ----------------MARGINS-------\n");
		break;
	case 0x000c0002:
		INFO("PMU0: R%d_RxClkDly_Margin = %d\n", args_list[0], args_list[1]);
		break;
	case 0x000d0002:
		INFO("PMU0: R%d_VrefDac_Margin = %d\n", args_list[0], args_list[1]);
		break;
	case 0x000e0002:
		INFO("PMU0: R%d_TxDqDly_Margin = %d\n", args_list[0], args_list[1]);
		break;
	case 0x000f0002:
		INFO("PMU0: R%d_DeviceVref_Margin = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00100000:
		INFO("PMU0: -----------------------\n");
		break;
	case 0x00110003:
		INFO("PMU0: eye %d's for all TG's is [%d ... %d]\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00120000:
		INFO("PMU0: ------- settingWeight -----\n");
		break;
	case 0x00130002:
		INFO("PMU0: Weight %d @ Setting %d\n", args_list[0], args_list[1]);
		break;
	case 0x0014001f:
		INFO("PMU4: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d >%d< %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8], args_list[9], args_list[10], args_list[11], args_list[12], args_list[13], args_list[14], args_list[15], args_list[16], args_list[17], args_list[18], args_list[19], args_list[20], args_list[21], args_list[22], args_list[23], args_list[24], args_list[25], args_list[26], args_list[27], args_list[28], args_list[29], args_list[30]);
		break;
	case 0x00150002:
		INFO("PMU3: Voltage Range = [%d, %d]\n", args_list[0], args_list[1]);
		break;
	case 0x00160004:
		INFO("PMU4: -- DB%d L%d -- centers: delay = %d, voltage = %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00170001:
		INFO("PMU5: <<KEY>> 0 TxDqDlyTg%d <<KEY>> coarse(6:6) fine(5:0)\n", args_list[0]);
		break;
	case 0x00180001:
		INFO("PMU5: <<KEY>> 0 messageBlock VrefDqR%d <<KEY>> MR14(6:0)\n", args_list[0]);
		break;
	case 0x00190001:
		INFO("PMU5: <<KEY>> 0 messageBlock VrefDqR%d <<KEY>> MR6(6:0)\n", args_list[0]);
		break;
	case 0x001a0001:
		INFO("PMU5: <<KEY>> 0 RxClkDlyTg%d <<KEY>> fine(5:0)\n", args_list[0]);
		break;
	case 0x001b0003:
		INFO("PMU0: tgToCsn: tg %d + 0x%x -> csn %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x001c0002:
		INFO("PMU: Error: LP4 rank %d cannot be mapped on tg %d\n", args_list[0], args_list[1]);
		break;
	case 0x001d0002:
		INFO("PMU3: Sending vref %d,  Mr = 0X%x, to all devices\n", args_list[0], args_list[1]);
		break;
	case 0x001e0004:
		INFO("PMU4: -------- %dD Write Scanning TG %d (CS 0x%x) Lanes 0x%x --------\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x001f0002:
		INFO("PMU0: training lanes 0x%x using lanes 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00200003:
		INFO("PMU4: ------- 2D-DFE Read Scanning TG %d (CS 0x%x) Lanes 0x%x -------\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00210004:
		INFO("PMU4: ------- %dD Read Scanning TG %d (CS 0x%x) Lanes 0x%x -------\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00220003:
		INFO("PMU4: TG%d MR1[13,6,5]=0x%x MR6[13,9,8]=0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00230002:
		INFO("PMU0: training lanes 0x%x using lanes 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00240003:
		INFO("PMU4: ------- 2D-DFE Read Scanning TG %d (CS 0x%x) Lanes 0x%x -------\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00250004:
		INFO("PMU4: ------- %dD Read Scanning TG %d (CS 0x%x) Lanes 0x%x -------\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00260002:
		INFO("PMU0: training lanes 0x%x using lanes 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00270002:
		INFO("PMU3: Sending vref %d,  Mr = 0X%x, to all devices\n", args_list[0], args_list[1]);
		break;
	case 0x00280004:
		INFO("PMU4: -------- %dD Write Scanning TG %d (CS 0x%x) Lanes 0x%x --------\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00290001:
		INFO("PMU0: input %d\n", args_list[0]);
		break;
	case 0x002a0002:
		INFO("PMU4: Programmed Voltage Search Range [%d, %d]\n", args_list[0], args_list[1]);
		break;
	case 0x002b0002:
		INFO("PMU3: Delay Stepsize = %d Fine, Voltage Stepsize = %d DAC\n", args_list[0], args_list[1]);
		break;
	case 0x002c0002:
		INFO("PMU4: Delay Weight = %d, Voltage Weight = %d\n", args_list[0], args_list[1]);
		break;
	case 0x002d0003:
		INFO("PMU0: raw 0x%x allFine %d incDec %d", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x002e0008:
		INFO("PMU0: db%d l%d, voltage 0x%x (u_r %d) delay 0x%x (u_r %d) - lcdl %d mask 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7]);
		break;
	case 0x002f0005:
		INFO("PMU0: DB%d L%d, Eye %d, Seed = (0x%x, 0x%x)\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00300002:
		INFO("PMU3: 2D Enables       : %d,                    1,                %d\n", args_list[0], args_list[1]);
		break;
	case 0x00310006:
		INFO("PMU3: 2D Delay   Ranges: OOPL[0x%x,0x%x], IP[0x%x,0x%x], OOPR[0x%x,0x%x]\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00320002:
		INFO("PMU3: 2D Voltage Search Range : [%d, %d]\n", args_list[0], args_list[1]);
		break;
	case 0x00330002:
		INFO("PMU4: Found Voltage Search Range [%d, %d]\n", args_list[0], args_list[1]);
		break;
	case 0x00340002:
		INFO("PMU0: User Weight = %d, Voltage Weight = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00350005:
		INFO("PMU0: D(%d,%d) V(%d,%d | %d)\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00360002:
		INFO("PMU0: Norm Weight = %d, Voltage Weight = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00370002:
		INFO("PMU0: seed 0 = (%d,%d) (center)\n", args_list[0], args_list[1]);
		break;
	case 0x00380003:
		INFO("PMU0: seed 1 = (%d,%d).min edge at idx %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00390003:
		INFO("PMU0: seed 2 = (%d,%d) max edge at idx %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x003a0003:
		INFO("PMU0: Search point %d = (%d,%d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x003b0005:
		INFO("PMU0: YMARGIN: ^ %d, - %d, v %d. rate %d = %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x003c0003:
		INFO("PMU0: XMARGIN: center %d, edge %d. = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x003d0002:
		INFO("PMU0: ----------- weighting (%d,%d) ----------------\n", args_list[0], args_list[1]);
		break;
	case 0x003e0003:
		INFO("PMU0: X margin - L %d R %d - Min %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x003f0003:
		INFO("PMU0: Y margin - L %d R %d - Min %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00400003:
		INFO("PMU0: center (%d,%d) weight = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00410003:
		INFO("PMU4: Eye largest blob area %d from %d to %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00420002:
		INFO("PMU0: Compute centroid min_x %d max_x %d\n", args_list[0], args_list[1]);
		break;
	case 0x00430003:
		INFO("PMU0: Compute centroid sumLnDlyWidth %d sumLnVrefWidth %d sumLnWidth %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00440000:
		INFO("PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n");
		break;
	case 0x00450003:
		INFO("PMU0: Centroid ( %d, %d ) found with sumLnWidth %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00460003:
		INFO("PMU0: Optimal allFine Center ( %d + %d ,%d )\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00470004:
		INFO("PMU0: Compute sumDlyVrefWidth %d sumVrefWidth %d sumVrefDlyWidth %d sumDlyWidth %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00480000:
		INFO("PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n");
		break;
	case 0x00490002:
		INFO("PMU0: Average center ( %d, %d ) found\n", args_list[0], args_list[1]);
		break;
	case 0x004a0003:
		INFO("PMU0: Optimal allFine Center ( %d + %d ,%d )\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x004b0003:
		INFO("PMU3: point %d starting at (%d,%d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x004c0006:
		INFO("PMU3: point %d starting at (%d,%d) with x_step %d y_step %d for %d iterations\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x004d0002:
		INFO("PMU0: picking left (%d > %d)\n", args_list[0], args_list[1]);
		break;
	case 0x004e0002:
		INFO("PMU0: picking right (%d > %d)\n", args_list[0], args_list[1]);
		break;
	case 0x004f0002:
		INFO("PMU0: picking down (%d > %d)\n", args_list[0], args_list[1]);
		break;
	case 0x00500002:
		INFO("PMU0: picking up (%d > %d)\n", args_list[0], args_list[1]);
		break;
	case 0x00510009:
		INFO("PMU3: new center @ (%d, %d). Moved (%i, %i) -- L %d, R %d, C %d, U %d, D %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x00520003:
		INFO("PMU3: cordNum %d improved %d to %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00530000:
		INFO("PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n");
		break;
	case 0x00540004:
		INFO("PMU0: Optimal allFine Center ( %d + %d ,%d ), found with weight %d.\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00550003:
		INFO("PMU0: merging lanes=%d..%d, centerMerge_t %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00560001:
		INFO("PMU0: laneVal %d is disable\n", args_list[0]);
		break;
	case 0x00570001:
		INFO("PMU0: laneVal %d is disable\n", args_list[0]);
		break;
	case 0x00580002:
		INFO("PMU0: checking common center %d against current center %d\n", args_list[0], args_list[1]);
		break;
	case 0x00590001:
		INFO("PMU: Error: getCompoundEye Called on lane%d eye with non-compatible centers\n", args_list[0]);
		break;
	case 0x005a0001:
		INFO("PMU0: laneItr %d is disable\n", args_list[0]);
		break;
	case 0x005b0001:
		INFO("PMU0: laneItr %d is disable\n", args_list[0]);
		break;
	case 0x005c0005:
		INFO("PMU0: lane %d, data_idx %d, offset_idx %d, = [%d..%d]\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005d0003:
		INFO("PMU0: lane %d, data_idx %d, offset_idx %d, offset_idx out of range!\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x005e0003:
		INFO("PMU0: mergeData[%d] = max_v_low %d, min_v_high %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x005f0005:
		INFO("PMU1: writing merged center (%d,%d) back to dataBlock[%d]. doDelay %d, doVoltage %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00600005:
		INFO("PMU0: applying relative (%i,%i) back to dataBlock[%d]. doDelay %d, doVoltage %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00610002:
		INFO("PMU0: drvstren %x is idx %d in the table\n", args_list[0], args_list[1]);
		break;
	case 0x00620000:
		INFO("PMU4: truncating FFE drive strength search range. Out of drive strengths to check.\n");
		break;
	case 0x00630002:
		INFO("PMU5: Weak 1 changed to pull-up %d ohms, pull-down %d ohms\n", args_list[0], args_list[1]);
		break;
	case 0x00640002:
		INFO("PMU5: Weak 0 changed to pull-up %d ohms, pull-down %d ohms\n", args_list[0], args_list[1]);
		break;
	case 0x00650003:
		INFO("PMU0: dlyMargin L %d R %d, min %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00660003:
		INFO("PMU0: vrefMargin T %d B %d, min %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00670002:
		INFO("PMU3: new minimum VrefMargin (%d < %d) recorded\n", args_list[0], args_list[1]);
		break;
	case 0x00680002:
		INFO("PMU3: new minimum DlyMargin (%d < %d) recorded\n", args_list[0], args_list[1]);
		break;
	case 0x00690000:
		INFO("PMU0: RX finding the per-nibble, per-tg rxClkDly values\n");
		break;
	case 0x006a0003:
		INFO("PMU0: Merging collected eyes [%d..%d) and analyzing for nibble %d's optimal rxClkDly\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x006b0002:
		INFO("PMU0: -- centers: delay = %d, voltage = %d\n", args_list[0], args_list[1]);
		break;
	case 0x006c0003:
		INFO("PMU0: dumping optimized eye -- centers: delay = %d (%d), voltage = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x006d0000:
		INFO("PMU0: TX optimizing txDqDelays\n");
		break;
	case 0x006e0001:
		INFO("PMU3: Analyzing collected eye %d for a lane's optimal TxDqDly\n", args_list[0]);
		break;
	case 0x006f0001:
		INFO("PMU0: eye-lane %d is disable\n", args_list[0]);
		break;
	case 0x00700000:
		INFO("PMU0: TX optimizing device voltages\n");
		break;
	case 0x00710002:
		INFO("PMU0: Merging collected eyes [%d..%d) and analyzing for optimal device txVref\n", args_list[0], args_list[1]);
		break;
	case 0x00720002:
		INFO("PMU0: -- centers: delay = %d, voltage = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00730003:
		INFO("PMU0: dumping optimized eye -- centers: delay = %d (%d), voltage = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00740000:
		INFO("PMU4: VrefDac (compound all TG) Bottom Top -> Center\n");
		break;
	case 0x00750005:
		INFO("PMU4: DB%d L%d   %d   %d  ->  %d (DISCONNECTED)\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00760005:
		INFO("PMU4: DB%d L%d   %d   %d  ->  %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00770005:
		INFO("PMU0: writing rxClkDelay for tg%d db%d nib%d to 0x%x from eye[%d] (DISCONNECTED)\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00780003:
		INFO("PMU: Error: Dbyte %d nibble %d's optimal rxClkDly of 0x%x is out of bounds\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00790005:
		INFO("PMU0: writing rxClkDelay for tg%d db%d nib%d to 0x%x from eye[%d]\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x007a0005:
		INFO("PMU0: tx voltage for tg%d nib%d to %d (%d) from eye[%d]\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x007b0001:
		INFO("PMU0: vref Sum = %d\n", args_list[0]);
		break;
	case 0x007c0004:
		INFO("PMU0: tx voltage total is %d/%d -> %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x007d0007:
		INFO("PMU0: writing txDqDelay for tg%d db%d ln%d to  0x%x (%d coarse, %d fine) from eye[%d] (DISCONNECTED)\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6]);
		break;
	case 0x007e0003:
		INFO("PMU: Error: Dbyte %d lane %d's optimal txDqDly of 0x%x is out of bounds\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x007f0007:
		INFO("PMU0: writing txDqDelay for tg%d db%d l%d to  0x%x (%d coarse, %d fine) from eye[%d]\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6]);
		break;
	case 0x00800002:
		INFO("PMU0: %d (0=tx, 1=rx) TgMask for this simulation: %x\n", args_list[0], args_list[1]);
		break;
	case 0x00810001:
		INFO("PMU0: eye-byte %d is disable\n", args_list[0]);
		break;
	case 0x00820001:
		INFO("PMU0: eye-lane %d is disable\n", args_list[0]);
		break;
	case 0x00830003:
		INFO("PMU10: Start d4_2d_lrdimm_rx_dfe dimm %d nbTap %d biasStepMode %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00840001:
		INFO("PMU10: DB DFE feature not fully supported, F2BCEx value is 0x%x\n", args_list[0]);
		break;
	case 0x00850001:
		INFO("PMU10: DB DFE feature fully supported, F2BCEx value is 0x%x\n", args_list[0]);
		break;
	case 0x00860002:
		INFO("PMU8: Start d4_2d_lrdimm_rx_dfe for tap %d biasStepInc %d\n", args_list[0], args_list[1]);
		break;
	case 0x00870001:
		INFO("PMU7: Start d4_2d_lrdimm_rx_dfe tapCoff 0x%x\n", args_list[0]);
		break;
	case 0x00880003:
		INFO("PMU6: d4_2d_lrdimm_rx_dfe db %d lane %d area %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00890004:
		INFO("PMU7: d4_2d_lrdimm_rx_dfe db %d lane %d max area %d best bias 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x008a0001:
		INFO("PMU0: eye-lane %d is disable\n", args_list[0]);
		break;
	case 0x008b0003:
		INFO("PMU5: Setting 0x%x improved rank weight (%d < %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x008c0001:
		INFO("PMU4: Setting 0x%x still optimal\n", args_list[0]);
		break;
	case 0x008d0002:
		INFO("PMU5: ---- Training CS%d MR%d DRAM Equalization ----\n", args_list[0], args_list[1]);
		break;
	case 0x008e0001:
		INFO("PMU0: eye-lane %d is disable\n", args_list[0]);
		break;
	case 0x008f0003:
		INFO("PMU0: eye %d weight %d allTgWeight %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00900002:
		INFO("PMU5: FFE figure of merit improved from %d to %d\n", args_list[0], args_list[1]);
		break;
	case 0x00910001:
		INFO("PMU5: Rejecting FFE figure of merit of %d\n", args_list[0]);
		break;
	case 0x00920002:
		INFO("PMU: Error: LP4 rank %d cannot be mapped on tg %d\n", args_list[0], args_list[1]);
		break;
	case 0x00930000:
		INFO("PMU4: Adjusting vrefDac0 for just 1->x transitions\n");
		break;
	case 0x00940000:
		INFO("PMU4: Adjusting vrefDac1 for just 0->x transitions\n");
		break;
	case 0x00950001:
		INFO("PMU5: Strong 1, pull-up csr_DrvStrenFSDqP %#X\n", args_list[0]);
		break;
	case 0x00960001:
		INFO("PMU5: Strong 0, pull-down csr_DrvStrenFSDqN %#X\n", args_list[0]);
		break;
	case 0x00970000:
		INFO("PMU4: Enabling weak drive strengths (FFE)\n");
		break;
	case 0x00980000:
		INFO("PMU5: Changing all weak driver strengths\n");
		break;
	case 0x00990000:
		INFO("PMU5: Finalizing weak drive strengths\n");
		break;
	case 0x009a0000:
		INFO("PMU5: Disabling weak drive strengths (FFE) since no improvement\n");
		break;
	case 0x009b0000:
		INFO("PMU4: Retraining with optimal drive strength settings\n");
		break;
	case 0x009c0002:
		INFO("PMU0: targetting CsX = %d and CsY = %d\n", args_list[0], args_list[1]);
		break;
	case 0x009d0001:
		INFO("PMU1:prbsGenCtl:%x\n", args_list[0]);
		break;
	case 0x009e0000:
		INFO("PMU1: loading 2D acsm sequence\n");
		break;
	case 0x009f0000:
		INFO("PMU1: loading 1D acsm sequence\n");
		break;
	case 0x00a00002:
		INFO("PMU3: %d memclocks @ %d to get half of 300ns\n", args_list[0], args_list[1]);
		break;
	case 0x00a10000:
		INFO("PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n");
		break;
	case 0x00a20000:
		INFO("PMU3: Running 1D search for left eye edge\n");
		break;
	case 0x00a30001:
		INFO("PMU1: In Phase Left Edge Search cs %d\n", args_list[0]);
		break;
	case 0x00a40001:
		INFO("PMU1: Out of Phase Left Edge Search cs %d\n", args_list[0]);
		break;
	case 0x00a50000:
		INFO("PMU3: Running 1D search for right eye edge\n");
		break;
	case 0x00a60001:
		INFO("PMU1: In Phase Right Edge Search cs %d\n", args_list[0]);
		break;
	case 0x00a70001:
		INFO("PMU1: Out of Phase Right Edge Search cs %d\n", args_list[0]);
		break;
	case 0x00a80001:
		INFO("PMU1: mxRdLat training pstate %d\n", args_list[0]);
		break;
	case 0x00a90001:
		INFO("PMU1: mxRdLat search for cs %d\n", args_list[0]);
		break;
	case 0x00aa0001:
		INFO("PMU0: MaxRdLat non consistant DtsmLoThldXingInd 0x%x\n", args_list[0]);
		break;
	case 0x00ab0003:
		INFO("PMU4: CS %d Dbyte %d worked with DFIMRL = %d DFICLKs\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00ac0004:
		INFO("PMU3: MaxRdLat Read Lane err mask for csn %d, DFIMRL %d DFIClks, dbyte %d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00ad0003:
		INFO("PMU3: MaxRdLat Read Lane err mask for csn %d DFIMRL %d, All dbytes = 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00ae0001:
		INFO("PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n", args_list[0]);
		break;
	case 0x00af0002:
		INFO("PMU3: Smallest passing DFIMRL for all dbytes in CS%d = %d DFIClks\n", args_list[0], args_list[1]);
		break;
	case 0x00b00000:
		INFO("PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n");
		break;
	case 0x00b10003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00b20006:
		INFO("PMU4: Adjusting rxclkdly db %d nib %d from %d+%d=%d->%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00b30000:
		INFO("PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x00b40005:
		INFO("PMU4: DB %d Lane %d: %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00b50002:
		INFO("PMU2: TXDQ delayLeft[%d] = %d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00b60004:
		INFO("PMU2: TXDQ delayLeft[%d] = %d oopScaled = %d selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00b70002:
		INFO("PMU2: TXDQ delayRight[%d] = %d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00b80004:
		INFO("PMU2: TXDQ delayRight[%d] = %d oopScaled = %d selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00b90003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00ba0000:
		INFO("PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x00bb0002:
		INFO("PMU4: DB %d Lane %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00bc0005:
		INFO("PMU4: DB %d Lane %d: %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00bd0002:
		INFO("PMU3: Running 1D search csn %d for DM Right/NotLeft(%d) eye edge\n", args_list[0], args_list[1]);
		break;
	case 0x00be0002:
		INFO("PMU3: WrDq DM byte%d with Errcnt %d\n", args_list[0], args_list[1]);
		break;
	case 0x00bf0002:
		INFO("PMU3: WrDq DM byte%d avgDly 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00c00002:
		INFO("PMU1: WrDq DM byte%d with Errcnt %d\n", args_list[0], args_list[1]);
		break;
	case 0x00c10001:
		INFO("PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n", args_list[0]);
		break;
	case 0x00c20000:
		INFO("PMU4: DM TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x00c30002:
		INFO("PMU4: DB %d Lane %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00c40005:
		INFO("PMU4: DB %d Lane %d: %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00c50003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00c60004:
		INFO("PMU3: Errcnt for MRD/MWD search nib %d delay = (%d, 0x%x) = %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00c70000:
		INFO("PMU3: Precharge all open banks\n");
		break;
	case 0x00c80002:
		INFO("PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n", args_list[0], args_list[1]);
		break;
	case 0x00c90000:
		INFO("PMU4: MRD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n");
		break;
	case 0x00ca0000:
		INFO("PMU4: MWD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n");
		break;
	case 0x00cb0004:
		INFO("PMU10: Warning: DB %d nibble %d has multiple working coarse positions, %d and %d, choosing the smaller delay\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00cc0003:
		INFO("PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00cd0006:
		INFO("PMU4: DB %d nibble %d: %d, %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00ce0002:
		INFO("PMU1: Start MRD/nMWD %d for csn %d\n", args_list[0], args_list[1]);
		break;
	case 0x00cf0002:
		INFO("PMU2: RXDQS delayLeft[%d] = %d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00d00006:
		INFO("PMU2: RXDQS delayLeft[%d] = %d delayOop[%d] = %d OopScaled %d, selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00d10002:
		INFO("PMU2: RXDQS delayRight[%d] = %d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00d20006:
		INFO("PMU2: RXDQS delayRight[%d] = %d delayOop[%d] = %d OopScaled %d, selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00d30000:
		INFO("PMU4: RxClkDly Passing Regions (EyeLeft EyeRight -> EyeCenter)\n");
		break;
	case 0x00d40002:
		INFO("PMU4: DB %d nibble %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00d50005:
		INFO("PMU4: DB %d nibble %d: %d %d -> %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00d60003:
		INFO("PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00d70002:
		INFO("PMU0: goodbar = %d for RDWR_BLEN %d\n", args_list[0], args_list[1]);
		break;
	case 0x00d80001:
		INFO("PMU3: RxClkDly = %d\n", args_list[0]);
		break;
	case 0x00d90005:
		INFO("PMU0: db %d l %d absLane %d -> bottom %d top %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00da0009:
		INFO("PMU3: BYTE %d - %d %d %d %d %d %d %d %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x00db0002:
		INFO("PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n", args_list[0], args_list[1]);
		break;
	case 0x00dc0004:
		INFO("PMU0: db%d l%d - %d %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00dd0002:
		INFO("PMU0: goodbar = %d for RDWR_BLEN %d\n", args_list[0], args_list[1]);
		break;
	case 0x00de0004:
		INFO("PMU3: db%d l%d saw %d issues at rxClkDly %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00df0003:
		INFO("PMU3: db%d l%d first saw a pass->fail edge at rxClkDly %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e00002:
		INFO("PMU3: lane %d PBD = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00e10003:
		INFO("PMU3: db%d l%d first saw a DBI pass->fail edge at rxClkDly %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e20003:
		INFO("PMU2: db%d l%d already passed rxPBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e30003:
		INFO("PMU0: db%d l%d, PBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e40002:
		INFO("PMU: Error: dbyte %d lane %d failed read deskew\n", args_list[0], args_list[1]);
		break;
	case 0x00e50003:
		INFO("PMU0: db%d l%d, inc PBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e60003:
		INFO("PMU1: Running lane deskew on pstate %d csn %d rdDBIEn %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e70000:
		INFO("PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n");
		break;
	case 0x00e80002:
		INFO("PMU1: AcsmCsMapCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00e90002:
		INFO("PMU1: AcsmCsMapCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x00ea0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D3U Type\n", args_list[0]);
		break;
	case 0x00eb0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D3R Type\n", args_list[0]);
		break;
	case 0x00ec0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D4U Type\n", args_list[0]);
		break;
	case 0x00ed0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D4R Type\n", args_list[0]);
		break;
	case 0x00ee0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%x, but image built for D4LR Type\n", args_list[0]);
		break;
	case 0x00ef0000:
		INFO("PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n");
		break;
	case 0x00f00003:
		INFO("PMU10: PHY TOTALS - NUM_DBYTES %d NUM_NIBBLES %d NUM_ANIBS %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00f10006:
		INFO("PMU10: CSA=0x%x, CSB=0x%x, TSTAGES=0x%x, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR3\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00f20006:
		INFO("PMU10: CSA=0x%x, CSB=0x%x, TSTAGES=0x%x, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR4\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00f30008:
		INFO("PMU10: CS=0x%x, TSTAGES=0x%x, HDTOUT=%d, 2T=%d, MMISC=%d AddrMirror=%d DRAMFreq=%dMT DramType=%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7]);
		break;
	case 0x00f40004:
		INFO("PMU10: Pstate%d MR0=0x%x MR1=0x%x MR2=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00f50008:
		INFO("PMU10: Pstate%d MRS MR0=0x%x MR1=0x%x MR2=0x%x MR3=0x%x MR4=0x%x MR5=0x%x MR6=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7]);
		break;
	case 0x00f60005:
		INFO("PMU10: Pstate%d MRS MR1_A0=0x%x MR2_A0=0x%x MR3_A0=0x%x MR11_A0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00f70000:
		INFO("PMU10: UseBroadcastMR set. All ranks and channels use MRXX_A0 for MR settings.\n");
		break;
	case 0x00f80005:
		INFO("PMU10: Pstate%d MRS MR01_A0=0x%x MR02_A0=0x%x MR03_A0=0x%x MR11_A0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00f90005:
		INFO("PMU10: Pstate%d MRS MR12_A0=0x%x MR13_A0=0x%x MR14_A0=0x%x MR22_A0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00fa0005:
		INFO("PMU10: Pstate%d MRS MR01_A1=0x%x MR02_A1=0x%x MR03_A1=0x%x MR11_A1=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00fb0005:
		INFO("PMU10: Pstate%d MRS MR12_A1=0x%x MR13_A1=0x%x MR14_A1=0x%x MR22_A1=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00fc0005:
		INFO("PMU10: Pstate%d MRS MR01_B0=0x%x MR02_B0=0x%x MR03_B0=0x%x MR11_B0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00fd0005:
		INFO("PMU10: Pstate%d MRS MR12_B0=0x%x MR13_B0=0x%x MR14_B0=0x%x MR22_B0=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00fe0005:
		INFO("PMU10: Pstate%d MRS MR01_B1=0x%x MR02_B1=0x%x MR03_B1=0x%x MR11_B1=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00ff0005:
		INFO("PMU10: Pstate%d MRS MR12_B1=0x%x MR13_B1=0x%x MR14_B1=0x%x MR22_B1=0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x01000002:
		INFO("PMU1: AcsmOdtCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01010002:
		INFO("PMU1: AcsmCsMapCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01020002:
		INFO("PMU1: AcsmCsMapCtrl%d 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01030000:
		INFO("PMU1: HwtCAMode set\n");
		break;
	case 0x01040001:
		INFO("PMU3: DDR4 infinite preamble enter/exit mode %d\n", args_list[0]);
		break;
	case 0x01050002:
		INFO("PMU1: In rxenb_train() csn=%d pstate=%d\n", args_list[0], args_list[1]);
		break;
	case 0x01060000:
		INFO("PMU3: Finding DQS falling edge\n");
		break;
	case 0x01070000:
		INFO("PMU3: Searching for DDR3/LPDDR3/LPDDR4 read preamble\n");
		break;
	case 0x01080009:
		INFO("PMU3: dtsm fails Even Nibbles : %x %x %x %x %x %x %x %x %x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x01090009:
		INFO("PMU3: dtsm fails Odd  Nibbles : %x %x %x %x %x %x %x %x %x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x010a0002:
		INFO("PMU3: Preamble search pass=%d anyfail=%d\n", args_list[0], args_list[1]);
		break;
	case 0x010b0000:
		INFO("PMU: Error: RxEn training preamble not found\n");
		break;
	case 0x010c0000:
		INFO("PMU3: Found DQS pre-amble\n");
		break;
	case 0x010d0001:
		INFO("PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n", args_list[0]);
		break;
	case 0x010e0000:
		INFO("PMU3: RxEn aligning to first rising edge of burst\n");
		break;
	case 0x010f0001:
		INFO("PMU3: Decreasing RxEn delay by %d fine step to allow full capture of reads\n", args_list[0]);
		break;
	case 0x01100001:
		INFO("PMU3: MREP Delay = %d\n", args_list[0]);
		break;
	case 0x01110003:
		INFO("PMU3: Errcnt for MREP nib %d delay = %d is %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01120002:
		INFO("PMU3: MREP nibble %d sampled a 1 at data buffer delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x01130002:
		INFO("PMU3: MREP nibble %d saw a 0 to 1 transition at data buffer delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x01140000:
		INFO("PMU2:  MREP did not find a 0 to 1 transition for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n");
		break;
	case 0x01150002:
		INFO("PMU2:  Rising edge found in alias window, setting rxDly for nibble %d = %d\n", args_list[0], args_list[1]);
		break;
	case 0x01160002:
		INFO("PMU: Error: Failed MREP for nib %d with %d one\n", args_list[0], args_list[1]);
		break;
	case 0x01170003:
		INFO("PMU2:  Rising edge not found in alias window with %d one, leaving rxDly for nibble %d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01180002:
		INFO("PMU3: Training DIMM %d CSn %d\n", args_list[0], args_list[1]);
		break;
	case 0x01190001:
		INFO("PMU3: exitCAtrain_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x011a0001:
		INFO("PMU3: enterCAtrain_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x011b0001:
		INFO("PMU3: CAtrain_switchmsb_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x011c0001:
		INFO("PMU3: CATrain_rdwr_lp3 looking for pattern %x\n", args_list[0]);
		break;
	case 0x011d0000:
		INFO("PMU3: exitCAtrain_lp4\n");
		break;
	case 0x011e0001:
		INFO("PMU3: DEBUG enterCAtrain_lp4 1: cs 0x%x\n", args_list[0]);
		break;
	case 0x011f0001:
		INFO("PMU3: DEBUG enterCAtrain_lp4 3: Put dbyte %d in async mode\n", args_list[0]);
		break;
	case 0x01200000:
		INFO("PMU3: DEBUG enterCAtrain_lp4 5: Send MR13 to turn on CA training\n");
		break;
	case 0x01210003:
		INFO("PMU3: DEBUG enterCAtrain_lp4 7: idx = %d vref = %x mr12 = %x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01220001:
		INFO("PMU3: CATrain_rdwr_lp4 looking for pattern %x\n", args_list[0]);
		break;
	case 0x01230004:
		INFO("PMU3: Phase %d CAreadbackA db:%d %x xo:%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01240005:
		INFO("PMU3: DEBUG lp4SetCatrVref 1: cs=%d chan=%d mr12=%x vref=%d.%d%%\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x01250003:
		INFO("PMU3: DEBUG lp4SetCatrVref 3: mr12 = %x send vref= %x to db=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01260000:
		INFO("PMU10:Optimizing vref\n");
		break;
	case 0x01270004:
		INFO("PMU4:mr12:%x cs:%d chan %d r:%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01280005:
		INFO("PMU3: i:%d bstr:%d bsto:%d st:%d r:%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x01290002:
		INFO("Failed to find sufficient CA Vref Passing Region for CS %d channel %d\n", args_list[0], args_list[1]);
		break;
	case 0x012a0005:
		INFO("PMU3:Found %d.%d%% MR12:%x for cs:%d chan %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x012b0002:
		INFO("PMU3:Calculated %d for AtxImpedence from acx %d.\n", args_list[0], args_list[1]);
		break;
	case 0x012c0000:
		INFO("PMU3:CA Odt impedence ==0.  Use default vref.\n");
		break;
	case 0x012d0003:
		INFO("PMU3:Calculated %d.%d%% for Vref MR12=0x%x.\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x012e0000:
		INFO("PMU3: CAtrain_lp\n");
		break;
	case 0x012f0000:
		INFO("PMU3: CAtrain Begins.\n");
		break;
	case 0x01300001:
		INFO("PMU3: CAtrain_lp testing dly %d\n", args_list[0]);
		break;
	case 0x01310001:
		INFO("PMU5: CA bitmap dump for cs %x\n", args_list[0]);
		break;
	case 0x01320001:
		INFO("PMU5: CAA%d ", args_list[0]);
		break;
	case 0x01330001:
		INFO("%x", args_list[0]);
		break;
	case 0x01340000:
		INFO("\n");
		break;
	case 0x01350001:
		INFO("PMU5: CAB%d ", args_list[0]);
		break;
	case 0x01360001:
		INFO("%x", args_list[0]);
		break;
	case 0x01370000:
		INFO("\n");
		break;
	case 0x01380003:
		INFO("PMU3: anibi=%d, anibichan[anibi]=%d ,chan=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01390001:
		INFO("%x", args_list[0]);
		break;
	case 0x013a0001:
		INFO("\nPMU3:Raw CA setting :%x", args_list[0]);
		break;
	case 0x013b0002:
		INFO("\nPMU3:ATxDly setting:%x margin:%d\n", args_list[0], args_list[1]);
		break;
	case 0x013c0002:
		INFO("\nPMU3:InvClk ATxDly setting:%x margin:%d\n", args_list[0], args_list[1]);
		break;
	case 0x013d0000:
		INFO("\nPMU3:No Range found!\n");
		break;
	case 0x013e0003:
		INFO("PMU3: 2 anibi=%d, anibichan[anibi]=%d ,chan=%d", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x013f0002:
		INFO("\nPMU3: no neg clock => CA setting anib=%d, :%d\n", args_list[0], args_list[1]);
		break;
	case 0x01400001:
		INFO("PMU3:Normal margin:%d\n", args_list[0]);
		break;
	case 0x01410001:
		INFO("PMU3:Inverted margin:%d\n", args_list[0]);
		break;
	case 0x01420000:
		INFO("PMU3:Using Inverted clock\n");
		break;
	case 0x01430000:
		INFO("PMU3:Using normal clk\n");
		break;
	case 0x01440003:
		INFO("PMU3: 3 anibi=%d, anibichan[anibi]=%d ,chan=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01450002:
		INFO("PMU3: Setting ATxDly for anib %x to %x\n", args_list[0], args_list[1]);
		break;
	case 0x01460000:
		INFO("PMU: Error: CA Training Failed.\n");
		break;
	case 0x01470000:
		INFO("PMU1: Writing MRs\n");
		break;
	case 0x01480000:
		INFO("PMU4:Using MR12 values from 1D CA VREF training.\n");
		break;
	case 0x01490000:
		INFO("PMU3:Writing all MRs to fsp 1\n");
		break;
	case 0x014a0000:
		INFO("PMU10:Lp4Quickboot mode.\n");
		break;
	case 0x014b0000:
		INFO("PMU3: Writing MRs\n");
		break;
	case 0x014c0001:
		INFO("PMU10: Setting boot clock divider to %d\n", args_list[0]);
		break;
	case 0x014d0000:
		INFO("PMU3: Resetting DRAM\n");
		break;
	case 0x014e0000:
		INFO("PMU3: setup for RCD initalization\n");
		break;
	case 0x014f0000:
		INFO("PMU3: pmu_exit_SR from dev_init()\n");
		break;
	case 0x01500000:
		INFO("PMU3: initializing RCD\n");
		break;
	case 0x01510000:
		INFO("PMU10: **** Executing 2D Image ****\n");
		break;
	case 0x01520001:
		INFO("PMU10: **** Start DDR4 Training. PMU Firmware Revision 0x%x ****\n", args_list[0]);
		break;
	case 0x01530001:
		INFO("PMU10: **** Start DDR3 Training. PMU Firmware Revision 0x%x ****\n", args_list[0]);
		break;
	case 0x01540001:
		INFO("PMU10: **** Start LPDDR3 Training. PMU Firmware Revision 0x%x ****\n", args_list[0]);
		break;
	case 0x01550001:
		INFO("PMU10: **** Start LPDDR4 Training. PMU Firmware Revision 0x%x ****\n", args_list[0]);
		break;
	case 0x01560000:
		INFO("PMU: Error: Mismatched internal revision between DCCM and ICCM images\n");
		break;
	case 0x01570001:
		INFO("PMU10: **** Testchip %d Specific Firmware ****\n", args_list[0]);
		break;
	case 0x01580000:
		INFO("PMU1: LRDIMM with EncodedCS mode, one DIMM\n");
		break;
	case 0x01590000:
		INFO("PMU1: LRDIMM with EncodedCS mode, two DIMMs\n");
		break;
	case 0x015a0000:
		INFO("PMU1: RDIMM with EncodedCS mode, one DIMM\n");
		break;
	case 0x015b0000:
		INFO("PMU2: Starting LRDIMM MREP training for all ranks\n");
		break;
	case 0x015c0000:
		INFO("PMU199: LRDIMM MREP training for all ranks completed\n");
		break;
	case 0x015d0000:
		INFO("PMU2: Starting LRDIMM DWL training for all ranks\n");
		break;
	case 0x015e0000:
		INFO("PMU199: LRDIMM DWL training for all ranks completed\n");
		break;
	case 0x015f0000:
		INFO("PMU2: Starting LRDIMM MRD training for all ranks\n");
		break;
	case 0x01600000:
		INFO("PMU199: LRDIMM MRD training for all ranks completed\n");
		break;
	case 0x01610000:
		INFO("PMU2: Starting RXEN training for all ranks\n");
		break;
	case 0x01620000:
		INFO("PMU2: Starting write leveling fine delay training for all ranks\n");
		break;
	case 0x01630000:
		INFO("PMU2: Starting LRDIMM MWD training for all ranks\n");
		break;
	case 0x01640000:
		INFO("PMU199: LRDIMM MWD training for all ranks completed\n");
		break;
	case 0x01650000:
		INFO("PMU2: Starting write leveling fine delay training for all ranks\n");
		break;
	case 0x01660000:
		INFO("PMU2: Starting read deskew training\n");
		break;
	case 0x01670000:
		INFO("PMU2: Starting SI friendly 1d RdDqs training for all ranks\n");
		break;
	case 0x01680000:
		INFO("PMU2: Starting write leveling coarse delay training for all ranks\n");
		break;
	case 0x01690000:
		INFO("PMU2: Starting 1d WrDq training for all ranks\n");
		break;
	case 0x016a0000:
		INFO("PMU2: Running DQS2DQ Oscillator for all ranks\n");
		break;
	case 0x016b0000:
		INFO("PMU2: Starting again read deskew training but with PRBS\n");
		break;
	case 0x016c0000:
		INFO("PMU2: Starting 1d RdDqs training for all ranks\n");
		break;
	case 0x016d0000:
		INFO("PMU2: Starting again 1d WrDq training for all ranks\n");
		break;
	case 0x016e0000:
		INFO("PMU2: Starting MaxRdLat training\n");
		break;
	case 0x016f0000:
		INFO("PMU2: Starting 2d RdDqs training for all ranks\n");
		break;
	case 0x01700000:
		INFO("PMU2: Starting 2d WrDq training for all ranks\n");
		break;
	case 0x01710000:
		INFO("PMU2: Starting 2d RdDqs training for all ranks\n");
		break;
	case 0x01720002:
		INFO("PMU3:read_fifo %x %x\n", args_list[0], args_list[1]);
		break;
	case 0x01730001:
		INFO("PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x01740001:
		INFO("PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x01750001:
		INFO("PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x01760005:
		INFO("PMU3: fixRxEnBackOff csn:%d db:%d dn:%d bo:%d dly:%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x01770001:
		INFO("PMU3: fixRxEnBackOff dly:%x\n", args_list[0]);
		break;
	case 0x01780000:
		INFO("PMU3: Entering setupPpt\n");
		break;
	case 0x01790000:
		INFO("PMU3: Start lp4PopulateHighLowBytes\n");
		break;
	case 0x017a0002:
		INFO("PMU3:Dbyte Detect: db%d received %x\n", args_list[0], args_list[1]);
		break;
	case 0x017b0002:
		INFO("PMU3:getDqs2Dq read %x from dbyte %d\n", args_list[0], args_list[1]);
		break;
	case 0x017c0002:
		INFO("PMU3:getDqs2Dq(2) read %x from dbyte %d\n", args_list[0], args_list[1]);
		break;
	case 0x017d0001:
		INFO("PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n", args_list[0]);
		break;
	case 0x017e0002:
		INFO("PMU4: Dbyte %d dqs2dq = %d/32 UI\n", args_list[0], args_list[1]);
		break;
	case 0x017f0003:
		INFO("PMU3:getDqs2Dq set dqs2dq:%d/32 ui (%d ps) from dbyte %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01800003:
		INFO("PMU3: Setting coarse delay in AtxDly chiplet %d from 0x%x to 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01810003:
		INFO("PMU3: Clearing coarse delay in AtxDly chiplet %d from 0x%x to 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01820000:
		INFO("PMU3: Performing DDR4 geardown sync sequence\n");
		break;
	case 0x01830000:
		INFO("PMU1: Enter self refresh\n");
		break;
	case 0x01840000:
		INFO("PMU1: Exit self refresh\n");
		break;
	case 0x01850000:
		INFO("PMU: Error: No dbiEnable with lp4\n");
		break;
	case 0x01860000:
		INFO("PMU: Error: No dbiDisable with lp4\n");
		break;
	case 0x01870001:
		INFO("PMU1: DDR4 update Rx DBI Setting disable %d\n", args_list[0]);
		break;
	case 0x01880001:
		INFO("PMU1: DDR4 update 2nCk WPre Setting disable %d\n", args_list[0]);
		break;
	case 0x01890005:
		INFO("PMU1: read_delay: db%d lane%d delays[%d] = 0x%x (max 0x%x)\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x018a0004:
		INFO("PMU1: write_delay: db%d lane%d delays[%d] = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x018b0001:
		INFO("PMU5: ID=%d -- db0  db1  db2  db3  db4  db5  db6  db7  db8  db9 --\n", args_list[0]);
		break;
	case 0x018c000b:
		INFO("PMU5: [%d]:0x %x %x %x %x %x %x %x %x %x %x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8], args_list[9], args_list[10]);
		break;
	case 0x018d0003:
		INFO("PMU2: dump delays - pstate=%d dimm=%d csn=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x018e0000:
		INFO("PMU3: Printing Mid-Training Delay Information\n");
		break;
	case 0x018f0001:
		INFO("PMU5: CS%d <<KEY>> 0 TrainingCntr <<KEY>> coarse(15:10) fine(9:0)\n", args_list[0]);
		break;
	case 0x01900001:
		INFO("PMU5: CS%d <<KEY>> 0 RxEnDly, 1 RxClkDly <<KEY>> coarse(10:6) fine(5:0)\n", args_list[0]);
		break;
	case 0x01910001:
		INFO("PMU5: CS%d <<KEY>> 0 TxDqsDly, 1 TxDqDly <<KEY>> coarse(9:6) fine(5:0)\n", args_list[0]);
		break;
	case 0x01920001:
		INFO("PMU5: CS%d <<KEY>> 0 RxPBDly <<KEY>> 1 Delay Unit ~= 7ps\n", args_list[0]);
		break;
	case 0x01930000:
		INFO("PMU5: all CS <<KEY>> 0 DFIMRL <<KEY>> Units = DFI clocks\n");
		break;
	case 0x01940000:
		INFO("PMU5: all CS <<KEY>> VrefDACs <<KEY>> DAC(6:0)\n");
		break;
	case 0x01950000:
		INFO("PMU1: Set DMD in MR13 and wrDBI in MR3 for training\n");
		break;
	case 0x01960000:
		INFO("PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n");
		break;
	case 0x01970003:
		INFO("PMU2: getMaxRxen(): maxDly %d maxTg %d maxNib %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01980003:
		INFO("PMU2: getRankMaxRxen(): maxDly %d Tg %d maxNib %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01990000:
		INFO("PMU1: skipping CDD calculation in 2D image\n");
		break;
	case 0x019a0001:
		INFO("PMU3: Calculating CDDs for pstate %d\n", args_list[0]);
		break;
	case 0x019b0003:
		INFO("PMU3: rxFromDly[%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x019c0003:
		INFO("PMU3: rxToDly  [%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x019d0003:
		INFO("PMU3: rxDly    [%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x019e0003:
		INFO("PMU3: txDly    [%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x019f0003:
		INFO("PMU3: allFine CDD_RR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01a00003:
		INFO("PMU3: allFine CDD_WW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01a10003:
		INFO("PMU3: CDD_RR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01a20003:
		INFO("PMU3: CDD_WW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01a30003:
		INFO("PMU3: allFine CDD_RW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01a40003:
		INFO("PMU3: allFine CDD_WR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01a50003:
		INFO("PMU3: CDD_RW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01a60003:
		INFO("PMU3: CDD_WR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01a70004:
		INFO("PMU3: F%dBC2x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01a80004:
		INFO("PMU3: F%dBC3x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01a90004:
		INFO("PMU3: F%dBC4x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01aa0004:
		INFO("PMU3: F%dBC5x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01ab0004:
		INFO("PMU3: F%dBC8x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01ac0004:
		INFO("PMU3: F%dBC9x_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01ad0004:
		INFO("PMU3: F%dBCAx_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01ae0004:
		INFO("PMU3: F%dBCBx_B%d_D%d = 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01af0000:
		INFO("PMU10: Entering context_switch_postamble\n");
		break;
	case 0x01b00003:
		INFO("PMU10: context_switch_postamble is enabled for DIMM %d, RC0A=0x%x, RC3x=0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01b10000:
		INFO("PMU10: Setting bcw fspace 0\n");
		break;
	case 0x01b20001:
		INFO("PMU10: Sending BC0A = 0x%x\n", args_list[0]);
		break;
	case 0x01b30001:
		INFO("PMU10: Sending BC6x = 0x%x\n", args_list[0]);
		break;
	case 0x01b40001:
		INFO("PMU10: Sending RC0A = 0x%x\n", args_list[0]);
		break;
	case 0x01b50001:
		INFO("PMU10: Sending RC3x = 0x%x\n", args_list[0]);
		break;
	case 0x01b60001:
		INFO("PMU10: Sending RC0A = 0x%x\n", args_list[0]);
		break;
	case 0x01b70001:
		INFO("PMU1: enter_lp3: DEBUG: pstate = %d\n", args_list[0]);
		break;
	case 0x01b80001:
		INFO("PMU1: enter_lp3: DEBUG: dfifreqxlat_pstate = %d\n", args_list[0]);
		break;
	case 0x01b90001:
		INFO("PMU1: enter_lp3: DEBUG: pllbypass = %d\n", args_list[0]);
		break;
	case 0x01ba0001:
		INFO("PMU1: enter_lp3: DEBUG: forcecal = %d\n", args_list[0]);
		break;
	case 0x01bb0001:
		INFO("PMU1: enter_lp3: DEBUG: pllmaxrange = 0x%x\n", args_list[0]);
		break;
	case 0x01bc0001:
		INFO("PMU1: enter_lp3: DEBUG: dacval_out = 0x%x\n", args_list[0]);
		break;
	case 0x01bd0001:
		INFO("PMU1: enter_lp3: DEBUG: pllctrl3 = 0x%x\n", args_list[0]);
		break;
	case 0x01be0000:
		INFO("PMU3: Loading DRAM with BIOS supplied MR values and entering self refresh prior to exiting PMU code.\n");
		break;
	case 0x01bf0002:
		INFO("PMU3: Setting DataBuffer function space of dimmcs 0x%x to %d\n", args_list[0], args_list[1]);
		break;
	case 0x01c00002:
		INFO("PMU4: Setting RCW FxRC%xx = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01c10002:
		INFO("PMU4: Setting RCW FxRC%x = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01c20001:
		INFO("PMU1: DDR4 update Rd Pre Setting disable %d\n", args_list[0]);
		break;
	case 0x01c30002:
		INFO("PMU2: Setting BCW FxBC%xx = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01c40002:
		INFO("PMU2: Setting BCW BC%x = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01c50002:
		INFO("PMU2: Setting BCW PBA mode FxBC%xx = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01c60002:
		INFO("PMU2: Setting BCW PBA mode BC%x = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01c70003:
		INFO("PMU4: BCW value for dimm %d, fspace %d, addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01c80002:
		INFO("PMU4: DB %d, value 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01c90000:
		INFO("PMU6: WARNING MREP underflow, set to min value -2 coarse, 0 fine\n");
		break;
	case 0x01ca0004:
		INFO("PMU6: LRDIMM Writing final data buffer fine delay value nib %d, trainDly %d, fineDly code %d, new MREP fine %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01cb0003:
		INFO("PMU6: LRDIMM Writing final data buffer fine delay value nib %d, trainDly %d, fineDly code %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01cc0003:
		INFO("PMU6: LRDIMM Writing data buffer fine delay type %d nib %d, code %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01cd0002:
		INFO("PMU6: Writing final data buffer coarse delay value dbyte %d, coarse = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01ce0003:
		INFO("PMU4: data 0x%x at MB addr 0x%x saved at CSR addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01cf0003:
		INFO("PMU4: data 0x%x at MB addr 0x%x restored from CSR addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01d00003:
		INFO("PMU4: data 0x%x at MB addr 0x%x saved at CSR addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01d10003:
		INFO("PMU4: data 0x%x at MB addr 0x%x restored from CSR addr 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01d20001:
		INFO("PMU3: Update BC00, BC01, BC02 for rank-dimm 0x%x\n", args_list[0]);
		break;
	case 0x01d30000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F0RC00 -> F0RC0F\n");
		break;
	case 0x01d40000:
		INFO("PMU3: Disable parity in F0RC0E\n");
		break;
	case 0x01d50000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F1RC00 -> F1RC05\n");
		break;
	case 0x01d60000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F1RC1x -> F1RC9x\n");
		break;
	case 0x01d70000:
		INFO("PMU3: Writing D4 Data buffer Control words BC00 -> BC0E\n");
		break;
	case 0x01d80002:
		INFO("PMU1: setAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x01d90002:
		INFO("PMU1: restoreFromAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x01da0002:
		INFO("PMU1: restoreAcsmFromAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x01db0002:
		INFO("PMU2: Setting D3R RC%d = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01dc0000:
		INFO("PMU3: Writing D3 RDIMM RCD Control words RC0 -> RC11\n");
		break;
	case 0x01dd0002:
		INFO("PMU0: VrefDAC0/1 vddqStart %d dacToVddq %d\n", args_list[0], args_list[1]);
		break;
	case 0x01de0001:
		INFO("PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n", args_list[0]);
		break;
	case 0x01df0001:
		INFO("PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n", args_list[0]);
		break;
	case 0x01e00001:
		INFO("PMU0: PHY VREF @ (%d/1000) VDDQ\n", args_list[0]);
		break;
	case 0x01e10002:
		INFO("PMU0: initalizing phy vrefDacs to %d ExtVrefRange %x\n", args_list[0], args_list[1]);
		break;
	case 0x01e20002:
		INFO("PMU0: initalizing global vref to %d range %d\n", args_list[0], args_list[1]);
		break;
	case 0x01e30002:
		INFO("PMU4: Setting initial device vrefDQ for CS%d to MR6 = 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01e40003:
		INFO("PMU1: In write_level_fine() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01e50000:
		INFO("PMU3: Fine write leveling hardware search increasing TxDqsDly until full bursts are seen\n");
		break;
	case 0x01e60000:
		INFO("PMU4: WL normalized pos   : ........................|........................\n");
		break;
	case 0x01e70007:
		INFO("PMU4: WL margin for nib %d: %x%x%x%x%x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6]);
		break;
	case 0x01e80000:
		INFO("PMU4: WL normalized pos   : ........................|........................\n");
		break;
	case 0x01e90000:
		INFO("PMU3: Exiting write leveling mode\n");
		break;
	case 0x01ea0001:
		INFO("PMU3: got %d for cl in load_wrlvl_acsm\n", args_list[0]);
		break;
	case 0x01eb0003:
		INFO("PMU1: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01ec0003:
		INFO("PMU3: left eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01ed0003:
		INFO("PMU3: right eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01ee0004:
		INFO("PMU3: eye center db:%d ln:%d dly:0x%x (maxdq:%x)\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01ef0003:
		INFO("PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01f00003:
		INFO("PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01f10002:
		INFO("PMU3: Coarse write leveling dbyte%d is still failing for TxDqsDly=0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01f20002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x01f30000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x01f40001:
		INFO("PMU3: got %d for cl in load_wrlvl_acsm\n", args_list[0]);
		break;
	case 0x01f50003:
		INFO("PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01f60003:
		INFO("PMU3: left eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01f70003:
		INFO("PMU3: right eye edge search db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01f80004:
		INFO("PMU3: eye center db: %d ln: %d dly: 0x%x (maxdq: 0x%x)\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01f90003:
		INFO("PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01fa0003:
		INFO("PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01fb0002:
		INFO("PMU3: Coarse write leveling nibble%d is still failing for TxDqsDly=0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x01fc0002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x01fd0000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x01fe0000:
		INFO("PMU4: WL normalized pos   : ................................|................................\n");
		break;
	case 0x01ff0009:
		INFO("PMU4: WL margin for nib %d: %x%x%x%x%x%x%x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x02000000:
		INFO("PMU4: WL normalized pos   : ................................|................................\n");
		break;
	case 0x02010001:
		INFO("PMU8: Adjust margin after WL coarse to be larger than %d\n", args_list[0]);
		break;
	case 0x02020001:
		INFO("PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n", args_list[0]);
		break;
	case 0x02030002:
		INFO("PMU8: Decrement nib %d TxDqsDly by %d fine step\n", args_list[0], args_list[1]);
		break;
	case 0x02040003:
		INFO("PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x02050005:
		INFO("PMU2: Write level: dbyte %d nib%d dq/dmbi %d dqsfine 0x%x dqDly 0x%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x02060002:
		INFO("PMU3: Coarse write leveling nibble%d is still failing for TxDqsDly=0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x02070002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x02080000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x02090001:
		INFO("PMU3: DWL delay = %d\n", args_list[0]);
		break;
	case 0x020a0003:
		INFO("PMU3: Errcnt for DWL nib %d delay = %d is %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x020b0002:
		INFO("PMU3: DWL nibble %d sampled a 1 at delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x020c0003:
		INFO("PMU3: DWL nibble %d passed at delay %d. Rising edge was at %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x020d0000:
		INFO("PMU2: DWL did nto find a rising edge of memclk for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n");
		break;
	case 0x020e0002:
		INFO("PMU2:  Rising edge found in alias window, setting wrlvlDly for nibble %d = %d\n", args_list[0], args_list[1]);
		break;
	case 0x020f0002:
		INFO("PMU: Error: Failed DWL for nib %d with %d one\n", args_list[0], args_list[1]);
		break;
	case 0x02100003:
		INFO("PMU2:  Rising edge not found in alias window with %d one, leaving wrlvlDly for nibble %d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04000000:
		INFO("PMU: Error:Mailbox Buffer Overflowed.\n");
		break;
	case 0x04010000:
		INFO("PMU: Error:Mailbox Buffer Overflowed.\n");
		break;
	case 0x04020000:
		INFO("PMU: ***** Assertion Error - terminating *****\n");
		break;
	case 0x04030002:
		INFO("PMU1: swapByte db %d by %d\n", args_list[0], args_list[1]);
		break;
	case 0x04040003:
		INFO("PMU3: get_cmd_dly max(%d ps, %d memclk) = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04050002:
		INFO("PMU0: Write CSR 0x%x 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x04060002:
		INFO("PMU0: hwt_init_ppgc_prbs(): Polynomial: %x, Deg: %d\n", args_list[0], args_list[1]);
		break;
	case 0x04070001:
		INFO("PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n", args_list[0]);
		break;
	case 0x04080001:
		INFO("PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n", args_list[0]);
		break;
	case 0x0409000c:
		INFO("PMU1: acsm_addr %x, acsm_flgs %x, ddr_cmd %x, cmd_dly %x, ddr_addr %x, ddr_bnk %x, ddr_cs %x, cmd_rcnt %x, AcsmSeq0/1/2/3 %x %x %x %x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8], args_list[9], args_list[10], args_list[11]);
		break;
	case 0x040a0000:
		INFO("PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n");
		break;
	case 0x040b0000:
		INFO("PMU1: acsm RUN\n");
		break;
	case 0x040c0000:
		INFO("PMU1: acsm STOPPED\n");
		break;
	case 0x040d0002:
		INFO("PMU1: acsm_init: acsm_mode %x mxrdlat %x\n", args_list[0], args_list[1]);
		break;
	case 0x040e0002:
		INFO("PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n", args_list[0], args_list[1]);
		break;
	case 0x040f0002:
		INFO("PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n", args_list[0], args_list[1]);
		break;
	case 0x04100002:
		INFO("PMU1: setAcsmCLCWL: CASL %d WCASL %d\n", args_list[0], args_list[1]);
		break;
	case 0x04110001:
		INFO("PMU: Error: Reserved value of register F0RC0F found in message block: 0x%x\n", args_list[0]);
		break;
	case 0x04120001:
		INFO("PMU3: Written MRS to CS=0x%x\n", args_list[0]);
		break;
	case 0x04130001:
		INFO("PMU3: Written MRS to CS=0x%x\n", args_list[0]);
		break;
	case 0x04140000:
		INFO("PMU3: Entering Boot Freq Mode.\n");
		break;
	case 0x04150001:
		INFO("PMU: Error: Boot clock divider setting of %d is too small\n", args_list[0]);
		break;
	case 0x04160000:
		INFO("PMU3: Exiting Boot Freq Mode.\n");
		break;
	case 0x04170002:
		INFO("PMU3: Writing MR%d OP=%x\n", args_list[0], args_list[1]);
		break;
	case 0x04180000:
		INFO("PMU: Error: Delay too large in slomo\n");
		break;
	case 0x04190001:
		INFO("PMU3: Written MRS to CS=0x%x\n", args_list[0]);
		break;
	case 0x041a0000:
		INFO("PMU3: Enable Channel A\n");
		break;
	case 0x041b0000:
		INFO("PMU3: Enable Channel B\n");
		break;
	case 0x041c0000:
		INFO("PMU3: Enable All Channels\n");
		break;
	case 0x041d0002:
		INFO("PMU2: Use PDA mode to set MR%d with value 0x%x\n", args_list[0], args_list[1]);
		break;
	case 0x041e0001:
		INFO("PMU3: Written Vref with PDA to CS=0x%x\n", args_list[0]);
		break;
	case 0x041f0000:
		INFO("PMU1: start_cal: DEBUG: setting CalRun to 1\n");
		break;
	case 0x04200000:
		INFO("PMU1: start_cal: DEBUG: setting CalRun to 0\n");
		break;
	case 0x04210001:
		INFO("PMU1: lock_pll_dll: DEBUG: pstate = %d\n", args_list[0]);
		break;
	case 0x04220001:
		INFO("PMU1: lock_pll_dll: DEBUG: dfifreqxlat_pstate = %d\n", args_list[0]);
		break;
	case 0x04230001:
		INFO("PMU1: lock_pll_dll: DEBUG: pllbypass = %d\n", args_list[0]);
		break;
	case 0x04240001:
		INFO("PMU3: SaveLcdlSeed: Saving seed %d\n", args_list[0]);
		break;
	case 0x04250000:
		INFO("PMU1: in phy_defaults()\n");
		break;
	case 0x04260003:
		INFO("PMU3: ACXConf:%d MaxNumDbytes:%d NumDfi:%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04270005:
		INFO("PMU1: setAltAcsmCLCWL setting cl=%d cwl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x014c0000:
		INFO("PMU3: Exiting write leveling mode\n");
		break;
	default:
		INFO("PMU Streaming Msg: Debug message not recognized !!  code: %x", codede_message_hex);
		panic();
	}
#endif
}
