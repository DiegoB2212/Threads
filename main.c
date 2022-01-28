// INTEGRANTES: DIEGO BARRERA, DANIEL RINCON Y ANGELA ROJAS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAIN 0
#define CRONO 1
#define TEMPO 2
#define ALARM 3
#define OFF 4

pthread_t horaLocal, printIU, options, cronometro, temporizador, alarm_;
int appState = MAIN;
struct tm getedHour[16];

int horaCronometro[3] = {40,59,0};
char cronoState = '0';

int horaTemporizador[2] = {0,0};
char tempState = '0';

int horaAlarma[2] = {1,2};
char alarmState = '0';

void *LocalTime(void *arg);
void *printMenu(void *arg);
void *apps(void *arg);
void *cronometer(void *arg);
void *timer (void *arg);
void *despertador(void *arg);

void main()
{
	if ( pthread_create(&horaLocal, NULL, LocalTime, NULL) != 0 )
   	{	   
      printf("Error hilo 1\n");
      exit(1);
   	}	   
	else if ( pthread_create(&printIU, NULL, printMenu, &getedHour)!=0 )
   	{	   
      printf("Error hilo 2\n");
      exit(2);
   	}	   
	else if ( pthread_create(&options, NULL, apps, NULL)!=0 )
   	{	   
      printf("Error hilo 3\n");
      exit(3);
   	}	  
	else if ( pthread_create(&cronometro, NULL, cronometer, NULL)!=0 )
   	{	   
      printf("Error hilo 4\n");
      exit(4);
   	} 
	else if ( pthread_create(&temporizador, NULL, timer, NULL)!=0 )
   	{	   
      printf("Error hilo 5\n");
      exit(5);
   	}   
	else if ( pthread_create(&alarm_, NULL, despertador, NULL)!=0 )
   	{	   
      printf("Error hilo 6\n");
      exit(6);
   	}  
	else 
	{
		pthread_join(options, NULL);
		sleep(1);
		pthread_join(horaLocal, NULL);
		pthread_join(cronometro, NULL);
		pthread_join(temporizador, NULL);
		pthreadd_join(alarm_, NULL);
		pthread_join(printIU, NULL);
	}

	if(appState == OFF)
	{
		system("clear");
		printf("PROGRAMA FINALIZADO <3\n");
	}
}

void *LocalTime(void *arg)
{
	time_t fecha;
   	struct tm *temp;
   	struct tm fecha_tm;

	while(appState != OFF)
	{
		fecha = time(NULL); 
   		temp = localtime(&fecha);
   		memcpy(&fecha_tm, temp, sizeof fecha_tm);
		getedHour[0] = fecha_tm;

		getedHour[1].tm_hour = getedHour[0].tm_hour + 6;
		getedHour[1].tm_min = getedHour[0].tm_min;
		getedHour[1].tm_sec = getedHour[0].tm_sec;

		if(getedHour[1].tm_hour >= 24) getedHour[1].tm_hour -= 24;

		getedHour[2].tm_hour = getedHour[0].tm_hour + 5;
		getedHour[2].tm_min = getedHour[0].tm_min;
		getedHour[2].tm_sec = getedHour[0].tm_sec;

		if(getedHour[2].tm_hour >= 24) getedHour[2].tm_hour -= 24;
	}
}

