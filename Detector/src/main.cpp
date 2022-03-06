#include <wiringPi.h>
#include <stdio.h>

#include "CircularBuffer.hpp"
#include "state.h"
#include "config.hpp"

int main(void)
{
	wiringPiSetupSys();

	// set pin modes
	system("gpio export 25 in");
	system("gpio export 24 in");

	Config cfg("config.ini");

	State state;

	CircularBuffer<State> referenceBuffer_Enter("REFERENCE", 4, State::INNER, State::MIDDLE, State::OUTER, State::FINISH_OP);
	CircularBuffer<State> referenceBuffer_Leave("REFERENCE", 4, State::OUTER, State::MIDDLE, State::INNER, State::FINISH_OP);
	CircularBuffer<State> stateBuffer(4);

	while (true)
	{
		int sensor1 = digitalRead(24);
		int sensor2 = digitalRead(25);

		if (!sensor1 && sensor2)
		{
			state = State::INNER;
		}
		else if (!sensor1 && !sensor2)
		{
			state = State::MIDDLE;
		}
		else if (sensor1 && !sensor2)
		{
			state = State::OUTER;
		}
		else if (sensor1 && sensor2)
		{
			if (stateBuffer.Peek() == State::OUTER || stateBuffer.Peek() == State::INNER)
				state = State::FINISH_OP;
			else
				state = State::EMPTY;
		}

		if (state == State::EMPTY || state != stateBuffer.Peek())
			stateBuffer.Insert(state);

		if (stateBuffer.Compare(referenceBuffer_Enter))
		{
			puts("\t\t\t\t\t<< Enter");
		}
		else if (stateBuffer.Compare(referenceBuffer_Leave))
		{
			puts("\t\t\tLeave >>");
		}

		delay(10);
	}
	return 0;
}