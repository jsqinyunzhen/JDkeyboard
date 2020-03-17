/*
 * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * $Revision:   V5.1.0
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       RTX Configuration
 *
 * -----------------------------------------------------------------------------
 */
 
#include "cmsis_compiler.h"
#include "rtx_os.h"
#include <stdio.h>


#include "project_config.h"


#include "st_printf.h"
// OS Idle Thread
__WEAK __NO_RETURN void osRtxIdleThread (void *argument) {
  (void)argument;

  for (;;) {}
}





// OS Error Callback function
__WEAK uint32_t osRtxErrorNotify (uint32_t code, void *object_id) {
  (void)object_id;
	uint32_t max_len, start_addr;
  switch (code) {
    case osRtxErrorStackUnderflow:
		
      // Stack underflow detected for thread (thread_id=object_id)
		printf("Stack underflow detected for thread (thread_id=object_id) =0x%08x\r\n",(unsigned int)object_id);		
		if(thread_main == object_id)
		{
			printf("thread_main Stack underflow\r\n");
			start_addr = (uint32_t)thread1_stk_1;
			max_len = sizeof(thread1_stk_1);
		}
		else if(thread_uart_msg == object_id)
		{
			printf("thread_uart_msg Stack underflow\r\n");
			start_addr = (uint32_t)thread1_stk_2;
			max_len = sizeof(thread1_stk_2);
		}
		else if(thread_app == object_id)
		{
			printf("thread_app Stack underflow\r\n");
			start_addr = (uint32_t)thread1_stk_3;
			max_len = sizeof(thread1_stk_3);
		}
		//except_Handler();
		#ifdef MCU_USING_MODULE_BACKTRACE
		dump_memeory(start_addr, max_len);
		#endif
		print_heap_info();
				
      break;
    case osRtxErrorISRQueueOverflow:
      // ISR Queue overflow detected when inserting object (object_id)
		printf("ISR Queue overflow detected when inserting object =0x%08x\r\n",(unsigned int)object_id);
      break;
    case osRtxErrorTimerQueueOverflow:
      // User Timer Callback Queue overflow detected for timer (timer_id=object_id)
		printf("User Timer Callback Queue overflow detected for timer (timer_id=object_id)\r\n");
      break;
    case osRtxErrorClibSpace:
      // Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM
		printf("Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM\r\n");
      break;
    case osRtxErrorClibMutex:
      // Standard C/C++ library mutex initialization failed
		printf("Standard C/C++ library mutex initialization failed\r\n");
      break;
    default:
		printf("osRtxErrorNotify.....\r\n");
      break;
  }
  for (;;) {}
//return 0U;
}
