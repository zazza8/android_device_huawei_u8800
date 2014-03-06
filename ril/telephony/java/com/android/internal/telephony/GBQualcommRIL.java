/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.internal.telephony;

import static com.android.internal.telephony.RILConstants.*;

import android.content.Context;
import android.content.res.Resources;
import android.os.Message;
import android.os.Parcel;
import android.os.SystemProperties;
import android.text.TextUtils;

import com.android.internal.telephony.dataconnection.DataCallResponse;
import com.android.internal.telephony.dataconnection.DcFailCause;

public class GBQualcommRIL extends RIL implements CommandsInterface {

    public GBQualcommRIL(Context context, int networkMode, int cdmaSubscription) {
        super(context, networkMode, cdmaSubscription);
    }

    @Override
    public void
    supplyIccPinForApp(String pin, String aid, Message result) {
        //Note: This RIL request has not been renamed to ICC,
        //       but this request is also valid for SIM and RUIM
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_ENTER_SIM_PIN, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings
        rr.mParcel.writeInt(2);
        rr.mParcel.writeString(aid);
        rr.mParcel.writeString(pin);

        send(rr);
    }

    @Override public void
    supplyIccPukForApp(String puk, String newPin, String aid, Message result) {
        //Note: This RIL request has not been renamed to ICC,
        //       but this request is also valid for SIM and RUIM
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_ENTER_SIM_PUK, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings
        rr.mParcel.writeInt(3);
        rr.mParcel.writeString(aid);
        rr.mParcel.writeString(puk);
        rr.mParcel.writeString(newPin);

        send(rr);
    }

    @Override
    public void
    supplyIccPin2ForApp(String pin, String aid, Message result) {
        //Note: This RIL request has not been renamed to ICC,
        //       but this request is also valid for SIM and RUIM
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_ENTER_SIM_PIN2, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings
        rr.mParcel.writeInt(2);
        rr.mParcel.writeString(aid);
        rr.mParcel.writeString(pin);

        send(rr);
    }

    @Override
    public void
    supplyIccPuk2ForApp(String puk, String newPin2, String aid, Message result) {
        //Note: This RIL request has not been renamed to ICC,
        //       but this request is also valid for SIM and RUIM
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_ENTER_SIM_PUK2, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings
        rr.mParcel.writeInt(3);
        rr.mParcel.writeString(aid);
        rr.mParcel.writeString(puk);
        rr.mParcel.writeString(newPin2);

        send(rr);
    }

    @Override
    public void
    changeIccPinForApp(String oldPin, String newPin, String aid, Message result) {
        //Note: This RIL request has not been renamed to ICC,
        //       but this request is also valid for SIM and RUIM
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_CHANGE_SIM_PIN, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings
        rr.mParcel.writeInt(3);
        rr.mParcel.writeString(aid);
        rr.mParcel.writeString(oldPin);
        rr.mParcel.writeString(newPin);

        send(rr);
    }

    @Override
    public void
    changeIccPin2ForApp(String oldPin2, String newPin2, String aid, Message result) {
        //Note: This RIL request has not been renamed to ICC,
        //       but this request is also valid for SIM and RUIM
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_CHANGE_SIM_PIN2, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings
        rr.mParcel.writeInt(3);
        rr.mParcel.writeString(aid);
        rr.mParcel.writeString(oldPin2);
        rr.mParcel.writeString(newPin2);

        send(rr);
    }

    @Override
    public void
    supplyDepersonalization(String netpin, String type, Message result) {
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_ENTER_DEPERSONALIZATION_CODE, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest) +
                        " Type:" + type);

        rr.mParcel.writeInt(3);
        rr.mParcel.writeString(netpin);

        send(rr);
    }

    @Override
    public void
    queryFacilityLockForApp(String facility, String password, int serviceClass, String aid,
                            Message response) {
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_QUERY_FACILITY_LOCK, response);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings
        rr.mParcel.writeInt(4);

        rr.mParcel.writeString(aid);

        rr.mParcel.writeString(facility);
        rr.mParcel.writeString(password);

        rr.mParcel.writeString(Integer.toString(serviceClass));

        send(rr);
    }

    @Override
    public void
    setFacilityLockForApp(String facility, boolean lockState, String password,
                        int serviceClass, String aid, Message response) {
        String lockString;
         RILRequest rr
                = RILRequest.obtain(RIL_REQUEST_SET_FACILITY_LOCK, response);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings
        rr.mParcel.writeInt(5);

        rr.mParcel.writeString(aid);

        rr.mParcel.writeString(facility);
        lockString = (lockState)?"1":"0";
        rr.mParcel.writeString(lockString);
        rr.mParcel.writeString(password);
        rr.mParcel.writeString(Integer.toString(serviceClass));

        send(rr);

    }

    @Override
    protected Object
    responseSetupDataCall(Parcel p) {
        DataCallResponse dataCall = new DataCallResponse();
        String response[] = (String []) responseStrings(p);

        if (response.length >= 2) {
            dataCall.version = 4;
            dataCall.cid = Integer.parseInt(response[0]);
            dataCall.ifname = response[1];
            if (TextUtils.isEmpty(dataCall.ifname)) {
                throw new RuntimeException(
                        "RIL_REQUEST_SETUP_DATA_CALL response, no ifname");
            }
            String addresses = response[2];
            if (!TextUtils.isEmpty(addresses)) {
                dataCall.addresses = addresses.split(" ");
            }

            dataCall.dnses = new String[2];
            dataCall.dnses[0] = SystemProperties.get("net."+dataCall.ifname+".dns1");
            dataCall.dnses[1] = SystemProperties.get("net."+dataCall.ifname+".dns2");
        } else {
            dataCall.status = DcFailCause.ERROR_UNSPECIFIED.getErrorCode();
        }
        return dataCall;
    }

    @Override
    protected DataCallResponse getDataCallResponse(Parcel p, int version) {
        DataCallResponse dataCall = new DataCallResponse();

        dataCall.version = version;
        dataCall.cid = p.readInt();
        dataCall.active = p.readInt();
        dataCall.type = p.readString();
        String addresses = p.readString();
        if (!TextUtils.isEmpty(addresses)) {
            dataCall.addresses = addresses.split(" ");
        }
        // DataCallResponse needs an ifname. Since we don't have one use the name from the ThrottleService resource (default=rmnet0).
        dataCall.ifname = Resources.getSystem().getString(com.android.internal.R.string.config_datause_iface);

        dataCall.dnses = new String[2];
        dataCall.dnses[0] = SystemProperties.get("net."+dataCall.ifname+".dns1");
        dataCall.dnses[1] = SystemProperties.get("net."+dataCall.ifname+".dns2");
        return dataCall;
    }
}
