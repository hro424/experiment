package com.panasonic.sttd;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public abstract class CommandPacket {
    protected ByteArrayOutputStream packet;

    public CommandPacket() {
        packet = new ByteArrayOutputStream();
    }

    protected abstract int commandCode();

    protected abstract int totalLength();

    protected abstract int symbolTableOffset();

    static public abstract CommandPacket parse(byte[] buf, int len);

    protected void make() throws IOException {
        ByteBuffer bb;

        packet.write(commandCode());
        packet.write(0);
        packet.write(0);
        packet.write(0);

        bb = ByteBuffer.allocate(4);
        packet.write(bb.putInt(totalLength()).array());
        bb = ByteBuffer.allocate(4);
        packet.write(bb.putInt(symbolTableOffset()).array());
    }

    public final byte[] toByteArray() throws IOException {
        make();
        return packet.toByteArray();
    }
}

