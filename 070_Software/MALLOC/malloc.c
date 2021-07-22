///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//#include "stm32f10x.h"
#include "SysConfig.h"
#include "malloc.h"	    
///////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ����, ����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
///////////////////////////////////////////////////////////////////////////

//�ڴ��(4�ֽڶ���)
__align(4) u8 membase[MEM_MAX_SIZE];			//SRAM�ڴ��
//�ڴ�����
u16 memmapbase[MEM_ALLOC_TABLE_SIZE];			//SRAM�ڴ��MAP

//�ڴ�������	   
const u32 memtblsize=MEM_ALLOC_TABLE_SIZE;		//�ڴ���С
const u32 memblksize=MEM_BLOCK_SIZE;			//�ڴ�ֿ��С
const u32 memsize=MEM_MAX_SIZE;					//�ڴ��ܴ�С

//�ڴ���������
struct _m_mallco_dev mallco_dev=
{
	SysMemInit,			//�ڴ��ʼ��
	mem_perused,		//�ڴ�ʹ����
	membase,			//�ڴ��
	memmapbase,			//�ڴ����״̬��
	0,  				//�ڴ����δ����
};

//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void mymemcpy(void *des,void *src,u32 n)  
{  
	u8 *xdes=des;
	u8 *xsrc=src; 
	while(n--)*xdes++=*xsrc++;  
}  

//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s,u8 c,u32 count)  
{  
	u8 *xs = s;  
	while(count--)*xs++=c;  
}	   

//�ڴ�����ʼ��  
void SysMemInit(void)  
{  
	//�ڴ�״̬����������  
   mymemset(mallco_dev.memmap, 0, memtblsize*2);

	//�ڴ��������������  
	mymemset(mallco_dev.membase, 0, memsize);	

	//�ڴ�����ʼ��OK
	mallco_dev.memrdy = 1;						
}  

//��ȡ�ڴ�ʹ����
//����ֵ:ʹ����(0~100)
u8 mem_perused(void)  
{  
	u32 used=0;  
	u32 i;  

	for (i=0; i<memtblsize; i++)  
	{  
		if (mallco_dev.memmap[i])
			used++; 
	} 

	return (used*100) / (memtblsize);
}  

//�ڴ����(�ڲ�����)
//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
u32 mem_malloc(u32 size)  
{  
	signed long offset = 0;  
	//��Ҫ���ڴ����  
	u16 nmemb;	
	//�������ڴ����
	u16 cmemb = 0;
	u32 i;  

	//δ��ʼ��,��ִ�г�ʼ�� 
	if (!mallco_dev.memrdy)
		mallco_dev.init();	

	if (size == 0)
	{
		//����Ҫ����
		return 0xFFFFFFFF;				
	}

	//��ȡ��Ҫ����������ڴ����
	nmemb = size/memblksize;  					
	if (size%memblksize)
		nmemb++; 

	//���������ڴ������  
	for (offset=memtblsize-1; offset>=0; offset--)	
	{     
		//�������ڴ��������
		if(!mallco_dev.memmap[offset])
		{
			cmemb++;
		}
		else 
		{
			//�����ڴ������
			cmemb = 0;
		}

		//�ҵ�������nmemb�����ڴ��
		if (cmemb == nmemb)						
		{
			//��ע�ڴ��ǿ� 
			for (i=0; i<nmemb; i++)
			{  
				mallco_dev.memmap[offset+i]=nmemb;  
			}  

			//����ƫ�Ƶ�ַ  
			return (offset*memblksize);			
		}
	}  

	//δ�ҵ����Ϸ����������ڴ��  
	return 0xFFFFFFFF;
}  

//�ͷ��ڴ�(�ڲ�����) 
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
u8 mem_free(u32 offset)  
{  
	int i;  

	//δ��ʼ��,��ִ�г�ʼ��
	if (!mallco_dev.memrdy)
	{
		mallco_dev.init();

		//δ��ʼ��  
		return 1;
	}  

	//ƫ�����ڴ����. 
	if (offset<memsize)
	{  
		//ƫ�������ڴ�����
		int index = offset/memblksize;		

		//�ڴ������
		int nmemb = mallco_dev.memmap[index];	

		//�ڴ������
		for (i=0; i<nmemb; i++)  				
		{  
			mallco_dev.memmap[index+i]=0;  
		}

		return 0;  
	}
	else
	{
		//ƫ�Ƴ�����.
		return 2;
	}
}  

//�ͷ��ڴ�(�ⲿ����) 
//ptr:�ڴ��׵�ַ 
void myfree(void *ptr)  
{  
	u32 offset;  

	if (ptr == NULL)
		return;//��ַΪ0.  

	offset = (u32)ptr-(u32)mallco_dev.membase;  

	//�ͷ��ڴ�
	mem_free(offset);	
}  

//�����ڴ�(�ⲿ����)
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void* mymalloc(u32 size)
{  
	u32 offset;  									      
	offset = mem_malloc(size);

	if (offset == 0xFFFFFFFF)
		return NULL;  
	else 
		return (void*)((u32)mallco_dev.membase+offset);  
}  

//���·����ڴ�(�ⲿ����)
//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
void* myrealloc(void* ptr, u32 size)
{  
	u32 offset;  

	offset = mem_malloc(size);

	if (offset == 0XFFFFFFFF)
	{
		return NULL;     
	}
	else  
	{  									   
		//�������ڴ����ݵ����ڴ�   
		mymemcpy((void*)((u32)mallco_dev.membase+offset),ptr,size);	
		//�ͷž��ڴ�
		myfree(ptr);  											  	

		//�������ڴ��׵�ַ
		return (void*)((u32)mallco_dev.membase+offset);  			
	}  
}

