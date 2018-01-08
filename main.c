#include "ch554.h"
#include "types.h"

#include "ch554_conf.H"                                                      
#include "Delay.H"
#include "usb_endp.h"

#include <string.h>
#include <stdio.h>

uint8_t Count,FLAG;
extern UINT8 Ready;

sbit Ep2InKey = P1^5;


/*��������*/
xdata unsigned char HIDKey[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};


/*******************************************************************************
* Function Name  : Enp1IntIn()
* Description    : USB�豸ģʽ�˵�1���ж��ϴ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp1IntIn( )
{
    memcpy( Ep1Buffer, HIDKey, sizeof(HIDKey));                              //�����ϴ�����
    UEP1_T_LEN = sizeof(HIDKey);                                             //�ϴ����ݳ���
    UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;                //������ʱ�ϴ����ݲ�Ӧ��ACK
}

void HIDValueHandle()
{
    UINT8 i;
        i = getkey( );
        //printf( "%c", (UINT8)i );
        switch(i)
        {
//���������ϴ�ʾ��
        case 'A':                                                         //A��
            FLAG = 0;
            HIDKey[2] = 0x04;                                             //������ʼ
            Enp1IntIn();
            HIDKey[2] = 0;        	                                       //��������
            while(FLAG == 0)
            {
                ;    /*�ȴ���һ���������*/
            }
            Enp1IntIn();
            break;
        case 'P':                                                         //P��
            FLAG = 0;
            HIDKey[2] = 0x59;
            Enp1IntIn();
            HIDKey[2] = 0;                                                //��������
            while(FLAG == 0)
            {
                ;    /*�ȴ���һ���������*/
            }
            Enp1IntIn();
            break;
        case 'Q':                                                         //Num Lock��
            FLAG = 0;
            HIDKey[2] = 0x53;
            Enp1IntIn();
            HIDKey[2] = 0;                                                //��������
            while(FLAG == 0)
            {
                ;    /*�ȴ���һ���������*/
            }
            Enp1IntIn();
            break;
        default:                                                          //����
            UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;     //Ĭ��Ӧ��NAK
            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;     //Ĭ��Ӧ��NAK
            break;
        }
}

main()
{
    CH554_Init();                                                           //CH559ʱ��ѡ������

    printf("start ...\n");
                                                         //����Ƭ���ж�
    FLAG = 0;
    Ready = 0;
	
    while(1)
    {
        if(Ready)
        {
            HIDValueHandle();
        }
        if(Ready&&(Ep2InKey == 0)){                                       
          printf("ID0 = %02x %02x \n",(UINT16)*(PUINT8C)(0x3FFA),(UINT16)*(PUINT8C)(0x3FFB));
          printf("ID1 = %02x %02x \n",(UINT16)*(PUINT8C)(0x3FFC),(UINT16)*(PUINT8C)(0x3FFD));
          printf("ID2 = %02x %02x \n",(UINT16)*(PUINT8C)(0x3FFE),(UINT16)*(PUINT8C)(0x3FFF));
          mDelaymS( 10 ); 					
          Enp1IntIn();				
        }
        mDelaymS( 100 );                                                   //ģ�ⵥƬ����������
    }
}
