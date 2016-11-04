#define RECEIVEBUFF_SIZE 300       //Max is 65535
struct DMA_USARTx_RecevConfig 
{
	int  CurrentDataStartNum;     //Current data start receive point
	int  CurrentDataEndNum;       //Current data end receive point
	int  NextDataStartNum;        //Next data start receive point
	int  DMA_RecevCount;          //Received data numbers
};

uint8_t DMA_USARTx_RecevBuff[RECEIVEBUFF_SIZE];                      //DMA USARTx Received Data Buff
uint8_t Usartx_recev_buff[RECEIVEBUFF_SIZE];                         //USARTx Received Data Buff
struct  DMA_USARTx_RecevConfig   DMA_USARTx_RecevIndicator;          //Define "DMA_USARTx_RecevIndicator" 

/*******************************************************************************
* Function Name  : int  DMA_UARTx_RecevDataGet(void)
* Description    : DMA Receive Data From USRTx, UARTx could be UART1,UART2,UART3,UART4,UART5
* Input          : Usartx_recev_buff[]: Usartx could be Usart1,Usart2,Usart3,Usart4,Usart5
				   DMAy_Channelx: where y can be 1 or 2 to select the DMA and 
  *                x can be 1 to 7 for DMA1 and 1 to 5 for DMA2 to select the DMA Channel.
* Output         : None
* Return         : Length Of Received Data
*******************************************************************************/
int  DMA_UARTx_RecevDataGet(uint8_t Usartx_recev_buff[],DMA_Channel_TypeDef* DMAy_Channelx)
{
   int i=0,j=0;
	 u16 DMA_RecevLength =0;             //define DMA_RecevLength
     memset(Usartx_recev_buff, 0x00, sizeof(Usartx_recev_buff));       //clear up Usart3_recev_buff
	 DMA_USARTx_RecevIndicator.CurrentDataStartNum = DMA_USARTx_RecevIndicator.NextDataStartNum ;
	  
	 i = RECEIVEBUFF_SIZE - DMA_GetCurrDataCounter(DMAy_Channelx);       //choose the channel of DMA
	 if(DMA_USARTx_RecevIndicator.DMA_RecevCount <i)
	 {
     DMA_RecevLength =i -DMA_USARTx_RecevIndicator.DMA_RecevCount;
   }
	 else
	 {
     DMA_RecevLength = RECEIVEBUFF_SIZE -DMA_USARTx_RecevIndicator.DMA_RecevCount + i;
   }
   DMA_USARTx_RecevIndicator.DMA_RecevCount = i;
	
	 if((DMA_USARTx_RecevIndicator.CurrentDataStartNum + DMA_RecevLength-1) < RECEIVEBUFF_SIZE)
	 {
     DMA_USARTx_RecevIndicator.CurrentDataEndNum =DMA_USARTx_RecevIndicator.CurrentDataStartNum +DMA_RecevLength-1;     
   }
	 else
	 {
     DMA_USARTx_RecevIndicator.CurrentDataEndNum =(DMA_USARTx_RecevIndicator.CurrentDataStartNum +DMA_RecevLength-1) -RECEIVEBUFF_SIZE;  
   }
	 printf("\r\nDMA UARTx Recev Data Start Num:%d---End Num: %d\r\n",DMA_USARTx_RecevIndicator.CurrentDataStartNum,DMA_USARTx_RecevIndicator.CurrentDataEndNum);    //print test
	 if(DMA_USARTx_RecevIndicator.CurrentDataEndNum ==(RECEIVEBUFF_SIZE-1))
	 {
	   DMA_USARTx_RecevIndicator.NextDataStartNum = 0;
   }
	 else
	 {
		 DMA_USARTx_RecevIndicator.NextDataStartNum = DMA_USARTx_RecevIndicator.CurrentDataEndNum + 1;
   }	
   //////////////////////////Data Copy///////////////////////////////////////////////////////////////////
   if(DMA_RecevLength !=0)
	 {
     j =DMA_USARTx_RecevIndicator.CurrentDataStartNum;
		 if(DMA_USARTx_RecevIndicator.CurrentDataEndNum >DMA_USARTx_RecevIndicator.CurrentDataStartNum)
		 {
			 for(i=0; i<DMA_RecevLength; i++,j++)
			 {
					Usartx_recev_buff[i] =DMA_USARTx_RecevBuff[j];	
			 }
		 }
		 else
		 {
			 for(i=0; i<DMA_RecevLength; i++)
			 {
					if( j<(RECEIVEBUFF_SIZE-1) )
					{
						 Usartx_recev_buff[i] =DMA_USARTx_RecevBuff[j];
						 j++;				
					}
					else if( j==(RECEIVEBUFF_SIZE-1) )
					{
						 Usartx_recev_buff[i] =DMA_USARTx_RecevBuff[j];
						 j =0;				 
					}
			  } 
      }
    }
	  return DMA_RecevLength;
}