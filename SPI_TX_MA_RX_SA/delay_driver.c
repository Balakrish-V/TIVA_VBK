void delay_ms(void)
{
    unsigned long int i;

    for(i=0;i<100000000;i++);
}
void delay_sec(unsigned long sec)
{
    unsigned long i;
    for(i=0;i<1250000*sec;i++);
}
void delay(long k)
{
    while(k--);
}
