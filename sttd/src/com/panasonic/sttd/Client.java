package com.panasonic.sttd;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.net.ConnectException;

import com.panasonic.sttd.IpcEventListener;
import com.panasonic.sttd.CommandPacket;
import com.panasonic.sttd.StartCommandPacket;
import com.panasonic.sttd.StopCommandPacket;
import com.panasonic.sttd.Server;

public class Client {
    private DatagramSocket commandSocket;
    private Socket dataSocket;
    private OutputStream dataOut;

    private IpcEventListener listener;

    /*
    class Receiver extends Thread {
        void run() {
            if (commandIn == null) {
                return;
            }

            for (;;) {
                commandIn.receive();
                if (listener != null) {
                    rc = l.handleEvent(buf);
                    if (rc < 0) {
                        break;
                    }
                }
            }
        }
    }
    */

    public Client() {
    }

    public void connect() throws ConnectException, IOException {
        try {
            InetAddress serverAddr = InetAddress.getLocalHost();
            commandSocket = new DatagramSocket();
            commandSocket.connect(serverAddr, Server.COMMAND_PORT);
            //dataSocket = new Socket(serverAddr, Server.DATA_PORT);
            //dataOut = dataSocket.getOutputStream();
        } catch (Exception e) {
            this.close();
            throw new ConnectException();
        }
    }

    public void close() throws IOException {
        if (commandSocket != null) {
            commandSocket.close();
            commandSocket = null;
        }

        /*
        if (dataSocket != null) {
            dataSocket.close();
            dataSocket = null;
        }
        */
    }

    private void send(CommandPacket p) throws IOException {
        byte[] buf = p.toByteArray();
        System.out.println("send " + buf.length + " bytes");
        commandSocket.send(new DatagramPacket(buf, buf.length));
    }

    public void start(boolean continous, boolean interim, int alternatives,
               String lang, String serviceURI, Grammar[] grammars)
        throws IOException
    {
        StartCommandPacket packet = new StartCommandPacket();
        packet.write(continous, interim, alternatives, lang, serviceURI,
                     grammars);
        send(packet);
    }

    public void stop() throws IOException {
        send(new StopCommandPacket());
    }

    public void sendData(byte[] data) throws IOException {
    }

    public void setEventListener(IpcEventListener l) {
        listener = l;
    }
}

