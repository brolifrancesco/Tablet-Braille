char ch;
byte ncella=0;
byte cont;
byte out;

byte braille(){                   /*questa funzione si occupa di convertire la lettera scritta in braille
                                    in 8 bit con i quali verranno poi eseguite le varie operazioni 
                                    i primi 2 bit sono in più ed il punto 1 corrispode al 6 bit e così via*/
  switch (ch){                      
    case 'a':                       
      out=0b00100000; 
      break;
    case 'b':
      out=0b00110000;
      break;
    case 'c':
      out=0b00100100;
      break; 
    case 'd':
      out=0b00100110;
      break;                    
    case 'e':
      out=0b00100010;
      break;
    case 'f':
      out=0b00110100;
      break;
    case 'g':
      out=0b00110110;
      break;
    case 'h':
      out=0b00110010;
      break;
    case 'i':
      out=0b00010100;
      break;
    case 'j':
      out=0b00010110;
      break;
    case 'k':
      out=0b00101000;
      break;
    case 'l':
      out=0b00111000;
      break;
    case 'm':
      out=0b00101100;
      break;
    case 'n':
      out=0b00101110;
      break;
    case 'o':
      out=0b00101010;
      break;
    case 'p':
      out=0b00111100;
      break;
    case 'q':
      out=0b00111110;
      break;
    case 'r':
      out=0b00111010;
      break;
    case 's':
      out=0b00011100;
      break;
    case 't':
      out=0b00011110;
      break;
    case 'u':
      out=0b00101001;
      break;
    case 'v':
      out=0b00111001;
      break;
    case 'w':
      out=0b00010111;
      break;
    case 'x':
      out=0b00101101;
      break;
    case 'y':
      out=0b00101111;
      break;
    case 'z':
      out=0b00101011;
      break;        
    case ' ':
      out=0b00000000;                                         
  }
    return out;
}


void scrittura(){                     /*questa funzione si ouccpa dell'output fisico del prototipo*/
  
  PORTB=0b00011000;                   /*per la comunicazione vengono utilizzati gli output 9-16 del demux
                                        per questo impostiamo da subito il pin 10 high così da posizionarci
                                        sul pin 9 e contanto i su*/
                                        
  for(int i=0;i<8;i++){
  
    if(out & 1 == 1){                 /*si esegue un AND logico tra il valore della variabile out
                                        e 1 così da verificare se il bit che si sta leggendo vale 1 o 0
                                        */
      PORTB=PORTB-16;                               
      Serial.print("1");              /*nel caso il bit valesse 1 allora inviamo 1 sul bus dati*/
      delay(4000);
      PORTB=PORTB+16;
                                       
    }
    else{
      Serial.print(0);
      }
   PORTB=PORTB+1;                     /*avanziamo il selettore di 1 passando al prossimo pin del DEMUX*/
   delay(1000);
   out=out>>1;                        /* shiftiamo il valore della variabile out di 1 verso destra per
                                         passare al prossimo bit da leggere */
   
  }
}

void attivacella(){                 /*questa funzione si occupa di selezionare la cella con cui comunicare*/
  
 cont=ncella-1;
  for(int i=0;i<8;i++){       
  Serial.print(cont);
    if(cont & 1 == 1  ){           /*vengono eseguiti gli stessi step della funzione "scrittura"
                                     partendo però dal pin 0 del DEMUX*/
      PORTB=PORTB-16;
      delay(40);
      PORTB=PORTB+16;
    }
  
    PORTB=PORTB+1;               
    delay(300);
    cont=cont>>1;            
  }
  PORTB=0b00010000;

}


void reset(){                       /*la funzione reset si occupa di utilizzare l'ultimo output del DEMUX
                                      per resettare i registri di tutte le celle per permette
                                      una nuova comunicazione*/
  PORTB=0b00001111; 
  delay(1000);
  PORTB=0b00010000;
  return 0;
  }


void setup() {

DDRB = B00111111;                    /*durante tutto il programma viene utilizzata la port manipulation
                                       dell'arduino. In modo molto semplificato questo comando DDRB permette
                                       di controllare l'output delle porte 8-13 tramite una variabile.
                                       Utilizzando il comando DDRB nel void setup() si impostano i pin
                                       come input o output.*/                                       
Serial.begin(9600);
}
 
void loop() {

 reset();
 while (Serial.available() == 0) {   
}

 ch=Serial.read();
 delay(1000);
 Serial.println(ch);
  
 ncella=ncella+1;
 attivacella();
 braille();
 scrittura();

}
