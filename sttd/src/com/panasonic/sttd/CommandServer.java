package com.panasonic.sttd;

import java.io.DatagramPacket;
import java.io.DatagramSocket;
import java.io.IOException;

public class CommandServer extends Thread {

    private static final int BUFFER_SIZE = 4096;
    private DatagramSocket socket;

    public CommandServer(DatagramSocket s) {
        this.socket = s;
    }

    private CommandPacket parse(byte[] buf, int len) {
        CommandPacket p;
        switch (buf[0]) {
        case 0x01:
            p = StartCommandPacket.parse(buf, len);
            break;
        case 0x02:
            p = StopCommandPacket.parse(buf, len);
            break;
        case 0x03:
            p = AbortCommandPacket.parse(buf, len);
            break;
        default:
            p = null;
        }
        return p;
    }

    public void run() {
        byte[] buf = new byte[BUFFER_SIZE];
        DatagramPacket p = new DatagramPacket(buf, buf.length);
        CommandPacket cp;

        for (;;) {
            try {
                socket.receive(p);
                cp = parse(p.getData(), p.getLength());
                if (cp != null) {
                    cp.run();
                }
                else {
                    break;
                }
            } catch (IOException e) {
            }
        }
    }
}

