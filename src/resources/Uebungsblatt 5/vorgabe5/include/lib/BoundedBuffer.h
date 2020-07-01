#ifndef BoundedBuffer_h
#define BoundedBuffer_h

#include "interrupts/IntLock.h"
#include "thread/Activity.h"
#include "thread/ActivityScheduler.h"

/** Diese Klasse stellt einen begrenzten synchronisierten
 *  Puffer dar, welcher von Interruptbehandlungsroutinen
 *  gef�llt werden kann.
 *
 *  Die Klasse ist eine Templateklasse. Das bedeutet f�r euch
 *  erstmal nur, das ihr alle Methoden hier im Header implementieren
 *  m�sst.
 */
template<typename T,unsigned size>
class BoundedBuffer {
public:

	BoundedBuffer()
	{
		validElems = 0;
		readPos = 0;
		waiting = 0;
	}

	/** Diese Methode wird vom Interrupthandler aufgerufen, um
	 *  ein etwas in den Puffer zu schreiben. Ist der Puffer voll,
	 *  werden die zu schreibenden Daten verworfen.
	 *  Prozesse die auf eine Eingabe warten m�ssen hier geweckt werden.
	 */
	void add(T& elem)
	{
		if(validElems < size){
			buffer[(readPos+validElems)%size] = elem;
			validElems++;
		}
		if(waiting){
			waiting->wakeup();
			waiting = 0;
		}
	}

	/** Diese Methode wird von Prozessen aufgerufen, um Daten aus dem
	 *  Puffer zu lesen. Ist dieser leer wird der lesende Prozess schlafen
	 *  gelegt. Achtet hier besonders auf die Synchronisierung.
	 */
	T get()
	{
		if(!validElems){
			waiting = (Activity*)(scheduler.active());
			waiting->sleep();
		}

		validElems--;
		unsigned pos = readPos;
		readPos = (readPos + 1) % size;
		return buffer[pos];
	}

private:
	T buffer[size];
	unsigned readPos, validElems;
	Activity* waiting;
};

#endif

