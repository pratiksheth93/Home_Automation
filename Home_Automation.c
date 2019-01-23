#include <project.h>
CYBLE_CONN_HANDLE_T connectionHandle;
CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
uint8 DevType,DevCode,DevParam,Reserve,p,OFF,q,r;


void HandleDevice(){
    if(DevType==0x01){
        switch(DevCode)
        {
            case 0x01 : 
            if (DevParam == 00)
            {
                PWM1_WriteCompare(p);
                OFF=0;
            }
            else
            {
                PWM1_WriteCompare(0);
                OFF=1;
            }
            break;
            
            case 0x02 : 
            if (DevParam == 0)
            {
                PWM2_WriteCompare(q);
                OFF=0;
            }
            else
            {
                PWM2_WriteCompare(0);
                OFF=1;
            }
            break;
            
            case 0x03 :
            if (DevParam == 0)
            {
                PWM3_WriteCompare(r);
                OFF=0;
            }
            else
            {
                PWM3_WriteCompare(0);
                OFF=1;
            }
            break;
            
            default: 
            break;
        }
    }
    
    else if(DevType==0x02) {
        switch(DevCode)
        {
            case 0x01: 
            if (OFF == 0)
            {
                p=DevParam;
                PWM1_WriteCompare(p);

            }
            else
            p=DevParam;
            break;
            
            case 0x02: 
            if (OFF == 0)
            {
                q=DevParam;
                PWM2_WriteCompare(q);

            }
            else
            q=DevParam;
            break;
            
            case 0x03: 
            if (OFF == 0)
            {
                r=DevParam;
                PWM3_WriteCompare(r);
            }
            else
            r=DevParam; 
            break;
        }
}
}

void CustomEventHandler(uint32 event, void * eventParam){
 switch(event)
{
    case CYBLE_EVT_STACK_ON: //EVENT CALLED WHEN BLE STACK STARTS
    CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
    break;
    
    case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
    CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST); //starts advertising once the device is disconnected
    break;
    
    case CYBLE_EVT_GATT_CONNECT_IND: //THIS is the statement when someone gets connected it indicates
    connectionHandle=*(CYBLE_CONN_HANDLE_T *)eventParam; //this will help us to the format of event parameter
    break;
    
    case CYBLE_EVT_GATTS_WRITE_REQ:
    wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam; //For data
    if(CYBLE_HOME_CONTROL_DEVICE_CONTROL_CHAR_HANDLE == wrReqParam->handleValPair.attrHandle) //if wrReqParam is passed we need pointer (->) its is like having a structure inside a structure
    {
        DevType  = (uint8)wrReqParam->handleValPair.value.val[0];
        DevCode  = (uint8)wrReqParam->handleValPair.value.val[1];
        DevParam = (uint8)wrReqParam->handleValPair.value.val[2];
        Reserve  = (uint8)wrReqParam->handleValPair.value.val[3];
        HandleDevice();
        CyBle_GattsWriteRsp(connectionHandle); //giving respone to the user
        
    }
    break;
    
    
    default: break;
    
}
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    Clock_Start();
   
    PWM1_Start();
    PWM2_Start();
    PWM3_Start();

    PWM1_WriteCompare(0);
    PWM2_WriteCompare(0);
    PWM3_WriteCompare(0);
    CyBle_Start(CustomEventHandler);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        CyBle_ProcessEvents();
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

