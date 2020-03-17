#include "project_config.h"

#include "pack.h"
//#include "ymodem.h"
#include "stm32_eval_spi_flash.h"

//#define UNPACK_DIR		"./hicore"
//#define FLASH_SECTOR_SIZE	(64*1024)
/* 按字节计算校验和
	return: 32位的校验和
*/
unsigned int check_byte_sum(char *pdata, int len)
{
	unsigned int sum = 0;
	int i;

	for(i=0; i<len; i++)
	{
		sum += ((unsigned int)pdata[i]) & 0xff;
	}

	return sum;
}

/* convertData:利用简单的异或进行数据变换，用于升级文件的打包和解包
	input:	src -- source data
		len -- length of source data
	output：dst -- destination to put output data
	return：0 -- OK.
		1 -- Failed.
*/
int convertData(char *src, char *dst, int len)
{
	/* 固定的幻数，用于异或变换 */
	char magic[] = {0xba, 0xcd, 0xbc, 0xfe, 0xd6, 0xca, 0xdd, 0xd3,
					0xba, 0xb9, 0xa3, 0xab, 0xbf, 0xcb, 0xb5, 0xbe};
	int i, j;
	int magiclen, startmagic;

	if(src==NULL || dst==NULL)
	{
		printf("Invalid input param: src = %p, dst = %p\n", src, dst);
		return -1;
	}

	magiclen = sizeof(magic);
	for(i=0, startmagic=0; i<len; startmagic=(startmagic+1)%magiclen)
	{
		/* 用startmagic控制每次内循环magic的起始位置 */
		for(j=0; j<magiclen && i<len; j++, i++)
		{
			//*dst++ = *src++ ^ magic[(startmagic+j)%magiclen];	/* 进行异或变换 */
			*dst = (*src)^ magic[(startmagic+j)%magiclen];	/* 进行异或变换 */
	//		printf("src[%d]= 0x%02x ,magic[%d] *dst= 0x%02x\r\n",j, *src&0xff,(startmagic+j)%magiclen,*dst&0xff);
	//		printf("src[%d]= 0x%02x ",i, *src&0xff);
	//		printf("dst[%d]= 0x%02x ",i, *dst&0xff);
			dst++;
			src++;
		}
	}
	return 0;
}

//UPGRADE_FILE_HEADER *uheader;
FIRMWARE_HEADER *dst_pheader =NULL;

