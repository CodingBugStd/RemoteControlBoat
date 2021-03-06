#include "a4950.h"

/*******************************************************************
 * a4950.c
 * a4950.h
 * 功能:
 * 驱动多块a4950,每块a4950能驱动一个直流电机。
 * 关于移植:
 * 需要自定义a4950的标号和设置每块a4950的2个PWM通道标号
 * 需要对接port_PWMOut() -> 能根据PWM通道标号在对应的管脚输出PWM波
 * 需要对接port_PWMRead() -> 能根据PWM通道标号获得对应管脚PWM的当前输出
 * 需要设置最大PWM输出,a4950数量等
 * 2022/3/19   庞碧璋
 *******************************************************************/

#define port_PWMOut(ch,width)   PWM_Out(ch,width)   //PWM输出接口 ch->pwm输出的管道标号 width->脉宽
#define port_PWMRead(ch)        PWM_Read(ch)        //获取当前PWM输出接口

/*******************************************************************
 * 功能:控制A4950驱动电机
 * 参数:
 *  a4950:要控制的a4950
 *  out:a4950输出 -> 范围 : -half_max ~ half_max
 * 返回值:无
 * 2022/3/19   庞碧璋
 *******************************************************************/
void A4950_Out(a4950_Type*a4950,int out)
{
    uint16_t pwm_width[2];
    //脉宽限位
    if(out > a4950->half_max)
        out = a4950->half_max;
    else if(out < -a4950->half_max)
        out = -a4950->half_max;
    //计算输出
    if(a4950->dir)
    {
        pwm_width[0] = a4950->half_max - out;
        pwm_width[1] = a4950->half_max + out;
    }else
    {
        pwm_width[0] = a4950->half_max + out;
        pwm_width[1] = a4950->half_max - out;
    }
    //输出
    port_PWMOut(a4950->pwm1_ch,pwm_width[0]);
    port_PWMOut(a4950->pwm2_ch,pwm_width[1]);
}

/*******************************************************************
 * 功能:读取当前a4950的输出
 * 参数:
 *  a4950:要读取的a4950
 * 返回值:无
 * 2022/3/19   庞碧璋
 *******************************************************************/
int A4950_ReadOut(a4950_Type*a4950)
{
    if(a4950->dir)
        return -(port_PWMRead(a4950->pwm1_ch) - a4950->half_max);
    else
        return port_PWMRead(a4950->pwm1_ch) - a4950->half_max;
}

/*******************************************************************
 * 功能:a4950刹车
 * 参数:
 *  a4950_id:要刹车的a4950标号
 * 返回值:无
 * 2022/3/19   庞碧璋
 *******************************************************************/
void A4950_Brake(a4950_Type*a4950)
{
    port_PWMOut(a4950->pwm1_ch , a4950->half_max);
    port_PWMOut(a4950->pwm2_ch , a4950->half_max);
}


