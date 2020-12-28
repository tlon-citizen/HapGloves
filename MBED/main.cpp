/* Haptics Micro-controller Library
 * UHH HCI 2015
 * ariza@informatik.uni-hamburg.de
 */

#include "mbed.h"
#include "BLEDevice.h"
#include "HapticsController.h"
    
BLEDevice  ble;

static volatile bool triggerSensorPolling = false;

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    ble.startAdvertising(); // restart advertising
}

void onConnectionCallback(Gap::Handle_t handle, Gap::addr_type_t peerAddrType, const Gap::address_t peerAddr, const Gap::ConnectionParams_t *params)
{
    Gap::ConnectionParams_t cparams;
    ble.getPreferredConnectionParams(&cparams);
    
    Gap::ConnectionParams_t gap_conn_params;
    gap_conn_params.minConnectionInterval        = 16;
    gap_conn_params.maxConnectionInterval        = 16;
    gap_conn_params.connectionSupervisionTimeout = cparams.connectionSupervisionTimeout;
    gap_conn_params.slaveLatency                 = cparams.slaveLatency;
    
    ble.updateConnectionParams(handle, &gap_conn_params);
}

void periodicCallback(void)
{
    //led1 = !led1;
    triggerSensorPolling = true;
}

int main(void)
{
    led1 = 1;
    
    Ticker ticker;
    ticker.attach(periodicCallback, 0.05);

    ble.init();
    ble.onDisconnection(disconnectionCallback);
    ble.onConnection(onConnectionCallback);
    
    HCIHapticService hciHapticsService(ble, "UHH", "HCI", "HAPTICS", "hw-rev1", "fw-rev1", "soft-rev1" );
    
    const char DEVICE_NAME[]     = "HCI_HapGLeft";
    const uint16_t uuid16_list[] =   
    {
        GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE
        ,GattService::UUID_DEVICE_INFORMATION_SERVICE
    };
        
    /* Setup advertising. */
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.accumulateAdvertisingPayload(GapAdvertisingData::HUMAN_INTERFACE_DEVICE_HID);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(Gap::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(1000));
    ble.startAdvertising();

    while (true) 
    {
        if (triggerSensorPolling && ble.getGapState().connected) 
        {
            triggerSensorPolling = false;
            
            hciHapticsService.processInputs();
        } 
        else 
        {
            ble.waitForEvent();
        }
    }
}
