package com.panasonic.sttd;

import com.panasonic.sttd.CommandPacket;

public final class AckCommandPacket extends CommandPacket {
    public AckCommandPacket() {
        super();
    }

    protected int commandCode() {
        return 0x12;
    }

    protected int totalLength() {
        return 12;
    }

    protected int symbolTableOffset() {
        return 0;
    }
}

