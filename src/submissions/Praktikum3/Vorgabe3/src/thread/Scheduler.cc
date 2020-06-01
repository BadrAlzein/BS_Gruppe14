#include "thread/Scheduler.h"
#include "io/PrintStream.h"
#include "interrupts/IntLock.h"



void Scheduler::schedule(Schedulable *sched) {
    readylist.enqueue((Chain*)sched);
}


void Scheduler::remove(Schedulable *sched) {
    readylist.remove((Chain*) sched);
}

void Scheduler::reschedule() {
    Schedulable* head = (Schedulable *) readylist.dequeue();
	this->activate(head);
}

void Scheduler :: checkSlice(){
	//redundanter IntLock weil schon in scheduler geschützt
	//IntLock safe;
	/*
	 *	von Clock.cc
	 *	Wenn der Uhrentest erfolgreich verlaufen ist, kommentiert
	 *	den Propeller wieder aus und ruft stattdessen
	 *	die "checkSlice" Methode des Schedulers auf,
	 *	um diesen ggf. zum praeemptiven Rescheduling zu veranlassen.
	 *  Dem aktiven Prozess soll erst dann die CPU entzogen werden, wenn sein Quantum
     *	abgelaufen ist. Führe dazu eine Methode checkSlice () beim Scheduler ein, die überprüft, ob die
	 *	Zeitscheibe des aktuell laufenden Prozesses abgelaufen ist.
	 */
	Schedulable *active = (Schedulable *) getCurrentActivity();
	if (active->quantum() == clock.ticks())
	{
		clock.resetTicks(); //<= BRISHNA
		reschedule(); //reschedule aufrufen, wenn Zeitscheibe(Quantum) abgelaufen ist
	}
}
