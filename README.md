# VCO-Tuner

This program is used for a voltage-controlled-oscillator in a modular synthesizer. I used an ATMega-328 programmed with AVR C. I used the interrupt on the ATMega to start a timer which counted the period of the pulse on the oscilation input into it. I then used logarithms to convert the period counted to a musical note which could be subtracted from to find the note independent of octave.
