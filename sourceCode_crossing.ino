
#include <Arduino_FreeRTOS.h>
#include <semphr.h> // Este inclusa biblioteca semhr.h pentru utilizarea semafoarelor

// Definire task-uri ------------------------------------------

void TaskAnalogReadAlb( void *pvParameters );
void TaskAnalogReadRosu( void *pvParameters );

void TaskSemaphoreAlb( void *pvParameters );
void TaskSemaphoreRosu( void *pvParameters );

void TaskAfisareAlb( void *pvParameters );
void TaskAfisareRosu( void *pvParameters );

//----------------------------------------------------------------
// Definim handle-uri pentru semafoare, necesare identificarii ulterioare
SemaphoreHandle_t sem1;
SemaphoreHandle_t sem2;
SemaphoreHandle_t sem3;
SemaphoreHandle_t sem4;
SemaphoreHandle_t sem5;
SemaphoreHandle_t sem6;
SemaphoreHandle_t sem7;
//--------------------------------------------------------------
const int trigPinAlb = 9;
const int echoPinAlb = 10;

const int trigPinRosu = 2;
const int echoPinRosu = 3;

const int boardRosuRosu = 11;
const int boardRosuGalben = 12;
const int boardRosuVerde = 13;

const int boardAlbRosu = 6;
const int boardAlbGalben = 5;
const int boardAlbVerde = 4;

float  distanceAlb, distanceRosu;

