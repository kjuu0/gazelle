#include <stdio.h>  
#include <stdlib.h>
#include<string.h>  

typedef struct point{
    int x;
    int y;
} 
point;

void print_point(point* data)  
{  
    printf("point has x=%d, y=%d", data->x, data->y);
}

void print_point2(int x, int y, point* data, int (*p)())  
{  

    int a = p(x,y, data);
    printf("point has x=%d, y=%d", data->x, data->y);
    printf("return value is %d", a);
}

int set_point(int x, int y, point* data)  
{
    data->x = x;
    data->y = y;
    return 1000;
}


int main()  
{  
    point* data = (point*) malloc(sizeof(point));
    data->x = 0;
    data->y = 0;
    print_point(data);

    int x;
    int y;
    printf("Enter two integers: ");
    scanf("%d", &x);
    scanf("%d", &y);

    print_point2(x, y, data, set_point);

    return 0;  
}  