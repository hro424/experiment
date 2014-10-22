package com.panasonic.sttd;

import com.panasonic.sttd.CommandPacket;

public final class AbortCommandPacket extends CommandPacket {
    public AbortCommandPacket() {
        super();
    }

    protected int commandCode() {
        return 0x03;
    }

    protected int totalLength() {
        return 12;
    }

    protected int symbolTableOffset() {
        return 0;
    }

    public static CommandPacket parse(byte[] buf, int len) {
        if (buf[0] != commandCode()) {
            return null;
        }

        return new AbortCommandPacket();
    }
}