//---------------------------------------------------------------
// the setup function runs once when you press reset or power the board
void setup() { 

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  distanceAlb= 30;
  distanceRosu= 30;
  // Cream  semafoare binare
  sem1 = xSemaphoreCreateBinary();
  sem2 = xSemaphoreCreateBinary();
  sem3 = xSemaphoreCreateBinary();
  sem4 = xSemaphoreCreateBinary();
  sem5 = xSemaphoreCreateBinary();
  sem6 = xSemaphoreCreateBinary();
  sem7 = xSemaphoreCreateBinary();
// Semafoarele sunt default 0
xSemaphoreGive(sem1);

 

  // Sunt create taskuri, fiecare cu 128 de cuvinte de memorie rezervate si prioritate egala cu 1
  
  xTaskCreate(
    TaskAnalogReadAlb
    ,  "task2"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

   xTaskCreate(
    TaskAnalogReadRosu
    ,  "task3"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

   xTaskCreate(
    TaskSemaphoreAlb
    ,  "task4"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
    
    xTaskCreate(
    TaskSemaphoreRosu
    ,  "task5"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

   xTaskCreate(
    TaskAfisareAlb
    ,  "task6"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

   xTaskCreate(
    TaskAfisareRosu
    ,  "task7"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

   vTaskStartScheduler(); // Este pornit algoritmul de planificare
}
//----------------------------------------------------------------------
//--------------loop--------------------------------------------------
void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
//-------------------------------------------------------------------
void TaskAnalogReadRosu(void *pvParameters)  // This is a task.
{  
  pinMode(trigPinRosu, OUTPUT);
  pinMode(echoPinRosu, INPUT);
  float durationRosu;
  while(1)
  {
    
    if (xSemaphoreTake(sem1, portMAX_DELAY) == pdTRUE) {
    
      Serial.println(" ");
      Serial.println("TaskAnalogReadRosu");
      digitalWrite(trigPinRosu, LOW);
      vTaskDelay(2);
      digitalWrite(trigPinRosu, HIGH);
      vTaskDelay(10);
      digitalWrite(trigPinRosu, LOW);

      durationRosu = pulseIn(echoPinRosu, HIGH);
      distanceRosu = (durationRosu*.0343)/2;
      xSemaphoreGive(sem2);
      vTaskDelay( 50 );
      
      if(distanceRosu<10){
      xSemaphoreGive(sem3);
      vTaskDelay( 50 );
      }
      else{
        xSemaphoreGive(sem1);
        vTaskDelay( 50 );
      }
    }
    vTaskDelay( 200 );
  }
}
//-----------------------------------------------------------------
void TaskAnalogReadAlb(void *pvParameters)  // This is a task.
{
  pinMode(trigPinAlb, OUTPUT);
  pinMode(echoPinAlb, INPUT);
  while(1)
  {
    float durationAlb;
    if (xSemaphoreTake(sem3, portMAX_DELAY) == pdTRUE) {
      
      Serial.println("TaskAnalogReadAlb");
      digitalWrite(trigPinAlb, LOW);
      vTaskDelay(2);
      digitalWrite(trigPinAlb, HIGH);
      vTaskDelay(10);
      digitalWrite(trigPinAlb, LOW);

      durationAlb = pulseIn(echoPinAlb, HIGH);
      distanceAlb = (durationAlb*.0343)/2;
      xSemaphoreGive(sem4);
      vTaskDelay( 50 );
      
      if(distanceAlb>10){
      xSemaphoreGive(sem5);
      vTaskDelay( 50 );
      }
      else{
        xSemaphoreGive(sem1);
        vTaskDelay( 50 );
      }
    
    }
    vTaskDelay( 100 );  
  }  
}


//---------------------------------------------------------------
void TaskAfisareRosu(void *pvParameters)  // This is a task.
{

   while(1)
  {
    
    if (xSemaphoreTake(sem2, portMAX_DELAY) == pdTRUE) {
      Serial.println("TaskAfisareRosu");
      Serial.print("DistanceRosu: ");
      Serial.println(distanceRosu);
    }
    vTaskDelay( 100 );  
  }
}

//---------------------------------------------------
void TaskAfisareAlb(void *pvParameters)  // This is a task.
{

   while(1)
  {
    
    if (xSemaphoreTake(sem4, portMAX_DELAY) == pdTRUE) {
      Serial.println("TaskAfisareAlb");
      Serial.print("DistanceAlb: ");
      Serial.println(distanceAlb);
    }
    vTaskDelay( 100 );  
  }
}

//-------------------------------------------------------------
void TaskSemaphoreAlb(void *pvParameters){
  pinMode(boardAlbRosu, OUTPUT);
  pinMode(boardAlbGalben, OUTPUT);
  pinMode(boardAlbVerde, OUTPUT);

  digitalWrite(boardAlbRosu, LOW);
  digitalWrite(boardAlbGalben, LOW);
  digitalWrite(boardAlbVerde, HIGH);
  
  while (1)
  {
    
    if (xSemaphoreTake(sem5, portMAX_DELAY) == pdTRUE) {
      
      Serial.println("TaskSemaphoreAlbPartea1");
      digitalWrite(boardAlbRosu, LOW);
      digitalWrite(boardAlbGalben, HIGH);
      digitalWrite(boardAlbVerde, HIGH);
      vTaskDelay(100);
      digitalWrite(boardAlbRosu, HIGH);
      digitalWrite(boardAlbGalben, LOW);
      digitalWrite(boardAlbVerde, LOW);
      xSemaphoreGive(sem6);
      vTaskDelay( 50 );  
    }
    
    if (xSemaphoreTake(sem7, portMAX_DELAY) == pdTRUE) { 
      Serial.println("TaskSemaphoreAlbPartea2");
      digitalWrite(boardAlbRosu, LOW);
      digitalWrite(boardAlbGalben, LOW);
      digitalWrite(boardAlbVerde, HIGH);

      xSemaphoreGive(sem1);
      vTaskDelay( 50 ); 
    }
  vTaskDelay( 100 ); 
  }
}

//---------------------------------------------------------------
void TaskSemaphoreRosu(void *pvParameters){
  pinMode(boardRosuRosu, OUTPUT);
  pinMode(boardRosuGalben, OUTPUT);
  pinMode(boardRosuVerde, OUTPUT);

  digitalWrite(boardRosuRosu, HIGH);
  digitalWrite(boardRosuGalben, LOW);
  digitalWrite(boardRosuVerde, LOW);
  
  while (1)
  {
     
    if (xSemaphoreTake(sem6, portMAX_DELAY) == pdTRUE) {
      Serial.println("TaskSemaphoreRosu");
      digitalWrite(boardRosuRosu, LOW);
      digitalWrite(boardRosuGalben, LOW);
      digitalWrite(boardRosuVerde, HIGH);
      vTaskDelay(500);
      digitalWrite(boardRosuGalben, HIGH);
      vTaskDelay(100);
      digitalWrite(boardRosuRosu, HIGH);
      digitalWrite(boardRosuGalben, LOW);
      digitalWrite(boardRosuVerde, LOW);
      xSemaphoreGive(sem7);
      vTaskDelay( 50 );
    }
    vTaskDelay( 100 );  
  }
}
//------------------------------------
