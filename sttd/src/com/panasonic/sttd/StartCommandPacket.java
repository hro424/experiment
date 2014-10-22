package com.panasonic.sttd;

import com.panasonic.sttd.CommandPacket;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public final class StartCommandPacket extends CommandPacket {
    private ByteArrayOutputStream body;
    private ByteArrayOutputStream symbolTable;
    private short cur;
    private int total;
    private int offset;

    public StartCommandPacket() {
        super();
        body = new ByteArrayOutputStream();
        symbolTable = new ByteArrayOutputStream();
        cur = 0;
        total = 0;
        offset = 0;
    }

    protected int commandCode() {
        return 0x01;
    }

    protected int totalLength() {
        return total;
    }

    protected int symbolTableOffset() {
        return offset;
    }

    protected int commit(String s) throws IOException {
        byte[] buf = s.getBytes("US-ASCII");
        short len = (short)buf.length;

        body.write(ByteBuffer.allocate(2).putShort(cur).array());
        body.write(ByteBuffer.allocate(2).putShort(len).array());
        this.cur += len;

        symbolTable.write(buf);

        return len;
    }

    public void write(boolean continuous, boolean interim, int alternatives,
                      String lang, String serviceURI, Grammar[] grammars)
        throws IOException
    {
        int headerLength = 12;
        int bodyLength = 16;
        int symbolTableLength = 0;

        byte[] buf = ByteBuffer.allocate(4).putInt(alternatives).array();
        buf[0] |= (continuous ? 0x80 : 0) | (interim ? 0x40 : 0);
        body.write(buf);

        symbolTableLength += commit(lang);
        symbolTableLength += commit(serviceURI);
        body.write(ByteBuffer.allocate(4).putInt(grammars.length).array());

        for (Grammar g : grammars) {
            symbolTableLength += commit(g.name());
            body.write(ByteBuffer.allocate(4).putInt(g.weight()).array());
            bodyLength += 8;
        }

        total = headerLength + bodyLength + symbolTableLength;
        offset = bodyLength;
    }

    protected void make() throws IOException {
        super.make();
        body.writeTo(packet);
        symbolTable.writeTo(packet);
    }

    private String pick(byte[] symbolTable, int start, int length) {
        return new String(symbolTable, start, length);
    }

    private String pick(byte[] buf, int pos) {
        int offset = 12;
        ByteBuffer bb;

        bb = ByteBuffer.allocate(4);
        offset += bb.put(buf, 8, 4).getInt();
        bb = ByteBuffer.allocate(2);
        offset += bb.put(buf, pos, 2).getShort();
        bb = ByteBuffer.allocate(2);
        length = bb.put(buf, pos + 2, 2).getShort();

        return new String(buf, offset, length);
    }

    public static CommandPacket parse(byte[] buf, int len) {
        if (buf[0] != commandCode()) {
            return null;
        }

        lang = pick(buf, offset, length);
        offset += 4;
        serviceURI = pick(buf, offset, length);
        offset += 4;

        int max = ByteBuffer.allocate(4).put(buf, offset, 4).getInt();
        offset += 4;

        for (int i = 0; i < max; i++) {
            String s = pick(buf, offset);
            int w = ByteBuffer.allocate(4).put(buf, offset + 4, 4).getInt();
            grammers.push(new Grammar(s, w));
            offset += 8;
        }
    }
}

