#ifndef _USER_H_
#define _USER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "BSP\bsp.h"
#include "HARDWARE\hardware.h"

#include "SOFTWARE\pid.h"
#include "SOFTWARE\user_fun.h"
#include "SOFTWARE\auto_sail.h"
#include "SOFTWARE\vofa_p.h"
#include "SOFTWARE\battery_calculation.h"

#include "self_portable\self_portable.h"

/**************************电调相关类型*****************************************/
//电调任务参数
typedef struct
{
    er_Type er;    //电调
    uint8_t max_inc;     //单任务周期脉宽最大增量 -> us单位
    uint8_t cycle;      //任务执行周期 -> ms单位
    QueueHandle_t*queueAddr;   ///队列句柄的地址,用于电调任务接收控制信号
}ER_Type;

//电调控制类型 -> 通过队列发发送,由对应任务负责代码实现 CmdQueue -> 见main.c
typedef struct
{
    uint8_t type;   //0:保留 1:目标输出 2:最大增量 3:任务周期
    int dat;        
}ERctr_Type;

/**************************直流电机相关类型*****************************************/
//直流电机任务参数
typedef struct
{
    a4950_Type a4950 ;  //a4950
    uint16_t max_inc ;  //最大增量
    uint8_t cycle;
    QueueHandle_t*queueAddr;   //队列句柄的地址,用于直流电机任务接收控制信号
}DCMotor_Type;

//直流电机控制类型 -> 通过队列发发送,由对应任务负责代码实现 CmdQueue -> 见main.c
typedef struct
{
    uint8_t type;   //0:保留 1:目标速度 2:刹车 3:单周期增量 4:基于当前目标增加
    int dat;
}DCMotorCtr_Type;

/**************************舵机相关类型*****************************************/
//舵机任务参数
typedef struct
{
    streetMotor_Type streetMotor; //舵机 见hardware_def.h
    uint8_t cycle;              //任务执行周期 -> ms单位
    float angle_inc;          //单任务周期角度增量
    QueueHandle_t*queueAddr;    //队列句柄的地址,用于电调任务接收控制信号
}StreetMotor_Type;

//舵机控制类型 -> 通过队列发发送,由对应任务负责代码实现 CmdQueue -> 见main.c
typedef struct
{
    uint8_t type;       //0:保留 1:基于当前角度增加 2:单周期增量 3:设置目标角度 4:直接设置角度
    float dat;        //目标角度
}StreetMotorCtr_Type;

/**************************蜂鸣器相关类型*****************************************/
//蜂鸣器控制 -> 通过队列发发送,由对应任务负责代码实现 CmdQueue -> 见main.c
typedef struct
{
    uint8_t count;  //鸣响次数
    uint16_t fre;   //频率
    uint16_t on_ms; //鸣响时长
    uint16_t off_ms;    //关闭时长
}BeepCtr_Type;

/**************************联合体*****************************************/
//任务控制 -> 通过队列发发送,由对应任务负责代码实现 CmdQueue -> 见main.c
//整合Ctr_Type,方便向任务发送
typedef union
{
    BeepCtr_Type BeepCtr;
    StreetMotorCtr_Type StreetMotorCtr;
    DCMotorCtr_Type DCMotorCtr;
    ERctr_Type  ERctr;
}Ctr_Type;


/**************************系统状态*****************************************/
typedef struct
{
    short DCMotor1_F;
    short DCMotor2_F;
}User_Set;

//系统状态
typedef struct
{
    uint16_t nrf_signal; //nrf信号 0:正常 其他:信号丢失时长
    uint16_t RunMode;    //运行模式
}sysStatus_Type;

void MPU_Task(void*ptr);                    //陀螺仪刷新任务
void ReplyMaster_Task(void*ptr);            //主机回复任务
void nRF24L01_Intterrupt_Task(void*ptr);    //nrf中断任务
void Voltage_Task(void*ptr);                //电池电压检测任务
void DepthSensor_Task(void*ptr);            //水位深度检测任务
void OLED_Task(void*ptr);                   //oled刷新任务
void KeyInput_Task(void*ptr);               //按键任务
void ER_Task(void*ptr);                     //电调任务
void Motor_Task(void*ptr);                  //直流电机控制任务
void StreetMotor_Task(void*ptr);            //舵机任务
void Beep_Task(void*ptr);                   //蜂鸣器任务

void AutoRun_Task(void*ptr);

#endif  //_USER_H_

