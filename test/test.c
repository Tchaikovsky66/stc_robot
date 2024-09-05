#include <stdio.h>


#define variable 100
float motor_acc[100];
float msteps = 3200.0;

volatile int x_flag = 0;

void direct_acc(int speed)
{
	int minspeed = 20;
    int maxspeed = 180;
    msteps = msteps/10;
	if(speed>maxspeed)
		speed = maxspeed;
	for(int i = 0;i<variable;i++)
	{
		motor_acc[i] = minspeed + i*(speed-minspeed)/variable;
		motor_acc[i] = (1000*10/(msteps))/(motor_acc[i]/60);
        motor_acc[i] = motor_acc[i]/2;
		// if(motor_acc[i] < 10)
		// 	motor_acc[i] = 10;
	}
}


int one()
{
    if(x_flag)
    {
        return 1;
    }

    else
        printf("one()\n");
    return 0;
}

int two()
{
    if(one())
    {
        return 1;
    }
    else
        printf("two\n");
    return 0;
}

int main()
{
    x_flag = 1;
    if(two())
        printf("quit\n");
    direct_acc(50);
    for(int i = 0;i<100;i++)
    {
        printf("%f\n",motor_acc[i]);
    }
}