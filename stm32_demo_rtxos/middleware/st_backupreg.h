#ifndef _ST_BACKUPREG_H_
#define _ST_BACKUPREG_H_


/**
  * @brief  Writes data Backup DRx registers.
  * @param  FirstBackupData: data to be written to Backup data registers.
  * @retval None
  */
void WriteToBackupReg(uint16_t FirstBackupData);


/**
  * @brief  Checks if the Backup DRx registers values are correct or not.
  * @param  FirstBackupData: data to be compared with Backup data registers.
  * @retval 
  *          - 0: All Backup DRx registers values are correct
  *          - Value different from 0: Number of the first Backup register
  *            which value is not correct
  */
uint8_t CheckBackupReg(uint16_t FirstBackupData);

int st_BackupReg_init(void);

#endif
