#include "../inc/libmx.h"
void mx_printint(int n)
{
    int temp = 0;
    int tmp = n;
    while (n > 0) {
        n = n / 10;
        temp++;
    }
    char c[temp];
    if (tmp == 0) {
		mx_printchar('0');
	}
	if (tmp < 0) {
		if (n == -2147483648) {
			mx_printint(-214748364);
			mx_printint(8);
		}
		else {
			mx_printchar('-');
			mx_printint(-n);
		}
	}
	else {    
		for(int i = 0; i < temp; i++) {
        	c[i] = tmp%10 + 48;
        	tmp /= 10;
    	}
    	for(int i = temp - 1; i >= 0; i--) {
        	mx_printchar(c[i]);
    	}
	}
}
