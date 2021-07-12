#include "Device.h"

#include <cstdio>
#include <platform/CHIPDeviceLayer.h>

Device::Device(const char * szDeviceName, const char * szLocation)
{
    strncpy(mName, szDeviceName, sizeof(mName));
    strncpy(mLocation, szLocation, sizeof(mLocation));
    mState      = kState_Off;
    mReachable  = false;
    mEndpointId = 0;
    mChanged_CB = nullptr;
}

bool Device::IsOn()
{
    return mState == kState_On;
}

bool Device::IsReachable()
{
    return mReachable;
}

void Device::SetOnOff(bool aOn)
{
    bool changed;

    if (aOn)
    {
        changed = (mState != kState_On);
        mState  = kState_On;
        ChipLogProgress(DeviceLayer, "Device[%s]: ON", mName);
    }
    else
    {
        changed = (mState != kState_Off);
        mState  = kState_Off;
        ChipLogProgress(DeviceLayer, "Device[%s]: OFF", mName);
    }

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_State);
    }
}

void Device::SetReachable(bool aReachable)
{
    bool changed = (mReachable != aReachable);

    mReachable = aReachable;

    if (aReachable)
    {
        ChipLogProgress(DeviceLayer, "Device[%s]: ONLINE", mName);
    }
    else
    {
        ChipLogProgress(DeviceLayer, "Device[%s]: OFFLINE", mName);
    }

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_Reachable);
    }
}

void Device::SetName(const char * szName)
{
    bool changed = (strncmp(mName, szName, sizeof(mName)) != 0);

    ChipLogProgress(DeviceLayer, "Device[%s]: New Name=\"%s\"", mName, szName);

    strncpy(mName, szName, sizeof(mName));

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_Name);
    }
}

void Device::SetLocation(const char * szLocation)
{
    bool changed = (strncmp(mLocation, szLocation, sizeof(mLocation)) != 0);

    strncpy(mLocation, szLocation, sizeof(mLocation));

    ChipLogProgress(DeviceLayer, "Device[%s]: Location=\"%s\"", mName, mLocation);

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_Location);
    }
}

void Device::SetChangeCallback(DeviceCallback_fn aChanged_CB)
{
    mChanged_CB = aChanged_CB;
}

