// This code was used as the main function in main.c
// to test that Charlieplexing LEDs worked.
char blinkState = 0;
char numberOfLedSOn = 12;
unsigned long microSecondsElapsed = 0;
while(1){
    char upperLimit = numberOfLedSOn-1;
     if (blinkState){
         upperLimit++;
     }
    for (int i=0;i<upperLimit;	i++){
        setCharlieplexingState(i);
        _delay_us(1000);
        microSecondsElapsed+=1000;
    }
     // Update blinkState every second
     if (microSecondsElapsed >= 1e6){
         microSecondsElapsed = 0;
         blinkState = !blinkState;
     }
    }