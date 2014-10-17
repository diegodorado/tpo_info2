
/* MMC/SD command */

#define CMD0	(0)			// GO_IDLE_STATE
#define CMD1	(1)			// SEND_OP_COND (MMC)
#define	ACMD41	(0x80+41)	// SEND_OP_COND (SDC)
#define CMD8	(8)			// SEND_IF_COND
#define CMD9	(9)			// SEND_CSD
#define CMD10	(10)		// SEND_CID
#define CMD12	(12)		// STOP_TRANSMISSION
#define ACMD13	(0x80+13)	//SD_STATUS (SDC)
#define CMD16	(16)		//SET_BLOCKLEN
#define CMD17	(17)		// READ_SINGLE_BLOCK
#define CMD18	(18)		// READ_MULTIPLE_BLOCK
#define CMD23	(23)		// SET_BLOCK_COUNT (MMC)
#define	ACMD23	(0x80+23)	// SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24	(24)		// WRITE_BLOCK
#define CMD25	(25)		// WRITE_MULTIPLE_BLOCK
#define CMD32	(32)		// ERASE_ER_BLK_START
#define CMD33	(33)		// ERASE_ER_BLK_END
#define CMD38	(38)		// ERASE
#define CMD55	(55)		// APP_CMD
#define CMD58	(58)		// READ_OCR

/* Card type flags (CardType) */

#define CT_MMC		0x01		// MMC
#define CT_SD1		0x02
#define CT_SD2		0x04
#define CT_SDC		(CT_SD1|CT_SD2)
#define CT_BLOCK	0x08		// Block addressing

#define	CMDREAD		17
#define	CMDWRITE	24
#define	CMDREADCSD       9

#define R1_NO_ERROR         0x00
#define R1_IN_IDLE_STATE    0x01
#define R1_ERASE_RESET      0x02
#define R1_ILLEGAL_CMD      0x04
#define R1_COM_CRC_ERROR    0x08
#define R1_ERASE_SEQ_ERROR  0x10
#define R1_ADDRESS_ERROR    0x20
#define R1_PARA_ERROR       0x40
#define R1_MASK             0x7F

