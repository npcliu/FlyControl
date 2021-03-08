#ifndef __INTERACT_H
#define __INTERACT_H 

extern char send_type;

void UARTSendFloat(float d);
void SCISend_to_Own(USART_TypeDef* USARTx);
void SendParametersToRC();

#define anglex_Node_Number 1500
#define angley_Node_Number 1
#define anglez_Node_Number 1

void NodeInit();
void SaveAngleDataToNode(float * angle);
void SendNodeDataToComputer();
void RCDenote();
//void UartInit();

#endif