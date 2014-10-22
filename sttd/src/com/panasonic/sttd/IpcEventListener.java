package com.panasonic.sttd;

import java.util.EventListener;

interface IpcEventListener extends EventListener {
    void handleEvent(byte[] buf);
}

