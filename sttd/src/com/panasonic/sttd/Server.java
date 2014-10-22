package com.panasonic.sttd:

import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;

import com.panasonic.sttd.CommandPacket;
import com.panasonic.sttd.StartCommandPacket;
import com.panasonic.sttd.AckCommandPacket;
import com.panasonic.sttd.StopCommandPacket;
import com.panasonic.sttd.ResultPacket;
import com.panasonic.sttd.ErrorPacket;

public class Server {
    public static final int COMMAND_PORT = 50000;
    public static final int DATA_PORT = 50001;
    private DatagramSocket commandSocket;
    private ServerSocket dataSocket;
    private CommandServer comServer;
    private DataServer dataServer;

    public void Server() {
    }

    public void start() {
        try {
            InetAddress addr = InetAddress.getLocalHost();

            commandSocket = new DatagramSocket(COMMAND_PORT, addr);
            dataSocket = new ServerSocket();
            dataSocket.bind(new InetSocketAddress(addr, DATA_PORT));

            comServer = new CommandServer(commandSocket);
            comServer.start();

            dataServer = new DataServer(dataSocket);
            dataServer.start();
        } catch (Exception e) {
            this.close();
            throw new IOException();
        }
    }

    public void stop() {
        if (commandSocket != null) {
            commandSocket.close();
            commandSocket = null;
        }
    }
}

