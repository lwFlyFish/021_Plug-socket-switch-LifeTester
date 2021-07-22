#ifndef __MALLOC_H
#define __MALLOC_H

///////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
///////////////////////////////////////////////////////////////////////////
 
#ifndef NULL
#define NULL 0
#endif

//�ڴ�����趨.
#define MEM_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM_MAX_SIZE			10*1024  						//�������ڴ� 20K
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	//�ڴ���С
		 
//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(void);				//��ʼ��
	u8 (*perused)(void);		  	//�ڴ�ʹ����
	u8 	*membase;					//�ڴ�� 
	u16 *memmap; 					//�ڴ����״̬��
	u8  memrdy; 					//�ڴ�����Ƿ����
};

///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
// public:
	//��mallco.c���涨��
	extern struct _m_mallco_dev mallco_dev;	


// private:




///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
// public:
	//�����ڴ�
	void mymemset(void* s, u8 c, u32 count);
	//�����ڴ�
	void mymemcpy(void* des, void* src, u32 n);

	//�ڴ�����ʼ������(��/�ڲ�����)
	void SysMemInit(void);					
	//�ڴ����(�ڲ�����)
	u32 mem_malloc(u32 size);		 		
	//�ڴ��ͷ�(�ڲ�����)
	u8 mem_free(u32 offset);

	//���ڴ�ʹ����(��/�ڲ�����) 
	u8 mem_perused(void);					

	//�ڴ��ͷ�(�ⲿ����)
	void myfree(void* ptr);
	//�ڴ����(�ⲿ����)
	void* mymalloc(u32 size);

	//���·����ڴ�(�ⲿ����)
	void* myrealloc(void* ptr, u32 size);


// private:


	

#endif