void *printMenu(void *arg)
{
	while(appState != OFF)
	{
		struct tm *localHour = (struct tm *) arg;
		if(appState == MAIN)
		{
			printf("        %02d:%02d:%02d\n", localHour[0].tm_hour, localHour[0].tm_min, localHour[0].tm_sec);
			printf("++++++++++++++++++++++++++++++++\n");	
			printf(" BOGOTA:        %02d:%02d:%02d\n", localHour[0].tm_hour, localHour[0].tm_min, localHour[0].tm_sec);
			printf(" BARCELONA:     %02d:%02d:%02d\n", localHour[1].tm_hour, localHour[1].tm_min, localHour[1].tm_sec);
			printf(" LONDRES:       %02d:%02d:%02d\n", localHour[2].tm_hour, localHour[2].tm_min, localHour[2].tm_sec);
			printf("++++++++++++++++++++++++++++++++\n");
			printf(" Cronometro - Alarma - Temporizador\n");	
		}
		else if(appState == CRONO)
		{
			printf("        CRONOMETRO\n");
			printf("++++++++++++++++++++++++++++\n");
			printf(" (1). Empezar\n (2). Pausar\n (3). Reiniciar\n (m) Regresar\n");
			printf("++++++++++++++++++++++++++++\n");
			printf("     %02d : %02d : %02d \n", horaCronometro[2], horaCronometro[1], horaCronometro[0]);
		}
		else if(appState == TEMPO)
		{
			printf("        TEMPORIZADOR\n");
			printf("++++++++++++++++++++++++++++\n");
			printf(" (1). Empezar\n (2). Pausar\n (3). Aumentar 10 Segundos\n (4). Disminuir 10 Segundos\n (r). Reiniciar\n (m) Regresar\n");
			printf("++++++++++++++++++++++++++++\n\n");
			printf("         %02d : %02d \n", horaTemporizador[1], horaTemporizador[0]);
			if(horaTemporizador[1] == 0 && horaTemporizador[0] == 0) printf("TEMPORIZADOR FINALIZADO!!!!\n");
		}
		else if(appState == ALARM)
		{
			printf("        ALARMA\n");
			printf("+++++++++++++++++++++++++++\n");
			printf("  Hora Alarma = %02d : %02d : %02d\n", horaAlarma[0], horaAlarma[1], getedHour[0].tm_sec);
			printf("  Hora Actual = %02d : %02d : %02d\n", getedHour[0].tm_hour, getedHour[0].tm_min, getedHour[0].tm_sec);
			printf("+++++++++++++++++++++++++++\n");
			printf(" (1 - q). 10min\n (2 - w). 1min\n (3 - e). 10 seg\n (4 - r). 1 seg\n (m). Regresar\n\n");
			if(horaAlarma[0] == getedHour[0].tm_hour && horaAlarma[1] == getedHour[0].tm_min) printf("ALARMA!!!!!\n");
		}
		sleep(1);
		system("clear");
	}
}

void *apps(void *arg)
{
	char funcition = '0';	
	while(appState != OFF)
	{
		scanf("%c", &funcition);
		if(funcition == 'c' || funcition == 't' || funcition == 'a'|| funcition == 'm' || funcition == 'o')
		{
			switch (funcition)
			{
				case 'c': appState = CRONO; break;	 
				case 't': appState = TEMPO; break;
				case 'a': appState = ALARM; break;
				case 'm': appState = MAIN; break;
				case 'o': appState = OFF; break;
			}
		}
		else
		{
			if(appState == CRONO)
			{
				switch (funcition)
				{
					case '1': cronoState = '1'; break; // INICIAR	 
					case '2': cronoState = '2'; break; // PAUSASR
					case '3': cronoState = '3'; break; // REINICIAR
				}
			}
			else if(appState == TEMPO)
			{
				switch(funcition)
				{
					case '1': tempState = '1'; break; // INICIAR
					case '2': tempState = '2'; break; // PAUSAR
					case '3': tempState = '3'; break; // SUMA 10 SEG
					case '4': tempState = '4'; break; // RESTA 10 SEG
					case 'r': tempState = 'r'; break; // REINICIAR
				}

			}
			else if(appState == ALARM)
			{
				switch(funcition)
				{
					case '1': alarmState = '1'; break; // HORA + 10
					case 'q': alarmState = 'q'; break; // HORA - 10

					case '2': alarmState = '2'; break; // HORA + 1
					case 'w': alarmState = 'w'; break; // HORA - 1

					case '3': alarmState = '3'; break; // MINUTOS + 10
					case 'e': alarmState = 'e'; break; // MINUTOS - 10

					case '4': alarmState = '4'; break; // MINUTOS + 1
					case 'r': alarmState = 'r'; break; // MINUTOS - 1
				}
			}
		}
	}
}

