#include <app/common/gen/af-structs.h>
#include <app/common/gen/attribute-id.h>
#include <app/common/gen/cluster-id.h>
#include <app/reporting/reporting.h>
#include <core/CHIPError.h>
#include "nvs_flash.h"
#include <support/ErrorStr.h>
#include "esp_log.h"
#include "Device.h"
#include <app/server/Server.h>
#define DYNAMIC_ENDPOINT_COUNT 16
const char * TAG = "bridge-app";

using namespace chip;

static const int kUserLabelSize = 32;
// Current ZCL implementation of Struct uses a max-size array of 254 bytes
static const int kDescriptorAttributeArraySize = 254;
static const int kFixedLabelAttributeArraySize = 254;
// Four attributes in descriptor cluster: DeviceTypeList, ServerList, ClientList, PartsList
static const int kDescriptorAttributeCount          = 4;
static const int kFixedLabelElementsOctetStringSize = 16;

//ZCL format -> (len, string)
uint8_t * ToZclCharString(uint8_t * zclString, const char * cString, uint8_t maxLength)
{
    size_t len = strlen(cString);
    if (len > maxLength)
    {
        len = maxLength;
    }
    zclString[0] = static_cast<uint8_t>(len);
    memcpy(&zclString[1], cString, zclString[0]);
    return zclString;
}

//Converted into bytes and mapped the (label, value)
void EncodeFixedLabel(const char * label, const char * value, uint8_t * buffer, uint16_t length, EmberAfAttributeMetadata * am)
{
    char zclOctetStrBuf[kFixedLabelElementsOctetStringSize];
    uint16_t listCount = 1;
    _LabelStruct labelStruct;

    labelStruct.label = chip::ByteSpan(reinterpret_cast<const uint8_t *>(label), kFixedLabelElementsOctetStringSize);

    strncpy(zclOctetStrBuf, value, sizeof(zclOctetStrBuf));
    labelStruct.value = chip::ByteSpan(reinterpret_cast<uint8_t *>(&zclOctetStrBuf[0]), sizeof(zclOctetStrBuf));

    emberAfCopyList(ZCL_FIXED_LABEL_CLUSTER_ID, am, true, buffer, reinterpret_cast<uint8_t *>(&labelStruct), 1);
    emberAfCopyList(ZCL_FIXED_LABEL_CLUSTER_ID, am, true, buffer, reinterpret_cast<uint8_t *>(&listCount), 0);
}

void HandleDeviceStatusChanged(Device * dev, Device::Changed_t itemChangedMask)
{
    if (itemChangedMask & Device::kChanged_Reachable)
    {
        uint8_t reachable = dev->IsReachable() ? 1 : 0;
        emberAfReportingAttributeChangeCallback(dev->GetEndpointId(), ZCL_BRIDGED_DEVICE_BASIC_CLUSTER_ID,
                                                ZCL_REACHABLE_ATTRIBUTE_ID, CLUSTER_MASK_SERVER, 0, ZCL_BOOLEAN_ATTRIBUTE_TYPE,
                                                &reachable);
    }

    if (itemChangedMask & Device::kChanged_State)
    {
        uint8_t isOn = dev->IsOn() ? 1 : 0;
        emberAfReportingAttributeChangeCallback(dev->GetEndpointId(), ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID,
                                                CLUSTER_MASK_SERVER, 0, ZCL_BOOLEAN_ATTRIBUTE_TYPE, &isOn);
    }

    if (itemChangedMask & Device::kChanged_Name)
    {
        uint8_t zclName[kUserLabelSize];
        ToZclCharString(zclName, dev->GetName(), kUserLabelSize - 1);
        emberAfReportingAttributeChangeCallback(dev->GetEndpointId(), ZCL_BRIDGED_DEVICE_BASIC_CLUSTER_ID,
                                                ZCL_USER_LABEL_ATTRIBUTE_ID, CLUSTER_MASK_SERVER, 0, ZCL_CHAR_STRING_ATTRIBUTE_TYPE,
                                                zclName);
    }
    if (itemChangedMask & Device::kChanged_Location)
    {
        uint8_t buffer[kFixedLabelAttributeArraySize];
        EmberAfAttributeMetadata am = { .attributeId  = ZCL_LABEL_LIST_ATTRIBUTE_ID,
                                        .size         = kFixedLabelAttributeArraySize,
                                        .defaultValue = nullptr };

        EncodeFixedLabel("room", dev->GetLocation(), buffer, sizeof(buffer), &am);

        emberAfReportingAttributeChangeCallback(dev->GetEndpointId(), ZCL_FIXED_LABEL_CLUSTER_ID, ZCL_LABEL_LIST_ATTRIBUTE_ID,
                                                CLUSTER_MASK_SERVER, 0, ZCL_ARRAY_ATTRIBUTE_TYPE, buffer);
    }
}

static Device * gDevices[DYNAMIC_ENDPOINT_COUNT]; // number of dynamic endpoints count
extern "C" void app_main()
{
    int err = 0;
    // Initialize the ESP NVS layer.
    err = nvs_flash_init();
    if (err != CHIP_NO_ERROR)
    {
        ESP_LOGE(TAG, "nvs_flash_init() failed: %s", ErrorStr(err));
        return;
    }

    CHIP_ERROR chip_err = CHIP_NO_ERROR;

    //bridge will have own database named gDevices.
    //Clear database
    memset(gDevices, 0, sizeof(gDevices));

    //Bridged devices 4
    Device Light1("Light 1", "Office");
    Device Light2("Light 2", "Office");
    Device Light3("Light 3", "Kitchen");
    Device Light4("Light 4", "Den");

    //Whenever bridged device changes its state
    Light1.SetChangeCallback(&HandleDeviceStatusChanged);
    Light2.SetChangeCallback(&HandleDeviceStatusChanged);
    Light3.SetChangeCallback(&HandleDeviceStatusChanged);
    Light4.SetChangeCallback(&HandleDeviceStatusChanged);

    Light1.SetReachable(true);
    Light2.SetReachable(true);
    Light3.SetReachable(true);
    Light4.SetReachable(true);
    
    //Till above the bridged device was created, if there is a change in any attribute then with respective to its endpoint
    //the change is made.
    
    //Initialise CHIP with only BLE mode
/*    CHIPDeviceManager & deviceMgr = CHIPDeviceManager::GetInstance();

    err = deviceMgr.Init(&AppCallback);
    if (err != CHIP_NO_ERROR)
    {   
        ESP_LOGE(TAG, "device.Init() failed: %s", ErrorStr(err));
        return;
    }   

    InitServer();   */
}
