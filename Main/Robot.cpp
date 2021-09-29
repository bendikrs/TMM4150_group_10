// #include "Gripper.h"
// #include "Driver.h"
#include "Robot.h"


Robot::Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray, SyncDriver controller):
stepper_left(stepper_left), stepper_right(stepper_right), irArray(irArray), controller(controller)
{
    
    // stepper_right.begin(speed2rpm(this->speed), 1);
    // stepper_left.begin(speed2rpm(this->speed), 1);

    // stepper_right.enable(); // energize coils
    // stepper_left.enable();

    // stepper_left.setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, 500, 500);
    // stepper_right.setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, 500, 500);

    //stepper_left.nextAction
}

int Robot::checkIfWorking(){
    int avgIR;
    for (int i=1; i<3000; i++){
        avgIR = this->irArray.getAverage();
        
        
        delay(2);
    }
}

float Robot::speed2rpm(int _speed){
    /* speed [mm/s]
    output: RPM
    */
    return (_speed*60)/(3.141592*this->diameterDriveWheels);
}

void Robot::setSpeed(int _speed){
    this->speed = _speed;
    this->controller.getMotor(0).setRPM(speed2rpm(_speed));
    this->controller.getMotor(1).setRPM(speed2rpm(_speed));
}

/*
void LineFollow::runLineFollow(){
  for(int count=0; count<5; count++)
  {
    lectura_sensor[count]=map(_IRread(count),sensor_negro[count],sensor_blanco[count],0,127);
    acu+=lectura_sensor[count];
  }

  //Serial.println(millis());
  if (acu > NIVEL_PARA_LINEA)
  {
    acu/=5;

    int error = ((lectura_sensor[0]<<6)+(lectura_sensor[1]<<5)-(lectura_sensor[3]<<5)-(lectura_sensor[4]<<6))/acu;

    error = constrain(error,-100,100);

    //Calculamos la correcion de velocidad mediante un filtro PD
    int vel = (error * KP)/10 + (error-last_error)*KD;

    last_error = error;

    //Corregimos la velocidad de avance con el error de salida del filtro PD
    int motor_left = constrain((robotSpeed + vel),-100,100);
    int motor_right =constrain((robotSpeed - vel),-100,100);

    //Movemos el robot
    //motorsWritePct(motor_left,motor_right);
    motorsWritePct(motor_left,motor_right);

    //Esperamos un poquito a que el robot reaccione
    delay(intergrationTime);
  }
  else
  {
    //Hemos encontrado una linea negra
    //perpendicular a nuestro camino
    //paramos el robot
    motorsStop();

    //y detenemos la ejecuci�n del programa
    //while(true);
	reportActionDone();
	//setMode(MODE_SIMPLE);
  }
}  
*/

bool Robot::followLine(){
    readings ir;
    ir = this->irArray.getReadings();

    if (!ir.r0 && !ir.r1){ // viss svart
        setLeftSpeed(this->speed - this->turnSpeedDiff);
        setRightSpeed(this->speed);
    }
    else if (!ir.r3 && !ir.r4){ //viss svart 
        setLeftSpeed(this->speed);
        setRightSpeed(this->speed-this->turnSpeedDiff);
    }
    else 
    {
        setSpeed(this->speed);
    }
    
}


bool Robot::autoDrive(){
    followLine(); // oppdaterer leftSpeed og rightSpeed
    // this->controller.startMove(this->leftSpeed/40, -this->rightSpeed/40);
    // this->controller.nextAction();
    // ca. 1cm per iterasjon, ved speed=400 mm/s
    this->controller.move(this->leftSpeed/5, -this->rightSpeed/5);  //rotate(2*1080, -2*1080);
    // this->controller.
}

void Robot::moveRobot(int steps1, int steps2){
    this->controller.move(steps1, -steps2);
}

void Robot::beginRobot(){
    this->controller.getMotor(0).begin(speed2rpm(this->speed), 1);
    this->controller.getMotor(1).begin(speed2rpm(this->speed), 1);

    
    this->controller.getMotor(0).setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, 1000, 1000);
    this->controller.getMotor(1).setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, 1000, 1000);

    this->irArray.calibrateIRs();
}

void Robot::setLeftSpeed(int _speed){
    // speed: int [mm/s]
    this->leftSpeed = _speed;
    this->controller.getMotor(0).setRPM(speed2rpm(_speed));
}

void Robot::setRightSpeed(int _speed){
    // speed: int [mm/s]
    this->rightSpeed = _speed;
    this->controller.getMotor(1).setRPM(speed2rpm(_speed));
}
