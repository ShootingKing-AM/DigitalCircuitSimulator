/**
	This file is part of DigitalCircuitSimulator
	Copyright (C) 2015  ShootingKing (Harsha Raghu)

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include <iostream>
#include <stdio.h>
#include <math.h>

#ifdef _MSC_VER 
	// ignore deprecated MSVC warning C4996
	#pragma warning(disable : 4996)
#endif

using namespace std;

#if !defined null
	#define null NULL
#endif

class Gate;

enum
{
	NOT_DEFINED = 0,
	DEFINED_DIRECT,
	DEFINED_BY_CALC
};

class GateData // Gate outputs and Inputs
{
	public:
		bool isDirectInput;
		int isDefined;
		Gate* pParent; // Output form parent goes to child
		Gate* pChild; 
		int val;

		GateData()
		{
			isDirectInput = true;
			isDefined = NOT_DEFINED;
			pParent = pChild = null;
			val = 0;
		}
};

class Gate
{
    public:

	    char GateType[4];
		int id;
        int numInputs;

		GateData** pInputs;
		GateData* pOutput;

    	Gate* nextGate;
        Gate* pevGate;

		Gate() {}
        Gate( int numInpts )
        {
            nextGate = pevGate = null;
            numInputs = numInpts;
            pInputs = new GateData *[numInpts];
			for( int i = 0; i < numInpts; i++ )
			{
				pInputs[i] = new GateData;
			}
			pOutput = new GateData;
        }
        ~Gate()
        {
            delete [] pInputs;
            delete pOutput;
        }
		virtual int CalcOutput() { return 0; }

        int GetOutput()
		{
			for( int i = 0; i < numInputs; i++ )
			{
				if( !pInputs[i]->isDefined )
				{
					pInputs[i]->val = pInputs[i]->pParent->GetOutput();
					pInputs[i]->isDefined = DEFINED_BY_CALC;
				}
			}
			return CalcOutput();
		}
};

class AndGate : public Gate
{
    public:
		AndGate(int numInpts) : Gate(numInpts)
        {
            strcpy(GateType, "AND");
        }
        int CalcOutput()
        {
            pOutput->val = pInputs[0]->val;
            for( int i = 0; i < numInputs; i++ )
            {
                pOutput->val *= pInputs[i]->val;
            }
			return pOutput->val;
        }
};

class OrGate : public Gate
{
    public:
		OrGate(int numInpts) : Gate(numInpts)
		{
			strcpy(GateType, "OR"); 
		}
        int CalcOutput()
        {
            pOutput->val = pInputs[0]->val;
            for( int i = 1; i < numInputs; i++ )
            {
                pOutput->val += pInputs[i]->val;
            }
            pOutput->val = (((pOutput->val)>0)?1:0);
			return pOutput->val;
        }
};

class GateManager
{
    Gate *pHead, *pTail;
    // head - Gate1 - Gate2 - Gate3 - ..... - Tail null
	int totalGates;
	int outputSerial;

    public:
		int numDirectPorts;
		GateManager()
		{
			pHead = null;
			pTail = null;
			totalGates = 0;
			numDirectPorts = 0;
		}
        Gate* addGate( int type, int numinpts )
        {
			Gate* ptr;

            if( type )
            {
                ptr = new AndGate(numinpts);
			}
			else
			{
                ptr = new OrGate(numinpts);
			}

            if( pHead == null )
            {
                pHead = ptr;
            }
            ptr->pevGate = pTail;

			if( ptr->pevGate != null )
			{
				ptr->pevGate->nextGate = ptr;
			}

			pTail = ptr;
			ptr->id = totalGates++;

			return ptr;
		}
		void printDataPoints()
		{
			Gate* itr = pHead;
			int Serial = 1;

			printf( "\n\n" );
			if( itr != null )
			{
				while( itr != null )
				{
					printf( "%d: %s \nInput Serials : ", (itr->id+1), itr->GateType );
					for( int i = 0; i < itr->numInputs; i++ )
					{
						if( i > 0 ) printf( ", " );
						printf( "%d", Serial++ );
					}
					printf( "\nOutput Serial : %d\n\n", Serial++ );
					itr = itr->nextGate;
				}
			}
		}
		
		Gate* getGateIDFromSerial(int serial, int* InputID)
		{
			Gate* ptr = pHead;
			int oSerial = 0;
			if( ptr != null )
			{
				while( ptr != null )
				{
					oSerial += ptr->numInputs+1;
					//cout << oSerial << " " << serial << endl;
					//printf( "%d %d\n", oSerial, serial );
					if( serial <= oSerial)
					{
						*InputID = oSerial-serial;
						return ptr;
					}
					ptr = ptr->nextGate;
				}
			}
			return null;
		}
		
		void ConnectDataPoints( int s1, int s2 )
		{
			int InputIDForGate1, InputIDForGate2;
			
			Gate* Gate1 = getGateIDFromSerial( s1, &InputIDForGate1 );
			Gate* Gate2 = getGateIDFromSerial( s2, &InputIDForGate2 );

			if( Gate1 == null || Gate2 == null ) { cout << "ERROR GAtes"; system( "pause" ); }

			InputIDForGate2 = Gate2->numInputs - InputIDForGate2;
			
			//cout << InputIDForGate1 << InputIDForGate2 << endl;
			//cout << Gate2->id << ": " << Gate2->GateType << " - " << Gate2->numInputs << endl;
			delete Gate2->pInputs[InputIDForGate2];
			//Gate2->pInputs = null;

			if( InputIDForGate1 == 0 ) // Is an Output port?
			{
				Gate2->pInputs[InputIDForGate2] = Gate1->pOutput;

				Gate1->pOutput->pParent = Gate1;
				Gate1->pOutput->pChild = Gate2;
				Gate1->pOutput->isDirectInput = false;
			}
			else
			{
				InputIDForGate1 = Gate1->numInputs - InputIDForGate1 - 1;
				Gate2->pInputs[InputIDForGate2] = Gate1->pInputs[InputIDForGate1];

				Gate1->pInputs[InputIDForGate1]->pParent = Gate1;
				Gate1->pInputs[InputIDForGate1]->pChild = Gate2;
				Gate1->pInputs[InputIDForGate1]->isDirectInput = false;
			}
		}

		void showDirectInputGates(int outputS)
		{
			Gate* ptr = pHead;
			int Serial = 0;
			outputSerial = outputS;
			bool bhasDirectPort = false;

			if( ptr != null )
			{
				while( ptr != null )
				{
					//cout << ptr->id << ": " << ptr->GateType << " - " << endl;
					for( int i = 0; i < ptr->numInputs; i++ )
					{
						//printf( "%d\n", i );
						if( (ptr->pInputs[i]->isDirectInput) && (Serial != outputS) )
						{
							//cout << Serial;
							printf( "%d ", (Serial+1) );
							bhasDirectPort = true;
							numDirectPorts++;
						}
						Serial++;
					}
					if( bhasDirectPort )
						printf( "- %s(%d)\n", ptr->GateType, ptr->id );

					Serial++; // Compensation for pOutput
					ptr = ptr->nextGate;
					bhasDirectPort = false;
				}
			}
		}

		void assignDirectInputValues()
		{
			Gate* ptr = pHead;
			int Serial = 0;
			int value;

			while( ptr != null )
			{
				for( int i = 0; i < ptr->numInputs; i++ )
				{
					if( (ptr->pInputs[i]->isDirectInput) && (Serial != outputSerial) )
					{
						scanf( "%d", &value );
						while( !(value == 0 || value == 1) )
						{
							printf( "Enter 0 or 1 only \n" );
							scanf( "%d", &value );
						}
						ptr->pInputs[i]->val = value;
						ptr->pInputs[i]->isDefined = DEFINED_DIRECT;
					}
					Serial++;
				}

				Serial++; // Compensation for pOutput
				ptr = ptr->nextGate;
			}
		}

		void assignDirectInputValues(int* inputarray)
		{
			Gate* ptr = pHead;
			int Serial = 0;
			int j = 0;

			while( ptr != null )
			{
				for( int i = 0; i < ptr->numInputs; i++ )
				{
					if( (ptr->pInputs[i]->isDirectInput) && (Serial != outputSerial) )
					{
						ptr->pInputs[i]->val = inputarray[j++];
						ptr->pInputs[i]->isDefined = DEFINED_DIRECT;
					}
					Serial++;
				}

				Serial++; // Compensation for pOutput
				ptr = ptr->nextGate;
			}
		}

		int calcOutput()
		{
			Gate* ptr = pHead;
			while( ptr != null )
			{
				for( int i = 0; i < ptr->numInputs; i++ )
				{
					if( ptr->pInputs[i]->isDefined == DEFINED_BY_CALC )
					{
						ptr->pInputs[i]->isDefined = NOT_DEFINED;
					}
				}
				ptr = ptr->nextGate;
			}

			int dummy; // TODO : Overload GetID func
			Gate* outputGate = getGateIDFromSerial(outputSerial, &dummy);
			return outputGate->GetOutput();
		}

		void printAllPossibleOutputs()
		{
			int temp;
			int* arrayInputs = new int [numDirectPorts];
			memset( (void *)arrayInputs, 0, numDirectPorts*sizeof(int) );

			int j = numDirectPorts-1;

			for( int i = 0; i < pow(2, numDirectPorts); i++ )
			{
				temp = i;
				while( temp != 0 )
				{
					arrayInputs[j--] = (temp & 1);
					temp >>= 1;
				}

				printf( "\t\t" );
				for( int k = 0; k < numDirectPorts; k++ )
				{
					printf( "%d ", arrayInputs[k] );
				}
				assignDirectInputValues(arrayInputs);
				printf( " | %d\n", calcOutput() );

				j = numDirectPorts-1;
				memset( (void *)arrayInputs, 0, numDirectPorts*sizeof(int) );
			}
		}
};

int main()
{
    GateManager gtMngr;

	printf( "\tWelcome to Digital Circuit Simulator v1.0\n\n \
To create your own circuit the procedure is \n \
	1. Specify gate types and number of inputs \n \
	2. Specify connections between different ports of gates \n \
	3. Specify the output port \n \
	4. Truth table will be printed, you can also try giving our own inputs \n\n\n" );

    int gtType, numinpts;
	printf( "1. Enter GateType (1 - And, 0 - Or) and number of inputs for that gate Eg. 0 2 \nTo Go to next Step type any value other than 1,0 as gate type\n\n" );
	do
	{
		printf( "Enter GateType (1 - And, 0 - Or) and number of inputs for that gate : " );
		scanf( "%d %d", &gtType, &numinpts );
		if( gtType > 1 || gtType < 0 ) break;
		gtMngr.addGate(gtType, numinpts);
	}
	while(1);
	
	gtMngr.printDataPoints();
	
	
	printf( "\n2. To Proceed to next Step, type self refering connections, eg 1 1 or 0 0 or 3 3 \n\n" );
	int s1, s2;
	do
	{
		printf( "Enter connections (Eg. '1 2' connects 1 and 2 points) : " );
		scanf( "%d %d", &s1, &s2 );
		if( s1 == s2 ) break;
		gtMngr.ConnectDataPoints(s1, s2);
	}
	while(1);

	int outputSerial;
	printf( "\n3. Enter Output Serial for the circuit : " );
	scanf( "%d", &outputSerial );

	printf( "\nInput required for the following gates : \n" );
	gtMngr.showDirectInputGates(outputSerial);

	printf( "\nTruth table (with all possible combinations) :\n" );
	gtMngr.printAllPossibleOutputs();

	printf( "\n\n4. Want to try out your own values ? Enter input in form x x x x... \nupto number of inputs \n" );
	while(1)
	{
		printf( "Enter Input Values : " );
		gtMngr.assignDirectInputValues();

		printf( "Output : %d\n", gtMngr.calcOutput() );
	}

	//system( "pause" );
}