int main_parse_flash_pcm_info(void)
{
	//	char *pbuf;
	int i;
	//int	j, m, n;
	//	int fdout, fdin; 
    UPGRADE_FILE_HEADER *uheader;
	int fheader_len, header_len, header_checksum;
	//int checksum ;
	//FIRMWARE_HEADER *src_fheader, *dst_fheader, *src_pheader, *dst_pheader;
	FIRMWARE_HEADER *src_fheader, *dst_fheader, *src_pheader;
	


	fheader_len = sizeof(FIRMWARE_HEADER);
	printf("fheader_len=%d\r\n",fheader_len);
	printf("sizeof(UPGRADE_FILE_HEADER)=%d\r\n",sizeof(UPGRADE_FILE_HEADER));
	src_fheader = (FIRMWARE_HEADER *)malloc(fheader_len);
	if(src_fheader == NULL)
	{
		printf("upgrade 1 failed!\n");
		return -1;
	}
	memset((char *)src_fheader, 0, fheader_len);

	//if((dst_fheader = (FIRMWARE_HEADER *)malloc(fheader_len)) == NULL)
	dst_fheader = (FIRMWARE_HEADER *)malloc(fheader_len);
	if(dst_fheader == NULL)
	{
		printf("upgrade 2 failed!\n");
		return -1;
	}
	memset((char *)dst_fheader, 0, fheader_len);
	#if 1
   /* Read data from SPI FLASH memory */
    sFLASH_ReadBuffer((uint8_t*)src_fheader, FLASH_PCM_DATA_ADDRESS, fheader_len);
	//printf("magic_number=0x%x\r\n",src_fheader->magic_number);
	#else
	/*open the unpack file*/
	if((fdin = open(argv[1], O_RDONLY)) == -1)
	{
		printf("Can't open %s\n",argv[1]);
		printf("upgrade failed!\n");
		return  -1;
	}
	
	lseek(fdin, 0, SEEK_SET);
	read(fdin, (char *)src_fheader, fheader_len);
	#endif
	convertData((char *)src_fheader, (char *)dst_fheader, fheader_len);
	if(dst_fheader->file_nums == 0 || dst_fheader->file_nums > MAX_FILE_NUM)
	{
		printf("upgrade 3 failed!\n");
		while(1);
		return -1;
	}
	
	//printf("h=%d h2=%d\n",sizeof(FIRMWARE_HEADER),sizeof(FIRMWARE_HEADER2));
	header_len = fheader_len + dst_fheader->file_nums * sizeof(UPGRADE_FILE_HEADER);
	//header_len = fheader_len;
	printf("dst_fheader->magic_number=0x%x\r\n",dst_fheader->magic_number);
	printf("dst_fheader->file_nums=%d\r\n",dst_fheader->file_nums);
	printf("fheader_len=%d +file_nums=%d *UPGRADE_FILE_HEADER=%d = header_len=%d\r\n",fheader_len,dst_fheader->file_nums,sizeof(UPGRADE_FILE_HEADER),header_len);
	free(src_fheader);
	free(dst_fheader);

	if((src_pheader = (FIRMWARE_HEADER *)malloc(header_len)) == NULL)
	{
		printf("upgrade 4 failed!\n");
		return -1;
	}
	memset((char *)src_pheader, 0, header_len);

	if((dst_pheader = (FIRMWARE_HEADER *)malloc(header_len)) == NULL)
	{
		printf("upgrade 5 failed!\n");
		return -1;
	}
	memset((char *)dst_pheader, 0, header_len);

	#if 1
   /* Read data from SPI FLASH memory */
//#define  FLASH_WriteAddress     0x000000
//#define  FLASH_ReadAddress      FLASH_WriteAddress

    sFLASH_ReadBuffer((uint8_t*)src_pheader, FLASH_PCM_DATA_ADDRESS, header_len);	
	#else
	lseek(fdin, 0, SEEK_SET);
	read(fdin, (char *)src_pheader,  header_len);
	#endif
	convertData((char *)src_pheader, (char *)dst_pheader, header_len);
	header_checksum = check_byte_sum((char *)dst_pheader + 12 , header_len -12);

        
	if(dst_pheader->magic_number != CFG_MAGIC ||
		dst_pheader->header_length != header_len ||
		dst_pheader->header_check_sum!= header_checksum)
	{
		printf("upgrade 6 failed!,magic_number=0x%x =0x%x\r\n",dst_pheader->magic_number,CFG_MAGIC);
		printf("upgrade 6 failed!,header_len=0x%x =0x%x\r\n",dst_pheader->header_length,header_len);
		printf("upgrade 6 failed!,sum=0x%x checksum=0x%x\r\n",dst_pheader->header_check_sum,header_checksum);
		free((char *)dst_pheader);
		return -1;
	}

	#if 1
	#if 1
	   /* Read data from SPI FLASH memory */
   // sFLASH_ReadBuffer(dst_pheader, header_len, header_len);	
	#else
	mkdir(UNPACK_DIR,0777);

	lseek(fdin, header_len, SEEK_SET);
	#endif
	free(src_pheader);
	uheader = (UPGRADE_FILE_HEADER *)(dst_pheader + 1);
	for(i=0; i<dst_pheader->file_nums; i++)
	{
//		char pathname[200];
		#if 1
		printf("file name : %s\r\n", uheader[i].fileName);
		printf("file length: 0x%x\r\n", uheader[i].fileLen);
		printf("file offset: 0x%x\r\n", uheader[i].startOffset);
		printf("checkSum:0x%x\r\n", uheader[i].checkSum);
		#endif
		#if 0
		sprintf(pathname,"%s/%s",UNPACK_DIR,uheader[i].fileName);
		if((fdout = open(/*uheader[i].fileName*/pathname, O_WRONLY|O_CREAT|O_TRUNC|O_APPEND, 0777)) == -1) 
		{
			printf("create file %s error: ", uheader[i].fileName);
			fflush(stdout);
			perror("");
			printf("upgrade failed!\n");
			return -1;
		}
		#endif
		//check file
		#if 0
		if((pbuf = (char *)malloc(uheader[i].fileLen)) == NULL)
		{
			printf("upgrade failed!\n");
			return -1;
		}
		memset(pbuf, 0, uheader[i].fileLen);
		sFLASH_ReadBuffer(pbuf, FLASH_ReadAddress+header_len, uheader[i].fileLen);	
		//read(fdin, pbuf, uheader[i].fileLen);
		checksum = check_byte_sum(pbuf, uheader[i].fileLen);

		/*check the checksum is correct?*/
		if(checksum == uheader[i].checkSum)
		{
			printf("file %s is right, chksum = 0x%x\r\n",uheader[i].fileName, checksum);

			/* begin to copy file */
			m = uheader[i].fileLen / FLASH_SECTOR_SIZE;
			n = uheader[i].fileLen % FLASH_SECTOR_SIZE;
		
			for(j=1; j<=m; j++)
			{
				printf(".");
				//fflush(stdout);
				//write(fdout, pbuf, FLASH_SECTOR_SIZE);
				pbuf += FLASH_SECTOR_SIZE;
			}
			
			//write(fdout, pbuf, n);
			printf("\n\n");

			pbuf -= FLASH_SECTOR_SIZE * m;	/*return to start of pbuf*/
			free(pbuf);
		}
		else
		{
			printf("file %s: chksum error!\n", uheader[i].fileName);
			printf("upgrade failed!\n");
			free(pbuf);
			return -1;
		}
		#endif
		//close(fdout);	
	}
	#endif
	//close(fdin);
	printf("upgrade success!\r\n");

	return 0;
}