void *cronometer(void *arg)
{
	while(appState != OFF)
	{
		switch(cronoState)
		{
			case '1': //ACTIVAR 
				horaCronometro[0] += 1;
				if(horaCronometro[0] == 60)
				{
					horaCronometro[0] = 0;
					horaCronometro[1] += 1;
					if(horaCronometro[1] == 60)
					{
						horaCronometro[1] = 0;
						horaCronometro[2] += 1; 

						if(horaCronometro[2] == 60)
						{
							cronoState = '3';
						}
					}
				}
				sleep(1);
			break;

			case '2': // PAUSAR
			break;

			case '3': // REINICIAR
				horaCronometro[0] = 0;
				horaCronometro[1] = 0;
				horaCronometro[2] = 0;
			break;
		}
	}
}

void *timer (void* arg)
{
	while(appState != OFF)
	{
		switch(tempState)
		{
			case '1': // INICIAR
				
				if(horaTemporizador[0] == 0)
				{
					if(horaTemporizador[1] == 0 && horaTemporizador[0] == 0)
					{
						horaTemporizador[1] = 0;
						horaTemporizador[2] = 0;
						tempState = '2'; 
						appState = TEMPO;
					}
					else
					{
						horaTemporizador[0] = 59;
						horaTemporizador[1] -= 1;
					}
				}
				else horaTemporizador[0] -= 1;
				sleep(1);
			break;

			case '2': //PAUSAR
			break;

			case '3': //SUMAR 10 SEG
				if(horaTemporizador[0] == 50 && horaTemporizador[1] != 60)
				{
					horaTemporizador[0] = 0;
					horaTemporizador[1] += 1;

					if(horaTemporizador[1] == 60)
					{
						tempState = '2';
					}
					else tempState = '2';
				}
				else
				{
					if(horaTemporizador[1] != 60)
					{
						horaTemporizador[0] += 10;
						tempState = '2';						
					}

				}
			break;

			case '4': // RESTAR 10 SEG
				if(horaTemporizador[0] == 0 && horaTemporizador[1] != 0)
				{
					horaTemporizador[0] = 50;

					if(horaTemporizador[1] == 0)
					{
						tempState = '2';
					}
					else
					{
						horaTemporizador[1] -= 1;
						tempState = '2';
					}
				}
				else
				{
					if(horaTemporizador[0] != 0)
					{
						horaTemporizador[0] -= 10;
						tempState = '2';
					}

				}
			break;

			case 'r': // REINICIAR
				horaTemporizador[0] = 0;
				horaTemporizador[1] = 0;
			break;
		}
	}
}

void *despertador(void* arg)
{
	while(appState != OFF)
	{
		switch(alarmState)
		{
			// HORAS
			case '1':
				horaAlarma[0] += 10;
				alarmState = 'x';
			break;

			case 'q':
				horaAlarma[0] -= 10;
				alarmState = 'x';
			break;

			case '2':
				horaAlarma[0] += 1;
				alarmState = 'x';
			break;

			case 'w':
				horaAlarma[0] -= 1;
				alarmState = 'x';
			break;

			//MINUTOS
			case '3':
				horaAlarma[1] += 10;
				alarmState = 'x';
			break;

			case 'e':
				horaAlarma[1] -= 10;
				alarmState = 'x';
			break;

			case '4':
				horaAlarma[1] += 1;
				alarmState = 'x';
			break;

			case 'r':
				horaAlarma[1] -= 1;
				alarmState = 'x';
			break;
		}

		if(horaAlarma[0] == getedHour[0].tm_hour && horaAlarma[1] == getedHour[0].tm_min) appState = ALARM;
	}
}
