#ifndef Console_h
#define Console_h

#include "io/InputChannel.h"
#include "io/OutputChannel.h"
#include "system/Semaphore.h"

/**
 *  Console:	Die Systemkonsole
 * Um solche Unterbrechungen zu vermeiden, wird als Ein-/Ausgabeschnittstelle eine Console im-
plementiert. Der Zugriff auf die Console darf erst erfolgen, nachdem diese reserviert (attach)
wurde. Nach der Benutzung muß die Console wieder freigegeben werden (detach). Die innere
Synchronisation erfolgt dabei über eine Semaphore.
 */
class Console : public InputChannel, public OutputChannel
{
public:
	/**
 * Die innere
Synchronisation erfolgt dabei über eine Semaphore.

Benutzerprogramme, die die Console verwenden, befinden sich nicht im Kern. Für sie ist die
Nutzung der KernelSemaphore damit nicht angebracht. D
 */
	//binaere Semaphore
	Console(InputChannel &input, OutputChannel &output)
		: input(input), output(output), onOff(Semaphore(1)) {
		} 

	/** 	Konsole reservieren
	 */
	void attach();

	/** 	Konsole wieder freigeben
	 */
	void detach();

	/** 	Daten auf der Console ausgeben
	 */
	virtual int write(const char *data, int size);

	/** 	Jedes von der Tastatur eingelesene Zeichen
	 * 	soll auf dem Bildschirm dargestellt werden (Echo Funktion!)
	 * 	Wenn ein '\n' erkannt wurde oder bereits "size" Zeichen
	 *	gelesen wurden, ist eine Zeile zuende und read() kehrt zurueck.
	 *	Alle gelesenen Ascii-zeichen sind in den Puffer einzutragen
	 *	und zaehlen als gueltiges Zeichen!
	 */
	virtual int read(char *data, int size);

	/** 	Liefert das n�chste Zeichen aus dem Eingabepuffer zur�ck.
	 */
	char read();

	/**
     * print bluescreen
     */
	virtual void blueScreen(const char *msg)
	{
		output.blueScreen(msg);
	}

private:
	InputChannel &input;
	OutputChannel &output;
	Semaphore onOff;
};

#endif