#if 0
file name : audio_a_o.pcm
file length: 0x6dfe
file offset: 0x568
checkSum:0x36feab
file name : audio_charge_bug.pcm
file length: 0x7d4e
file offset: 0x7366
checkSum:0x3ea6da
file name : audio_charge_end.pcm
file length: 0x7797
file offset: 0xf0b4
checkSum:0x3bcb28
file name : audio_charge_start.pcm
file length: 0x7640
file offset: 0x1684b
checkSum:0x3b1fa0
file name : audio_choise_charge_method.pcm
file length: 0xbafd
file offset: 0x1de8b
checkSum:0x5d7d87
file name : audio_device_open.pcm
file length: 0xbafd
file offset: 0x29988
checkSum:0x5d7e8b
file name : audio_num10.pcm
file length: 0x5933
file offset: 0x35485
checkSum:0x2c99a5
file name : audio_num11.pcm
file length: 0x519b
file offset: 0x3adb8
checkSum:0x28cdbd
file name : audio_num12.pcm
file length: 0x5933
file offset: 0x3ff53
checkSum:0x2c99de
file name : audio_num13.pcm
file length: 0x4be9
file offset: 0x45886
checkSum:0x25f47d
file name : audio_num14.pcm
file length: 0x5cfe
file offset: 0x4a46f
checkSum:0x2e7e6b
file name : audio_num15.pcm
file length: 0x519b
file offset: 0x5016d
checkSum:0x28cd42
file name : audio_num16.pcm
file length: 0x60ca
file offset: 0x55308
checkSum:0x306544
file name : audio_num17.pcm
file length: 0x5cfe
file offset: 0x5b3d2
checkSum:0x2e7f6a
file name : audio_num18.pcm
file length: 0x5567
file offset: 0x610d0
checkSum:0x2ab2e7
file name : audio_num19.pcm
file length: 0x62b0
file offset: 0x66637
checkSum:0x315783
file name : audio_num1.pcm
file length: 0x45e0
file offset: 0x6c8e7
checkSum:0x22f027
file name : audio_num20.pcm
file length: 0x4be9
file offset: 0x70ec7
checkSum:0x25f42a
file name : audio_num2.pcm
file length: 0x4638
file offset: 0x75ab0
checkSum:0x231c49
file name : audio_num3.pcm
file length: 0x5567
file offset: 0x7a0e8
checkSum:0x2ab3bf
file name : audio_num4.pcm
file length: 0x5381
file offset: 0x7f64f
checkSum:0x29c0b9
file name : audio_num5.pcm
file length: 0x5cfe
file offset: 0x849d0
checkSum:0x2e7f31
file name : audio_num6.pcm
file length: 0x5932
file offset: 0x8a6ce
checkSum:0x2c9904
file name : audio_num7.pcm
file length: 0x5932
file offset: 0x90000
checkSum:0x2c994e
file name : audio_num8.pcm
file length: 0x5933
file offset: 0x95932
checkSum:0x2c9956
file name : audio_num9.pcm
file length: 0x5b18
file offset: 0x9b265
checkSum:0x2d8caa
file name : audio_smoke.pcm
file length: 0xda36
file offset: 0xa0d7d
checkSum:0x6d1af5
file name : audio_temp_high1.pcm
file length: 0xb400
file offset: 0xae7b3
checkSum:0x59ff98
file name : audio_temp_high2.pcm
file length: 0x8f48
file offset: 0xb9bb3
checkSum:0x47a460

#endif


unsigned char get_pcm_data_by_name(char* filename,unsigned char**pcm_flash_address,unsigned int* pcm_flash_len)
{
	UPGRADE_FILE_HEADER *uheader;
	int i = 0;
	uheader = (UPGRADE_FILE_HEADER *)(dst_pheader + 1);
	for(i=0; i<dst_pheader->file_nums; i++)
	{
//		char pathname[200];
		#if 1
		printf("file name : %s\r\n", uheader[i].fileName);
		printf("file length: 0x%x\r\n", uheader[i].fileLen);
		printf("file offset: 0x%x\r\n", uheader[i].startOffset);
		printf("checkSum:0x%x\r\n", uheader[i].checkSum);
		#endif
		if(strncmp(filename,uheader[i].fileName,32) == 0)
		{
			printf("find it...\r\n");
			*pcm_flash_address = (unsigned char*)uheader[i].startOffset;
			*pcm_flash_len = uheader[i].fileLen;
			
			return 0;
		}		
	}
	printf("no find it...\r\n");
	return 1;
}

