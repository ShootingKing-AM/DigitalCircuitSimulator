# DigitalCircuitSimulator (v1.0)

Using this program one can Simulate and generate truth tables for your digital circuit logics comprising of various gates and any number of inputs per gate. 

## How to use this program ?

![Example Circuit](http://shootingking-am.github.io/DigitalCircuitSimulator/skGates.bmp)

Sample Output for the circuit shown above :
```
        Welcome to Digital Circuit Simulator v1.0

 To create your own circuit the procedure is
        1. Specify gate types and number of inputs
        2. Specify connections between different ports of gates
        3. Specify the output port
        4. Truth table will be printed, you can also try giving our own inputs


1. Enter GateType (1 - And, 0 - Or) and number of inputs for that gate Eg. 0 2
To Go to next Step type any value other than 1,0 as gate type

Enter GateType (1 - And, 0 - Or) and number of inputs for that gate : 0 2
Enter GateType (1 - And, 0 - Or) and number of inputs for that gate : 1 2
Enter GateType (1 - And, 0 - Or) and number of inputs for that gate : 1 2
Enter GateType (1 - And, 0 - Or) and number of inputs for that gate : 4 0


1: OR
Input Serials : 1, 2
Output Serial : 3

2: AND
Input Serials : 4, 5
Output Serial : 6

3: AND
Input Serials : 7, 8
Output Serial : 9


2. To Proceed to next Step, type self refering connections, eg 1 1 or 0 0 or 3 3


Enter connections (Eg. '1 2' connects 1 and 2 points) : 3 7
Enter connections (Eg. '1 2' connects 1 and 2 points) : 6 8
Enter connections (Eg. '1 2' connects 1 and 2 points) : 0 0

3. Enter Output Serial for the circuit : 9

Input required for the following gates :
1 2 - OR(0)
4 5 - AND(1)

Truth table (with all possible combinations) :
                0 0 0 0  | 0
                0 0 0 1  | 0
                0 0 1 0  | 0
                0 0 1 1  | 0
                0 1 0 0  | 0
                0 1 0 1  | 0
                0 1 1 0  | 0
                0 1 1 1  | 1
                1 0 0 0  | 0
                1 0 0 1  | 0
                1 0 1 0  | 0
                1 0 1 1  | 1
                1 1 0 0  | 0
                1 1 0 1  | 0
                1 1 1 0  | 0
                1 1 1 1  | 1


4. Want to try out your own values ? Enter input in form x x x x...
upto number of inputs
Enter Input Values : 0 1 0 1
Output : 0
Enter Input Values : 1 0 1 0
Output : 0
Enter Input Values : 1 1 1 1
Output : 1
```

Currently the program dosent provide error cheking, so be sure about what you are feeding it !


To simulate your own circuit the procedure is,

1. Specify gate types and number of inputs.
   Only two gate types are presently defined in the program, AND and OR.
   In this step only specifiy the gates present in your circuit, and Number of inputs each gate has.

2. Specify connections between different ports of gates.
   Now, set connections betweeen different gates you defined in step 1.
   After completing step1, the program will print out all the gates so defined and specifies a "Serial" number for each DataPort.
   DataPort means any functional port for a gate, can be input or output.
   Example, Entering '3 7' in step 2 means,
   Conneting port 3 to port 7, REMEMBER ! Signal flows in the same order as you specify them here, so 3 7 means signal flows from 3 to    7 which is different from '7 3'.

3. Specify the output port.
   Specify a Serial of the entire circuit from whcih you are expecting to get output.

4. Truth table will be printed, you can also try giving our own inputs.

## Issues / Bugs / Requests
[Here](https://github.com/ShootingKing-AM/DigitalCircuitSimulator/issues)

## TODO
- Test it throughly.
- Extend Base Gate class to implement other gates too.
- A gui would be a lot easier for the user. (Hopefully will be coming soon)

This program is tested under WINDOWS 7, using MSVS11 c++ compiler.
