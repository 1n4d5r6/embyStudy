#include "delay.h"
#include "sys.h"
#include "psd_usart.h"
#include "psd_soft_i2c.h"
#include "max30102.h"
#include "algorithm.h"
#include <stdlib.h>
//STM32F10X_HD
#define MAX_BRIGHTNESS 255
#define START 100
#define DATA_LENGTH 500

int compare(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}

int findMedian(int arr[], int size) {
	qsort(arr, size, sizeof(int), compare);
	if (size % 2 == 0) {
		int midIndex = size / 2;
		return (arr[midIndex - 1] + arr[midIndex]) / 2;
	} else {

		int midIndex = size / 2;
		return arr[midIndex];
	}
}


uint32_t aun_ir_buffer[DATA_LENGTH]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[DATA_LENGTH];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;


 int main(void)
 { 
	uint32_t un_min, un_max, un_prev_data;
	int i;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	Usart_1_Config();
	Soft_I2C_Init();
	maxim_max30102_reset();
	maxim_max30102_read_reg(0,&uch_dummy);
	maxim_max30102_init();
	un_min=0x3FFFF;
	un_max=0;
	n_ir_buffer_length=DATA_LENGTH; 
	for(i=0;i<n_ir_buffer_length;i++)
	{
			while(PAin(5)==1);  
			maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));
			if(un_min>aun_red_buffer[i])
					un_min=aun_red_buffer[i];
			if(un_max<aun_red_buffer[i])
					un_max=aun_red_buffer[i];
	}
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	while(1)
	{
	  i=0;
		un_min=0x3FFFF;
		un_max=0;
		for(i=START;i<DATA_LENGTH;i++)
		{
				aun_red_buffer[i-START]=aun_red_buffer[i];
				aun_ir_buffer[i-START]=aun_ir_buffer[i];
				if(un_min>aun_red_buffer[i])
				un_min=aun_red_buffer[i];
				if(un_max<aun_red_buffer[i])
				un_max=aun_red_buffer[i];
		}
		
		for(i=DATA_LENGTH - START;i<DATA_LENGTH;i++)
		{
			uint32_t red_buf , ir_buf;
			while(PAin(5)==1);
			maxim_max30102_read_fifo(&red_buf, &ir_buf);		
			aun_ir_buffer[i] = ir_buf;
			aun_red_buffer[i] = red_buf;
		}
		maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
		printf(" HR=%i,", n_heart_rate); 
		printf(" HRvalid=%i,", ch_hr_valid);
		printf(" SpO2=%i,", n_sp02);
		printf(" SPO2Valid=%i\r\n", ch_spo2_valid);
    delay_ms(1);		
	}
}


